#include "login.h"

#include <QApplication>
#include <QApplication>
// 可选：添加调试头文件，验证资源是否存在
#include <QFile>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login w;
    // 资源路径完全正确（与.qrc前缀、文件路径1:1匹配）
    a.setWindowIcon(QIcon(":/new/prefix1/image/logo.ico"));
    // 调试代码：运行后可在控制台验证资源是否加载成功
    QFile iconFile(":/new/prefix1/image/logo.ico");
    if(iconFile.exists())
    {
        qDebug() << "图标资源加载成功！路径：" << iconFile.fileName();
    }else
    {
        qDebug() << "图标资源不存在！请检查.qrc前缀和路径！";
    }
    w.show();
    return a.exec();
}
