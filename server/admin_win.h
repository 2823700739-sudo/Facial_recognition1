#ifndef ADMIN_WIN_H
#define ADMIN_WIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class admin_win; }
QT_END_NAMESPACE

class admin_win : public QMainWindow
{
    Q_OBJECT

public:
    admin_win(QWidget *parent = nullptr);
    ~admin_win();

private:
    Ui::admin_win *ui;
};
#endif // ADMIN_WIN_H
