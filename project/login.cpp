#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QSqlError>

login::login(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::login)
{
    ui->setupUi(this);

    // 安装数据库驱动
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
