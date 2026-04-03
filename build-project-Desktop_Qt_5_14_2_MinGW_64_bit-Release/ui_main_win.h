/********************************************************************************
** Form generated from reading UI file 'main_win.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WIN_H
#define UI_MAIN_WIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_main_win
{
public:
    QLineEdit *lineEdit;
    QPushButton *add_bt;
    QTableWidget *tableWidget;
    QPushButton *lock_bt;
    QPushButton *find_bt;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QLabel *welcome_lb;
    QTextEdit *textEdit;
    QLabel *label_2;
    QPushButton *del_bt;

    void setupUi(QWidget *main_win)
    {
        if (main_win->objectName().isEmpty())
            main_win->setObjectName(QString::fromUtf8("main_win"));
        main_win->resize(786, 434);
        main_win->setStyleSheet(QString::fromUtf8(""));
        lineEdit = new QLineEdit(main_win);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(20, 20, 161, 40));
        add_bt = new QPushButton(main_win);
        add_bt->setObjectName(QString::fromUtf8("add_bt"));
        add_bt->setGeometry(QRect(660, 320, 90, 90));
        add_bt->setMinimumSize(QSize(90, 90));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(72);
        add_bt->setFont(font);
        add_bt->setStyleSheet(QString::fromUtf8("color: rgb(255, 170, 0);\n"
"image: url(:/new/prefix1/image/save.png);"));
        add_bt->setIconSize(QSize(90, 90));
        tableWidget = new QTableWidget(main_win);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(20, 70, 161, 298));
        lock_bt = new QPushButton(main_win);
        lock_bt->setObjectName(QString::fromUtf8("lock_bt"));
        lock_bt->setGeometry(QRect(660, 90, 90, 90));
        lock_bt->setMinimumSize(QSize(90, 90));
        lock_bt->setStyleSheet(QString::fromUtf8("image: url(:/new/prefix1/image/lock.png);"));
        lock_bt->setIconSize(QSize(90, 90));
        find_bt = new QPushButton(main_win);
        find_bt->setObjectName(QString::fromUtf8("find_bt"));
        find_bt->setGeometry(QRect(140, 20, 40, 40));
        find_bt->setStyleSheet(QString::fromUtf8("image: url(:/new/prefix1/image/serach.png);\n"
""));
        layoutWidget = new QWidget(main_win);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(610, 10, 171, 51));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        layoutWidget->setFont(font1);
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font2.setPointSize(12);
        label->setFont(font2);

        horizontalLayout->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        welcome_lb = new QLabel(layoutWidget);
        welcome_lb->setObjectName(QString::fromUtf8("welcome_lb"));
        welcome_lb->setFont(font2);
        welcome_lb->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(welcome_lb);

        textEdit = new QTextEdit(main_win);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(190, 70, 431, 341));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Arial"));
        font3.setPointSize(12);
        textEdit->setFont(font3);
        label_2 = new QLabel(main_win);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(270, 30, 271, 31));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Arial"));
        font4.setPointSize(20);
        label_2->setFont(font4);
        del_bt = new QPushButton(main_win);
        del_bt->setObjectName(QString::fromUtf8("del_bt"));
        del_bt->setGeometry(QRect(20, 370, 161, 41));
        QFont font5;
        font5.setFamily(QString::fromUtf8("\345\255\227\351\255\202132\345\217\267-\346\261\237\350\210\237\350\241\214\345\256\242"));
        font5.setPointSize(18);
        del_bt->setFont(font5);

        retranslateUi(main_win);

        QMetaObject::connectSlotsByName(main_win);
    } // setupUi

    void retranslateUi(QWidget *main_win)
    {
        main_win->setWindowTitle(QCoreApplication::translate("main_win", "Form", nullptr));
        add_bt->setText(QString());
        lock_bt->setText(QString());
        find_bt->setText(QString());
        label->setText(QCoreApplication::translate("main_win", "\346\254\242\350\277\216\345\233\236\346\235\245", nullptr));
        welcome_lb->setText(QCoreApplication::translate("main_win", "user1\347\232\204\347\247\201\345\257\206\347\251\272\351\227\264", nullptr));
        textEdit->setPlaceholderText(QCoreApplication::translate("main_win", "\350\257\267\350\276\223\345\205\245\350\246\201\344\277\235\345\255\230\347\232\204\344\277\241\346\201\257", nullptr));
        label_2->setText(QCoreApplication::translate("main_win", "\350\257\267\350\276\223\345\205\245\346\202\250\350\246\201\344\277\235\345\255\230\347\232\204\344\277\241\346\201\257", nullptr));
        del_bt->setText(QCoreApplication::translate("main_win", "\345\210\240\351\231\244\344\276\277\347\255\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class main_win: public Ui_main_win {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WIN_H
