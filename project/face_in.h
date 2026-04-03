#ifndef FACE_IN_H
#define FACE_IN_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QEventLoop>
#include <QPixmapCache>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <vector>
#include <QString>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include "main_win.h"
namespace Ui {
class face_in;
}

class face_in : public QWidget
{
    Q_OBJECT

public:
    explicit face_in(QWidget *parent = nullptr);
    ~face_in();

signals:
    void loginSuccessful();

private slots:
    void processFrame();

private:
    Ui::face_in *ui;
    QLabel *videoLabel;
    cv::VideoCapture cap;
    QTimer *timer;
    QSqlDatabase database;
    main_win *Main_win;//主菜单界面
    // 存储所有已注册用户的人脸特征
    struct UserFeature {
        int user_id;
        QString username;
        std::vector<float> features;
    };
    std::vector<UserFeature> registered_users;

    void loadUserFeatures();
    cv::Mat QImageToMat(const QImage& image);
    QImage MatToQImage(const cv::Mat& mat);
};

#endif // FACE_IN_H
