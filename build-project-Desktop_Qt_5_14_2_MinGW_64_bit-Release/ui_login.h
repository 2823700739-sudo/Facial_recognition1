/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QWidget *centralwidget;
    QLabel *text_lb;
    QPushButton *face_bt;
    QPushButton *register_bt;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QString::fromUtf8("login"));
        login->resize(330, 565);
        login->setMinimumSize(QSize(330, 565));
        login->setMaximumSize(QSize(330, 565));
        login->setStyleSheet(QString::fromUtf8("background-image: url(:/new/prefix1/E:/doubao_download/background_image.png);"));
        centralwidget = new QWidget(login);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        text_lb = new QLabel(centralwidget);
        text_lb->setObjectName(QString::fromUtf8("text_lb"));
        text_lb->setGeometry(QRect(100, 30, 141, 71));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(48);
        text_lb->setFont(font);
        text_lb->setStyleSheet(QString::fromUtf8("color: rgb(85, 170, 255);\n"
"border-radius:12px;\n"
"background-color: transparent;\n"
"border: none;\n"
""));
        text_lb->setAlignment(Qt::AlignCenter);
        face_bt = new QPushButton(centralwidget);
        face_bt->setObjectName(QString::fromUtf8("face_bt"));
        face_bt->setGeometry(QRect(90, 370, 150, 50));
        face_bt->setMinimumSize(QSize(150, 50));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(16);
        face_bt->setFont(font1);
        face_bt->setStyleSheet(QString::fromUtf8("background-color:rgb(85, 255, 255);"));
        register_bt = new QPushButton(centralwidget);
        register_bt->setObjectName(QString::fromUtf8("register_bt"));
        register_bt->setGeometry(QRect(90, 440, 150, 50));
        register_bt->setMinimumSize(QSize(150, 50));
        register_bt->setFont(font1);
        register_bt->setStyleSheet(QString::fromUtf8("background-color:rgb(85, 255, 255);"));
        login->setCentralWidget(centralwidget);
        menubar = new QMenuBar(login);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 330, 23));
        login->setMenuBar(menubar);
        statusbar = new QStatusBar(login);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        login->setStatusBar(statusbar);

        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QMainWindow *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "login", nullptr));
        text_lb->setText(QCoreApplication::translate("login", "\347\236\263\350\256\260", nullptr));
        face_bt->setText(QCoreApplication::translate("login", "\344\272\272\350\204\270\350\257\206\345\210\253", nullptr));
        register_bt->setText(QCoreApplication::translate("login", "\346\263\250\345\206\214\350\264\246\345\217\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
