#include "face_in.h"
#include "ui_face_in.h"
#include <seeta/FaceEngine.h>
#include <seeta/Struct_cv.h>
#include <seeta/FaceRecognizer.h>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

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

face_in::face_in(QWidget *parent) : QWidget(parent),
                                    ui(new Ui::face_in)
{
    ui->setupUi(this);

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
    connect(timer, &QTimer::timeout, this, &face_in::processFrame);
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
    if (!database.isOpen()) return;

    QSqlQuery query(database);
    query.exec("SELECT u.user_id, u.username, f.face_feature FROM user_faces f JOIN users u ON u.user_id = f.user_id");

    while (query.next()) {
        int uid = query.value(0).toInt();
        QString uname = query.value(1).toString();
        QString featureStr = query.value(2).toString();

        QStringList list = featureStr.split(",", QString::SkipEmptyParts);
        
        seeta::FaceRecognizer* fr = engin->FDB.ExtractionCore(0);
        int expected_size = fr->GetExtractFeatureSize();

        if (list.size() == expected_size) {
            UserFeature uf;
            uf.user_id = uid;
            uf.username = uname;
            uf.features.resize(expected_size);
            for (int i = 0; i < expected_size; ++i) {
                uf.features[i] = list.at(i).toFloat();
            }
            registered_users.push_back(uf);
        }
    }
    qDebug() << "已成功加载注册用户人脸数量：" << registered_users.size();
}

void face_in::processFrame()
{
    if (!cap.isOpened()) return;

    cv::Mat frame;
    cap >> frame;
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

                if (max_sim > 0.6) {
                    matchFound = true;
                    qDebug() << "识别成功! 用户ID:" << best_user_id << "用户名:" << best_username << " 相似度:" << max_sim;

                    // 停止定时器与摄像头，防重复触发
                    timer->stop();
                    cap.release();
                    
                    Main_win=new main_win(nullptr, best_user_id, best_username);
                    Main_win->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时，删除窗口
                    Main_win->setWindowModality(Qt::ApplicationModal); //窗口为模态窗口，只能在窗口关闭后才能操作
                    Main_win->show();
                    
                    // 发送信号给主窗口(login)让它关闭
                    emit loginSuccessful();

                    //关闭自己(face_in)
                    this->close();
                
                    return; 
                }
            }
        }
    }

    // 更新画面
    cv::cvtColor(displayFrame, displayFrame, cv::COLOR_BGR2RGB);
    QImage img((const unsigned char*)(displayFrame.data), displayFrame.cols, displayFrame.rows, displayFrame.step, QImage::Format_RGB888);
    videoLabel->setPixmap(QPixmap::fromImage(img));
}

