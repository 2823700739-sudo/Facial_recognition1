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
    explicit face_in(QWidget *parent = nullptr, bool unlock_mode = false, int target_user_id = -1);
    ~face_in();

signals:
    void loginSuccessful();

private slots:
    void processFrame();

private:
    Ui::face_in *ui;
    bool is_unlock_mode;// 是否为解锁模式（true 代表这是为了验证当前用户身份以解锁便签）
    int m_target_user_id;// 解锁模式下的目标用户 ID（只有识别到这个用户才允许解锁）
    QLabel *videoLabel;// 用于显示摄像头画面
    cv::VideoCapture cap;// OpenCV 视频捕获对象
    QTimer *timer;// 定时器用于定时抓取摄像头画面进行处理
    QSqlDatabase database;// 数据库连接对象
    main_win *Main_win;//主菜单界面
    // 存储所有已注册用户的人脸特征
    struct UserFeature {
        int user_id;
        QString username;// 方便调试显示用户名
        std::vector<float> features;// 存储人脸特征的向量（长度由 SeetaFace 的 FR 模块决定）
    };
    std::vector<UserFeature> registered_users;

    void loadUserFeatures();
    cv::Mat QImageToMat(const QImage& image);
    QImage MatToQImage(const cv::Mat& mat);
};

#endif // FACE_IN_H
