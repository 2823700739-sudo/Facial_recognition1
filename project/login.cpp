#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// 静态成员初始化
QTcpSocket* login::clientSocket = nullptr;

// 全局同步网络请求工具：发送JSON请求并等待（阻塞）服务端的JSON回包
QJsonObject login::sendSyncRequest(const QJsonObject &request)
{
    if (!clientSocket || clientSocket->state() != QAbstractSocket::ConnectedState) {
        return QJsonObject();
    }
    
    QByteArray sendData = QJsonDocument(request).toJson(QJsonDocument::Compact);// 转成紧凑的 JSON 格式
    clientSocket->write(sendData);// 发送数据
    clientSocket->flush();// 确保数据发送出去

    // 同步等待服务端回包，设置超时时间为 3 秒
    if (clientSocket->waitForReadyRead(3000)) {
        QByteArray recvData = clientSocket->readAll();// 读取服务端回包
        return QJsonDocument::fromJson(recvData).object();// 转回 JSON 对象
    }
    
    return QJsonObject();
}
// 刷新并加载用户的所有便签
login::login(QWidget *parent): QMainWindow(parent), ui(new Ui::login)
{
    ui->setupUi(this);
    this->setWindowTitle("登入界面");
    // 初始化全局 TCP Socket 并连接到服务端
    if (clientSocket == nullptr) {
        clientSocket = new QTcpSocket();
        clientSocket->connectToHost(QHostAddress("192.168.11.84"), 7777);
        
        if (clientSocket->waitForConnected(2000)) {
            qDebug() << "连接服务端成功！" << clientSocket->peerAddress().toString();
        } else {
            qDebug() << "连接服务端失败，请检查服务端是否启动。";
            QMessageBox::warning(this, "警告", "无法连接到服务器 192.168.11.84:7777 !");
        }
    }
}

//
login::~login()
{
    if (clientSocket && clientSocket->isOpen()) {
        clientSocket->disconnectFromHost();
        clientSocket->deleteLater();
        clientSocket = nullptr;
    }
    delete ui;
}

//人脸识别登入按钮
void login::on_face_bt_clicked()
{
    face_win=new face_in();
    face_win->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时，删除窗口        
    face_win->setWindowModality(Qt::ApplicationModal); //窗口为模态窗口，只能在窗口关闭后才能操作
    
    // 连接信号：当登入成功时关闭当前登入界面
    connect(face_win, &face_in::loginSuccessful, this, &login::close);
    face_win->show();
}

//注册按钮
void login::on_register_bt_clicked()
{
    res = new register_win();
    res->setAttribute(Qt::WA_DeleteOnClose); //关闭注册窗口时，删除注册窗口
    res->setWindowModality(Qt::ApplicationModal); //注册窗口为模态窗口，只能在注册窗口关闭后才能操作
    res->show();
}

