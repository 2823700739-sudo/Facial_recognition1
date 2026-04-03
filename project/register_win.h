#ifndef REGISTER_WIN_H
#define REGISTER_WIN_H

#include <QWidget>
#include <QCameraInfo>
#include <QCamera>
#include <QVideoWidget>
#include <QTransform>
#include<QCameraImageCapture>
#include<QCameraViewfinder>

//人脸识别相关
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <opencv2/opencv.hpp>
namespace Ui {
class register_win;
}

class register_win : public QWidget
{
    Q_OBJECT

public:
    explicit register_win(QWidget *parent = nullptr);
    ~register_win();
    
    // 声明提取特征的复用函数
    cv::Mat detect_and_extract_face(const QString &image_path);
    QString extract_face_feature(const cv::Mat &face_image);

private slots:
    void on_start_bt_clicked();

    void on_stop_bt_clicked();

    void on_takephoto_bt_clicked();

    void on_pushButton_clicked();

private:
    Ui::register_win *ui;
    QCamera *camera;
    QVideoWidget *win;
    QCameraImageCapture *image; // 图像抓拍器

};

#endif // REGISTER_WIN_H
