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
    explicit main_win(QWidget *parent = nullptr, int user_id = -1, QString username = "");
    ~main_win();

private slots:
    void on_add_bt_clicked();

    void on_lock_bt_clicked();

    void on_find_bt_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_del_bt_clicked();

private:
    Ui::main_win *ui;
    QSqlDatabase db;//创建数据库对象
    int current_user_id;
    QString current_username;
    int current_edit_note_id;

    void loadUserNotes();
    
    // 锁定蒙版指针
    class QWidget *maskWidget = nullptr;
    // 解锁按钮槽函数
    void on_unlock_clicked();
};

#endif // MAIN_WIN_H
