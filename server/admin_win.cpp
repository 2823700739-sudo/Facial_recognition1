#include "admin_win.h"
#include "ui_admin_win.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHostAddress>

admin_win::admin_win(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::admin_win)
{
    ui->setupUi(this);
    this->setWindowTitle("服务端监控管理系统");

    // 初始化界面表头
    ui->clientTable->setColumnCount(3);
    ui->clientTable->setHorizontalHeaderLabels({"IP 地址", "端口", "状态"});
    ui->clientTable->horizontalHeader()->setStretchLastSection(true);

    // 初始化数据库
    initDatabase();

    // 初始化TCP服务器
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress("192.168.11.84"), 7777)) {
        logMessage("错误：服务器启动失败：" + tcpServer->errorString());
    } else {
        logMessage("服务器启动成功，正在监听端口 7777...");
    }

    connect(tcpServer, &QTcpServer::newConnection, this, &admin_win::onNewConnection);
}

admin_win::~admin_win()
{
    // 如果有未关闭的连接则将其闭合
    foreach(QTcpSocket* socket, clientInfos.keys()) {
        socket->disconnectFromHost();
    }
    tcpServer->close();
    delete ui;
}

void admin_win::logMessage(const QString &msg)
{
    QString time = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss] ");
    ui->logTextEdit->append(time + msg);
}

void admin_win::initDatabase()
{
    // 在这里配置数据库，目前存在于 "d:/four_project/Facial_recognition/note_db.db"
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("d:/four_project/Facial_recognition/note_db.db");
    
    if (!db.open()) {
        logMessage("错误：数据库连接失败 - " + db.lastError().text());
        return;
    }
    
    logMessage("数据库连接成功！");
    
    // 初始化表结构，防止初次运行没有表
    QSqlQuery query(db);
    
    // 用户表
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "user_id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "username TEXT UNIQUE,"
               "create_time TEXT"
               ")");
    // 人脸特征表
    query.exec("CREATE TABLE IF NOT EXISTS user_faces ("
               "face_id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "user_id INTEGER,"
               "face_feature TEXT,"
               "create_time TEXT,"
               "FOREIGN KEY(user_id) REFERENCES users(user_id)"
               ")");
    // 便签表
    query.exec("CREATE TABLE IF NOT EXISTS notes ("
               "note_id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "user_id INTEGER,"
               "title TEXT,"
               "content TEXT,"
               "create_time TEXT,"
               "update_time TEXT,"
               "FOREIGN KEY(user_id) REFERENCES users(user_id)"
               ")");
}

void admin_win::updateClientTable()
{
    ui->clientTable->setRowCount(0);
    int row = 0;
    QMapIterator<QTcpSocket*, QString> i(clientInfos);
    while (i.hasNext()) {
        i.next();
        QTcpSocket* socket = i.key();
        ui->clientTable->insertRow(row);
        ui->clientTable->setItem(row, 0, new QTableWidgetItem(socket->peerAddress().toString()));
        ui->clientTable->setItem(row, 1, new QTableWidgetItem(QString::number(socket->peerPort())));
        ui->clientTable->setItem(row, 2, new QTableWidgetItem(i.value()));
        row++;
    }
}

void admin_win::onNewConnection()
{
    while (tcpServer->hasPendingConnections()) {
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        if (socket) {
            QString ip = socket->peerAddress().toString();
            logMessage("新客户端已连接：" + ip + ":" + QString::number(socket->peerPort()));
            clientInfos.insert(socket, "已连接");
            updateClientTable();

            connect(socket, &QTcpSocket::readyRead, this, &admin_win::onReadyRead);
            connect(socket, &QTcpSocket::disconnected, this, &admin_win::onClientDisconnected);
        }
    }
}

void admin_win::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;
    
    // 可能一次收到多条消息，这里按简单方式处理，更严谨的通常有包头包尾解析
    QByteArray data = socket->readAll();
    processClientRequest(socket, data);
}

void admin_win::onClientDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QString ip = socket->peerAddress().toString();
    logMessage("客户端断开连接：" + ip + ":" + QString::number(socket->peerPort()));

    clientInfos.remove(socket);
    updateClientTable();
    socket->deleteLater();
}

void admin_win::processClientRequest(QTcpSocket* socket, const QByteArray& data)
{
    // 这里简单设计为一串 JSON 命令
    // {"action":"register", "username":"test", "features":"0.1,0.2..."}
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        logMessage("收到未知/非法格式的数据请求：" + QString(data));
        return;
    }

    QJsonObject obj = doc.object();
    QString action = obj.value("action").toString();

    // 根据 action 区分客户端来的注册/读取请求，然后回发处理结果
    logMessage("收到客户端请求动作: " + action);
    
    QJsonObject response;
    response.insert("action_reply", action);

    if (action == "register") {
        QString username = obj.value("username").toString();
        QString features = obj.value("features").toString();
        QString create_time = obj.value("create_time").toString();

        // 插入 users 表
        QSqlQuery query(db);
        query.prepare("INSERT INTO users (username, create_time) VALUES (:username, :create_time)");
        query.bindValue(":username", username);
        query.bindValue(":create_time", create_time);
        
        if (query.exec()) {
            int user_id = query.lastInsertId().toInt();
            
            // 插入 user_faces 表
            QSqlQuery faceQuery(db);
            faceQuery.prepare("INSERT INTO user_faces (user_id, face_feature) VALUES (:user_id, :features)");
            faceQuery.bindValue(":user_id", user_id);
            faceQuery.bindValue(":features", features);
            
            if (faceQuery.exec()) {
                response.insert("status", "success");
                response.insert("msg", "账户 " + username + " 注册成功！");
                logMessage("账户 " + username + " 在服务端注册成功");
            } else {
                response.insert("status", "error");
                response.insert("msg", "特征录入失败: " + faceQuery.lastError().text());
                logMessage("特征录入失败: " + faceQuery.lastError().text());
            }
        } else {
            response.insert("status", "error");
            response.insert("msg", "账户名可能已存在/创建失败: " + query.lastError().text());
            logMessage("创建用户失败: " + query.lastError().text());
        }
    } 
    else if (action == "load_features") {
        QJsonArray usersArray;
        QSqlQuery query(db);
        query.exec("SELECT u.user_id, u.username, f.face_feature FROM user_faces f JOIN users u ON u.user_id = f.user_id");
        while (query.next()) {
            QJsonObject userObj;
            userObj.insert("user_id", query.value(0).toInt());
            userObj.insert("username", query.value(1).toString());
            userObj.insert("face_feature", query.value(2).toString());
            usersArray.append(userObj);
        }
        response.insert("status", "success");
        response.insert("users", usersArray);
    }
    else if (action == "load_notes") {
        int user_id = obj.value("user_id").toInt();
        QJsonArray notesArray;
        QSqlQuery query(db);
        query.prepare("SELECT note_id, title, update_time FROM notes WHERE user_id = :user_id ORDER BY update_time DESC");
        query.bindValue(":user_id", user_id);
        if (query.exec()) {
            while (query.next()) {
                QJsonObject noteObj;
                noteObj.insert("note_id", query.value(0).toInt());
                noteObj.insert("title", query.value(1).toString());
                noteObj.insert("update_time", query.value(2).toString());
                notesArray.append(noteObj);
            }
        }
        response.insert("status", "success");
        response.insert("notes", notesArray);
    }
    else if (action == "save_note") {
        int is_new = obj.value("is_new").toInt();
        int user_id = obj.value("user_id").toInt();
        QString title = obj.value("title").toString();
        QString content = obj.value("content").toString();
        QString current_time = obj.value("time").toString();

        QSqlQuery query(db);
        if (is_new == 1) {
            query.prepare("INSERT INTO notes (user_id, title, content, create_time, update_time) VALUES (:u, :t, :c, :ct, :ut)");
            query.bindValue(":u", user_id);
            query.bindValue(":t", title);
            query.bindValue(":c", content);
            query.bindValue(":ct", current_time);
            query.bindValue(":ut", current_time);
        } else {
            int note_id = obj.value("note_id").toInt();
            query.prepare("UPDATE notes SET title = :t, content = :c, update_time = :ut WHERE note_id = :n AND user_id = :u");
            query.bindValue(":t", title);
            query.bindValue(":c", content);
            query.bindValue(":ut", current_time);
            query.bindValue(":n", note_id);
            query.bindValue(":u", user_id);
        }

        if (query.exec()) {
            response.insert("status", "success");
        } else {
            response.insert("status", "error");
            response.insert("msg", query.lastError().text());
        }
    }
    else if (action == "load_note_content") {
        int note_id = obj.value("note_id").toInt();
        int user_id = obj.value("user_id").toInt();
        QSqlQuery query(db);
        query.prepare("SELECT content FROM notes WHERE note_id = :note_id AND user_id = :user_id");
        query.bindValue(":note_id", note_id);
        query.bindValue(":user_id", user_id);
        if (query.exec() && query.next()) {
            response.insert("status", "success");
            response.insert("content", query.value(0).toString());
        } else {
            response.insert("status", "error");
        }
    }
    else if (action == "delete_note") {
        int note_id = obj.value("note_id").toInt();
        int user_id = obj.value("user_id").toInt();
        QSqlQuery query(db);
        query.prepare("DELETE FROM notes WHERE note_id = :note_id AND user_id = :user_id");
        query.bindValue(":note_id", note_id);
        query.bindValue(":user_id", user_id);
        if (query.exec()) {
            response.insert("status", "success");
        } else {
            response.insert("status", "error");
        }
    }
    else if (action == "search_notes") {
        int user_id = obj.value("user_id").toInt();
        QString keyword = obj.value("keyword").toString();
        QJsonArray notesArray;
        QSqlQuery query(db);
        query.prepare("SELECT note_id, title, update_time FROM notes WHERE user_id = :user_id AND (title LIKE :keyword OR content LIKE :keyword) ORDER BY update_time DESC");
        query.bindValue(":user_id", user_id);
        query.bindValue(":keyword", "%" + keyword + "%");
        if (query.exec()) {
            while (query.next()) {
                QJsonObject noteObj;
                noteObj.insert("note_id", query.value(0).toInt());
                noteObj.insert("title", query.value(1).toString());
                noteObj.insert("update_time", query.value(2).toString());
                notesArray.append(noteObj);
            }
        }
        response.insert("status", "success");
        response.insert("notes", notesArray);
    }
    else {
        response.insert("status", "error");
        response.insert("msg", "未知的请求行为");
    }

    QJsonDocument retDoc(response);
    socket->write(retDoc.toJson());
}
