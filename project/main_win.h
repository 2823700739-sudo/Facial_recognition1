#ifndef MAIN_WIN_H
#define MAIN_WIN_H

#include <QWidget>
#include <QDebug>
//数据库相关头文件
#include <QSqlDatabase>
#include <QSqlQuery>
namespace Ui {
class main_win;
}

class main_win : public QWidget
{
    Q_OBJECT

public:
    explicit main_win(QWidget *parent = nullptr, int user_id = -1, QString username = "");// 构造函数增加了 user_id 和 username 参数以便显示欢迎语和后续网络请求使用
    ~main_win();

private slots:
    void on_add_bt_clicked();

    void on_lock_bt_clicked();

    void on_find_bt_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);// 双击左侧表格加载对应的日记内容

    void on_del_bt_clicked();

private:
    Ui::main_win *ui;
    QSqlDatabase db;//创建数据库对象
    int current_user_id;// 当前登录用户的 ID
    QString current_username;// 当前登录用户的用户名（方便显示欢迎语和调试）
    int current_edit_note_id;// 当前正在编辑的便签 ID，-1 代表新建模式，否则代表编辑已有便签

    void loadUserNotes();
    
    // 锁定蒙版指针
    class QWidget *maskWidget = nullptr;
    // 解锁按钮槽函数
    void on_unlock_clicked();
};

#endif // MAIN_WIN_H
