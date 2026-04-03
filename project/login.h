#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include "register_win.h"
#include "face_in.h"
#include "main_win.h"
QT_BEGIN_NAMESPACE
namespace Ui { class login; }
QT_END_NAMESPACE

class login : public QMainWindow
{
    Q_OBJECT

public:
    login(QWidget *parent = nullptr);
    ~login();
    
    // 全局静态的 socket 以便其他窗口复用
    static QTcpSocket* clientSocket;
    
    // 全局同步网络请求工具：发送JSON请求并等待（阻塞）服务端的JSON回包
    static QJsonObject sendSyncRequest(const QJsonObject &request);

private slots:
    void on_face_bt_clicked();

    void on_register_bt_clicked();

private:
    Ui::login *ui;
    QSqlDatabase database;
    register_win *res;//注册界面
    face_in *face_win;//人脸识别界面
    main_win *Main_win;//主菜单界面
};
#endif // LOGIN_H
