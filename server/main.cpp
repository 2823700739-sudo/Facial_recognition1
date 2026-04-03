#include "admin_win.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    admin_win w;
    w.show();
    return a.exec();
}
