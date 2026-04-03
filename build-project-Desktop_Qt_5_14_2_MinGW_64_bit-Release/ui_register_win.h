/********************************************************************************
** Form generated from reading UI file 'register_win.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER_WIN_H
#define UI_REGISTER_WIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_register_win
{
public:
    QWidget *widget;
    QLabel *label;
    QComboBox *comboBox;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_4;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QLineEdit *lineEdit;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_5;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *start_bt;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *stop_bt;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *takephoto_bt;

    void setupUi(QWidget *register_win)
    {
        if (register_win->objectName().isEmpty())
            register_win->setObjectName(QString::fromUtf8("register_win"));
        register_win->resize(763, 461);
        widget = new QWidget(register_win);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(20, 90, 401, 291));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(14);
        widget->setFont(font);
        widget->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(85, 0, 255);"));
        label = new QLabel(register_win);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(160, 20, 141, 21));
        label->setFont(font);
        comboBox = new QComboBox(register_win);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(20, 50, 401, 31));
        layoutWidget = new QWidget(register_win);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(430, 50, 301, 371));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalSpacer_4 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(140, 30));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(250, 40));
        lineEdit->setFont(font);

        verticalLayout->addWidget(lineEdit);

        verticalSpacer = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        verticalLayout_2->addLayout(verticalLayout);


        verticalLayout_3->addLayout(verticalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(160, 50));
        pushButton->setFont(font);

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_4->addLayout(verticalLayout_3);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_5);


        horizontalLayout_2->addLayout(verticalLayout_4);

        layoutWidget1 = new QWidget(register_win);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(20, 390, 401, 32));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        start_bt = new QPushButton(layoutWidget1);
        start_bt->setObjectName(QString::fromUtf8("start_bt"));
        start_bt->setMinimumSize(QSize(120, 30));
        start_bt->setFont(font);

        horizontalLayout_3->addWidget(start_bt);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        stop_bt = new QPushButton(layoutWidget1);
        stop_bt->setObjectName(QString::fromUtf8("stop_bt"));
        stop_bt->setMinimumSize(QSize(120, 30));
        stop_bt->setFont(font);

        horizontalLayout_3->addWidget(stop_bt);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        takephoto_bt = new QPushButton(layoutWidget1);
        takephoto_bt->setObjectName(QString::fromUtf8("takephoto_bt"));
        takephoto_bt->setMinimumSize(QSize(120, 30));
        takephoto_bt->setFont(font);

        horizontalLayout_3->addWidget(takephoto_bt);


        retranslateUi(register_win);

        QMetaObject::connectSlotsByName(register_win);
    } // setupUi

    void retranslateUi(QWidget *register_win)
    {
        register_win->setWindowTitle(QCoreApplication::translate("register_win", "Form", nullptr));
        label->setText(QCoreApplication::translate("register_win", "\350\257\267\345\275\225\345\205\245\344\272\272\350\204\270\344\277\241\346\201\257", nullptr));
        label_2->setText(QCoreApplication::translate("register_win", "\350\257\267\350\276\223\345\205\245\346\263\250\345\206\214\344\277\241\346\201\257", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("register_win", "\350\257\267\350\276\223\345\205\245\350\264\246\345\217\267", nullptr));
        pushButton->setText(QCoreApplication::translate("register_win", "\347\241\256\350\256\244\346\263\250\345\206\214", nullptr));
        start_bt->setText(QCoreApplication::translate("register_win", "\345\220\257\345\212\250\346\221\204\345\203\217\345\244\264", nullptr));
        stop_bt->setText(QCoreApplication::translate("register_win", "\345\205\263\351\227\255\346\221\204\345\203\217\345\244\264", nullptr));
        takephoto_bt->setText(QCoreApplication::translate("register_win", "\346\213\215\347\205\247", nullptr));
    } // retranslateUi

};

namespace Ui {
    class register_win: public Ui_register_win {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_WIN_H
