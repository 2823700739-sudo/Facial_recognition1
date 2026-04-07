#include "face_in.h"
#include "ui_face_in.h"
#include "login.h"  // 加入 login 以便调用网络 sync 方法
#include <seeta/FaceEngine.h>
#include <seeta/Struct_cv.h>
#include <seeta/FaceRecognizer.h>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

static seeta::FaceEngine* engin = nullptr;

void init_seetaface() {
    if (!engin) {
        QString model_dir = "D:/four_project/Facial_recognition/opencv-SeetaFace-qt5.14.0minGW/SeetaFace/bin/model/";
        static seeta::ModelSetting FD_model((model_dir + "fd_2_00.dat").toStdString(), seeta::ModelSetting::CPU, 0);
        static seeta::ModelSetting PD_model((model_dir + "pd_2_00_pts5.dat").toStdString(), seeta::ModelSetting::CPU, 0);
        static seeta::ModelSetting FR_model((model_dir + "fr_2_10.dat").toStdString(), seeta::ModelSetting::CPU, 0);
        engin = new seeta::FaceEngine(FD_model, PD_model, FR_model);
    }
}

face_in::face_in(QWidget *parent, bool unlock_mode, int target_user_id) : QWidget(parent),
                                    ui(new Ui::face_in),
                                    is_unlock_mode(unlock_mode),
                                    m_target_user_id(target_user_id)
{
    ui->setupUi(this);
    this->setWindowTitle("人脸识别界面");
    // Initializing SeetaFace globally
    init_seetaface();

    // 创建用于实时显示画面的控件
    videoLabel = new QLabel(ui->widget);
    videoLabel->resize(ui->widget->size());
    videoLabel->setScaledContents(true);
    videoLabel->show();

    // 准备数据库并加载特征
    database = QSqlDatabase::database(); // 获取已有连接
    if (!database.isOpen()) {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("d:/four_project/Facial_recognition/note_db.db");
        database.open();
    }
    loadUserFeatures();

    // 打开系统默认(索引0)摄像头
    if (!cap.open(0)) {
        QMessageBox::critical(this, "错误", "无法打开本地摄像头！");
        return;
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &face_in::processFrame);//通过定时器来实时读取人脸数据
    // 每35ms抓取一帧(~28fps)
    timer->start(35);
}

face_in::~face_in()
{
    if (timer->isActive()) timer->stop();
    if (cap.isOpened()) cap.release();
    delete ui;
}

void face_in::loadUserFeatures()
{
    // 向服务器请求所有用户面部特征数据
    QJsonObject req;
    req.insert("action", "load_features");
    
    QJsonObject res = login::sendSyncRequest(req);
    if(res.value("status").toString() != "success") {
        qDebug() << "获取人脸数据失败: " << res.value("msg").toString();
        return;
    }
    
    QJsonArray usersArray = res.value("users").toArray();
    
    // 初始化 SeetaFace 的提取器以获取预期的特征长度
    init_seetaface();
    seeta::FaceRecognizer* fr = engin->FDB.ExtractionCore(0);
    int expected_size = fr->GetExtractFeatureSize();

    for (int i = 0; i < usersArray.size(); ++i) {
        QJsonObject userObj = usersArray[i].toObject();
        int uid = userObj.value("user_id").toInt();
        QString uname = userObj.value("username").toString();
        QString featureStr = userObj.value("face_feature").toString();

        QStringList list = featureStr.split(",", QString::SkipEmptyParts);

        if (list.size() == expected_size) {
            UserFeature uf;
            uf.user_id = uid;
            uf.username = uname;
            uf.features.resize(expected_size);
            for (int j = 0; j < expected_size; ++j) {
                uf.features[j] = list.at(j).toFloat();
            }
            registered_users.push_back(uf);
        }
    }
    qDebug() << "已成功从云端加载注册用户人脸数量：" << registered_users.size();
}

void face_in::processFrame()
{
    if (!cap.isOpened()) return;

    cv::Mat frame;
    cap >> frame;// 从摄像头读取一帧图像
    if (frame.empty()) return;

    // 图像镜像翻转 (通常摄像头读取自带镜像)
    cv::flip(frame, frame, 1);
    cv::Mat displayFrame = frame.clone(); // 用于后期绘制矩形框显示

    // SeetaFace要求输入数据格式
    seeta::cv::ImageData simage(frame);
    auto faces = engin->DetectFaces(simage);

    bool matchFound = false;

    if (faces.size() > 0) {
        // 在检测到脸上画框
        cv::Rect r(faces[0].pos.x, faces[0].pos.y, faces[0].pos.width, faces[0].pos.height);
        cv::rectangle(displayFrame, r, cv::Scalar(0, 255, 0), 2);

        // 如果还有数据未比对可以尝试提取当前特征
        if (!registered_users.empty()) {
            std::vector<SeetaPointF> points = engin->DetectPoints(simage, faces[0].pos);
            seeta::FaceRecognizer* fr = engin->FDB.ExtractionCore(0);
            int feature_size = fr->GetExtractFeatureSize();
            std::vector<float> current_features(feature_size);

            if (fr->Extract(simage, points.data(), current_features.data())) {
                double max_sim = 0.0;
                int best_user_id = -1;
                QString best_username = "";

                for (const auto& u : registered_users) {
                    double sim = fr->CalculateSimilarity(current_features.data(), u.features.data());
                    if (sim > max_sim) {
                        max_sim = sim;
                        best_user_id = u.user_id;
                        best_username = u.username;
                    }
                }

                if (max_sim > 0.75) {
                    if (is_unlock_mode && best_user_id != m_target_user_id) {
                        qDebug() << "识别到人脸，但不是当前登录用户（禁止解锁）";
                    } else {
                        matchFound = true;
                        qDebug() << "识别成功! 用户ID:" << best_user_id << "用户名:" << best_username << " 相似度:" << max_sim;

                        // 停止定时器与摄像头，防重复触发
                        timer->stop();
                        cap.release();// 释放摄像头资源
                        
                        // 如果这是解锁模式，且识别成功的用户就是当前登录用户，则直接关闭蒙版；如果这是普通登录模式，则打开主界面
                        if (!is_unlock_mode) {
                            Main_win=new main_win(nullptr, best_user_id, best_username);//识别成功后打开主界面，并传入用户 ID 和用户名以便显示欢迎语和后续网络请求使用
                            Main_win->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时，删除窗口
                            Main_win->setWindowModality(Qt::ApplicationModal); //窗口为模态窗口，只能在窗口关闭后才能操作
                            Main_win->show();
                        }
                        
                        // 发送信号给主窗口(login)让它关闭
                        emit loginSuccessful();

                        //关闭自己(face_in)
                        this->close();
                    
                        return; 
                    }
                }
            }
        }
    }

    // 更新画面
    cv::cvtColor(displayFrame, displayFrame, cv::COLOR_BGR2RGB);
    QImage img((const unsigned char*)(displayFrame.data), displayFrame.cols, displayFrame.rows, displayFrame.step, QImage::Format_RGB888);
    videoLabel->setPixmap(QPixmap::fromImage(img));
}

