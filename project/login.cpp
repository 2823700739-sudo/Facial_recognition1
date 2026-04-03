#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// 静态成员初始化
QTcpSocket* login::clientSocket = nullptr;

QJsonObject login::sendSyncRequest(const QJsonObject &request)
{
    if (!clientSocket || clientSocket->state() != QAbstractSocket::ConnectedState) {
        return QJsonObject();
    }
    
    QByteArray sendData = QJsonDocument(request).toJson(QJsonDocument::Compact);
    clientSocket->write(sendData);
    clientSocket->flush();

    // 简单粗暴的同步等待包回发机制
    if (clientSocket->waitForReadyRead(3000)) {
        QByteArray recvData = clientSocket->readAll();
        return QJsonDocument::fromJson(recvData).object();
    }
    
    return QJsonObject();
}

login::login(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::login)
{
    ui->setupUi(this);

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
    
    // 原来的 connect 注释掉
    // connect(clientSocket, &QTcpSocket::readyRead, this, &login::onServerMessageRead);

    // 依然保留本地 SQLite 连接兼容你剩余未完全解耦读取网络的代码（推荐逐步替换）
    database=QSqlDatabase::addDatabase("QSQLITE");
    // 设置数据库文件路径名
    database.setDatabaseName("d:/four_project/Facial_recognition/note_db.db");
    // 打开数据库
    bool ret = database.open();
    if (ret)
    {
        qDebug() << "数据库打开成功";
    }
    else
    {
        qDebug() << "数据库打开失败";
    }
    // 创建数据表
    QSqlQuery query(database);

    // 创 users 表 - 单一人脸识别无密码
    ret = query.exec("create table if not exists users (user_id INTEGER primary key autoincrement, username TEXT UNIQUE NOT NULL, create_time TEXT NOT NULL, last_login TEXT, status INTEGER DEFAULT 0);");
    if (ret) qDebug() << "users 表创建成功";
    else qDebug() << "users 表创建失败" << query.lastError().text();

    // 创 user_faces 表
    ret = query.exec("create table if not exists user_faces (face_id INTEGER primary key autoincrement,user_id INTEGER not null,face_feature text not null,create_time text not null);");
    if (ret)
    {
        qDebug() << "用户人脸数据表创建成功";
    }
    else
    {
        qDebug() << "用户人脸数据表创建失败";
    }
    
    // 创 notes 表
    ret = query.exec("create table if not exists notes (note_id integer primary key autoincrement,user_id INTEGER NOT NULL,title TEXT,content TEXT,image_data TEXT,create_time TEXT NOT NULL,update_time TEXT NOT NULL);");
    if (ret)
    {
        qDebug() << "notes 表创建成功";
    }
    else
    {
        qDebug() << "notes 表创建失败";
    }

    // 创 sessions 表
    ret = query.exec("create table if not exists sessions (session_id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER NOT NULL, session_token TEXT UNIQUE NOT NULL, login_time TEXT NOT NULL, expire_time TEXT NOT NULL, client_ip TEXT);");
    if (ret) qDebug() << "sessions 表创建成功";
    else qDebug() << "sessions 表创建失败" << query.lastError().text();
}

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
