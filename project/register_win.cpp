#include "register_win.h"
#include "ui_register_win.h"
#include <QLabel>
#include <QEventLoop>
#include <QPixmapCache>
#include <seeta/FaceEngine.h>
#include <seeta/Struct_cv.h>
#include <seeta/FaceRecognizer.h>

static seeta::FaceEngine* engin_reg = nullptr;

void init_seetaface_reg() {
    if (!engin_reg) {
        QString model_dir = "D:/four_project/Facial_recognition/opencv-SeetaFace-qt5.14.0minGW/SeetaFace/bin/model/";
        seeta::ModelSetting FD_model((model_dir + "fd_2_00.dat").toStdString(), seeta::ModelSetting::CPU, 0);
        seeta::ModelSetting PD_model((model_dir + "pd_2_00_pts5.dat").toStdString(), seeta::ModelSetting::CPU, 0);
        seeta::ModelSetting FR_model((model_dir + "fr_2_10.dat").toStdString(), seeta::ModelSetting::CPU, 0);
        engin_reg = new seeta::FaceEngine(FD_model, PD_model, FR_model);
    }
}

// 人脸检测（不再进行裁剪，仅判断有没有人脸，返回原图）
cv::Mat register_win::detect_and_extract_face(const QString &image_path)
{
    init_seetaface_reg();

    // 读取图片
    cv::Mat image = cv::imread(image_path.toStdString());
    if (image.empty())
    {
        qDebug() << "无法读取图片！";
        return cv::Mat();
    }

    seeta::cv::ImageData simage(image);
    auto faces = engin_reg->DetectFaces(simage);

    if (faces.empty())
    {
        qDebug() << "未检测到人脸！";
        return cv::Mat();
    }

    // 如果检测到人脸，直接返回源图像，交给 extract 特征阶段处理
    return image;
}

// 提取人脸特征（SeetaFace版）
QString register_win::extract_face_feature(const cv::Mat &face_image)
{
    init_seetaface_reg();

    seeta::cv::ImageData simage(face_image);
    auto faces = engin_reg->DetectFaces(simage);
    if (faces.empty()) {
        return "";
    }

    std::vector<SeetaPointF> points = engin_reg->DetectPoints(simage, faces[0].pos);
    
    // 提取特征值
    seeta::FaceRecognizer* fr = engin_reg->FDB.ExtractionCore(0);
    int feature_size = fr->GetExtractFeatureSize();
    float* features = new float[feature_size];
    fr->Extract(simage, points.data(), features);

    // 将特征拼成字符串返回
    QString feature_str;
    for (int i = 0; i < feature_size; i++)
    {
        feature_str += QString::number(features[i], 'f', 6) + ",";
    }
    
    delete[] features;
    return feature_str;
}

register_win::register_win(QWidget *parent) : QWidget(parent),
                                              ui(new Ui::register_win)
{
    ui->setupUi(this);
    // 获取系统中所有可以使用的摄像头信息
    QList<QCameraInfo> camlist = QCameraInfo::availableCameras();
    // 遍历容器
    for (int i = 0; i < camlist.size(); i++)
    {
        qDebug() << "摄像头描述信息是: " << camlist.at(i).description();
        // 在下拉框显示当前摄像头的名称
        ui->comboBox->addItem(camlist.at(i).deviceName());
    }
}

register_win::~register_win()
{
    delete ui;
}

// 启动摄像头
void register_win::on_start_bt_clicked()
{
    // 获取下拉框你选择的那个摄像头的设备名字
    QString camname = ui->comboBox->currentText();
    // 依据摄像头的设备名称创建摄像头对象
    camera = new QCamera(camname.toUtf8(), this);

    // 准备窗口
    win = new QVideoWidget(ui->widget);
    // 调整窗口大小
    win->resize(ui->widget->width(), ui->widget->height());
    // 显示窗口
    win->show();

    // 窗口跟摄像头对象绑定
    camera->setViewfinder(win);

    // 启动摄像头
    camera->start();

    // 4. 初始化图像抓拍器（核心：负责抓拍画面）
    image = new QCameraImageCapture(camera);
    // 设置抓拍质量：高质量
    image->setCaptureDestination(QCameraImageCapture::CaptureToFile);
}

// 关闭摄像头
void register_win::on_stop_bt_clicked()
{
    // 关闭摄像头
    camera->stop();
    win->hide();
}

// 拍照
void register_win::on_takephoto_bt_clicked()
{
    // 清除图片缓存，防止加载同一路径由于缓存原因导致图片不刷新
    QPixmapCache::clear();

    // 创建事件循环，等待异步抓拍真正保存到本地完成
    QEventLoop loop;
    connect(image, &QCameraImageCapture::imageSaved, &loop, &QEventLoop::quit);

    image->capture("D:/four_project/Facial_recognition/take_picture/register/1.jpg");

    // 阻塞在这里等待信号，图片真实落地后才会退出并继续往下走
    loop.exec();

    // 关闭摄像头
    camera->stop();
    win->hide();

    // 查找是否已经有为了显示图片而创建的QLabel，防止每次点击都new一个导致内存泄漏并堆叠
    QLabel *picWin = ui->widget->findChild<QLabel *>("captureLabel");
    if (!picWin)
    {
        picWin = new QLabel(ui->widget);
        picWin->setObjectName("captureLabel");
    }

    picWin->resize(ui->widget->width(), ui->widget->height());
    QPixmap pixmap("D:/four_project/Facial_recognition/take_picture/register/1.jpg");
    picWin->setPixmap(pixmap);
    picWin->setScaledContents(true);
    picWin->show();
    qDebug() << "抓拍成功！";
}

void register_win::on_pushButton_clicked()
{
    // 1. 获取当前抓拍好的照片路径
    QString imgPath = "D:/four_project/Facial_recognition/take_picture/register/1.jpg";

    // 2. 获取用户名
    QString username = ui->lineEdit->text().trimmed();
    if (username.isEmpty()) {
        qDebug() << "注册失败：用户名不能为空！";
        // 这里可以弹窗提示用户
        return;
    }

    // 3. 调用你的人脸特征提取函数，得到特征字符串
    cv::Mat faceImg = detect_and_extract_face(imgPath);
    if (faceImg.empty())
    {
        qDebug() << "注册失败：画面中未检测到人脸！";
        return;
    }
    QString featureStr = extract_face_feature(faceImg); // 提取到的人脸特征字符串

    // 4. 执行SQL存入数据库
    QSqlDatabase db = QSqlDatabase::database(); // 获取已在 login 连好的默认数据库
    if (!db.isOpen())
    {
        qDebug() << "数据库未连接！";
        return;
    }

    // --- 首先先插入新用户到 users 表 ---
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, create_time) VALUES (:username, :create_time)");
    query.bindValue(":username", username);
    query.bindValue(":create_time", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    
    if (!query.exec()) {
        qDebug() << "创建用户失败：" << query.lastError().text();
        // 可能是用户名重复
        return;
    }
    
    // 获取刚插入用户的 user_id
    int new_user_id = query.lastInsertId().toInt();
    
    // --- 接着插入人脸特征到 user_faces 表 ---
    query.prepare("INSERT INTO user_faces (user_id, face_feature, create_time) "
                  "VALUES (:user_id, :face_feature, :create_time)");
    
    // 绑定参数值
    query.bindValue(":user_id", new_user_id); 
    query.bindValue(":face_feature", featureStr);
    query.bindValue(":create_time", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    // 尝试执行
    if (query.exec())
    {
        qDebug() << "人脸数据成功写入数据库！ 用户ID:" << new_user_id;
    }
    else
    {
        qDebug() << "人脸数据写入失败：" << query.lastError().text();
    }
}
