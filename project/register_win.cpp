#include "register_win.h"
#include "ui_register_win.h"
#include "login.h"
#include <QLabel>
#include <QEventLoop>
#include <QPixmapCache>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <seeta/FaceEngine.h>
#include <seeta/Struct_cv.h>
#include <seeta/FaceRecognizer.h>
#include<QFile>
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
    this->setWindowTitle("注册界面");
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
    // 如果存在抓拍的缓存图片，每次关闭注册窗口都清除它
    QFile::remove("D:/four_project/Facial_recognition/take_picture/register/1.jpg");
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
    QFile file(imgPath);
    if (!file.exists()) {
        QMessageBox::warning(this, "警告", "请优先开启摄像头并抓拍一张人脸照片！");
        return;
    }

    // 2. 获取用户名
    QString username = ui->lineEdit->text().trimmed();
    if (username.isEmpty()) {
        qDebug() << "注册失败：用户名不能为空！";
        QMessageBox::warning(this, "警告", "注册失败：用户名不能为空！");
        return;
    }

    // 3. 调用你的人脸特征提取函数，得到特征字符串
    cv::Mat faceImg = detect_and_extract_face(imgPath);
    if (faceImg.empty())
    {
        qDebug() << "注册失败：画面中未检测到人脸！";
        QMessageBox::warning(this, "警告", "注册失败：画面中未检测到清晰人脸！请确保正对摄像头重新抓拍！");
        return;
    }
    QString featureStr = extract_face_feature(faceImg); // 提取到的人脸特征字符串
    if (featureStr.isEmpty() || featureStr.length() < 10) {
        QMessageBox::warning(this, "警告", "人脸特征提取失败，请重试！");
        return;
    }

    // 4. 发送数据到服务端
    if (login::clientSocket && login::clientSocket->state() == QAbstractSocket::ConnectedState) {
        QJsonObject jsonObj;
        jsonObj.insert("action", "register");
        jsonObj.insert("username", username);
        jsonObj.insert("features", featureStr);//把人脸特征字符串发送给服务端
        jsonObj.insert("create_time", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        
        QJsonDocument doc(jsonObj);
        QByteArray data = doc.toJson(QJsonDocument::Compact);
        
        login::clientSocket->write(data);// 发送数据
        login::clientSocket->flush();//刷新缓冲区确保数据发送出去
        qDebug() << "注册请求已发送给服务端!";
    } else {
        qDebug() << "未连接到服务端，无法注册！";
        QMessageBox::critical(this, "错误", "未连接到服务端，无法注册！");
    }

    this->close();
}
