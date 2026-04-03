#ifndef ADMIN_WIN_H
#define ADMIN_WIN_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class admin_win; }
QT_END_NAMESPACE

class admin_win : public QMainWindow
{
    Q_OBJECT

public:
    admin_win(QWidget *parent = nullptr);
    ~admin_win();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    Ui::admin_win *ui;
    QTcpServer *tcpServer;
    QSqlDatabase db;
    QMap<QTcpSocket*, QString> clientInfos; // 保存当前连接的客户端信息

    // 辅助函数
    void initDatabase();
    void logMessage(const QString &msg);
    void updateClientTable();
    void processClientRequest(QTcpSocket* socket, const QByteArray& data);
};

#endif // ADMIN_WIN_H
