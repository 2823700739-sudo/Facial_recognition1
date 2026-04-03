/********************************************************************************
** Form generated from reading UI file 'admin_win.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMIN_WIN_H
#define UI_ADMIN_WIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_admin_win
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTableWidget *clientTable;
    QLabel *label_2;
    QTextEdit *logTextEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *admin_win)
    {
        if (admin_win->objectName().isEmpty())
            admin_win->setObjectName(QString::fromUtf8("admin_win"));
        admin_win->resize(800, 600);
        centralwidget = new QWidget(admin_win);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        clientTable = new QTableWidget(centralwidget);
        if (clientTable->columnCount() < 3)
            clientTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        clientTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        clientTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        clientTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        clientTable->setObjectName(QString::fromUtf8("clientTable"));

        verticalLayout->addWidget(clientTable);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        logTextEdit = new QTextEdit(centralwidget);
        logTextEdit->setObjectName(QString::fromUtf8("logTextEdit"));
        logTextEdit->setReadOnly(true);

        verticalLayout->addWidget(logTextEdit);

        admin_win->setCentralWidget(centralwidget);
        menubar = new QMenuBar(admin_win);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        admin_win->setMenuBar(menubar);
        statusbar = new QStatusBar(admin_win);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        admin_win->setStatusBar(statusbar);

        retranslateUi(admin_win);

        QMetaObject::connectSlotsByName(admin_win);
    } // setupUi

    void retranslateUi(QMainWindow *admin_win)
    {
        admin_win->setWindowTitle(QCoreApplication::translate("admin_win", "admin_win", nullptr));
        label->setText(QCoreApplication::translate("admin_win", "\345\267\262\350\277\236\346\216\245\347\232\204\345\256\242\346\210\267\347\253\257\357\274\232", nullptr));
        QTableWidgetItem *___qtablewidgetitem = clientTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("admin_win", "IP \345\234\260\345\235\200", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = clientTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("admin_win", "\347\253\257\345\217\243", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = clientTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("admin_win", "\347\212\266\346\200\201", nullptr));
        label_2->setText(QCoreApplication::translate("admin_win", "\346\234\215\345\212\241\345\231\250\350\277\220\350\241\214\346\227\245\345\277\227\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class admin_win: public Ui_admin_win {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMIN_WIN_H
