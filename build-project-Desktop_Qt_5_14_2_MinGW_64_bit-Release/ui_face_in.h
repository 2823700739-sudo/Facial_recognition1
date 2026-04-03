/********************************************************************************
** Form generated from reading UI file 'face_in.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACE_IN_H
#define UI_FACE_IN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_face_in
{
public:
    QWidget *widget;
    QLabel *label;

    void setupUi(QWidget *face_in)
    {
        if (face_in->objectName().isEmpty())
            face_in->setObjectName(QString::fromUtf8("face_in"));
        face_in->resize(617, 563);
        widget = new QWidget(face_in);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(60, 90, 481, 431));
        widget->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 85, 255);"));
        label = new QLabel(face_in);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(210, 40, 181, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(22);
        label->setFont(font);

        retranslateUi(face_in);

        QMetaObject::connectSlotsByName(face_in);
    } // setupUi

    void retranslateUi(QWidget *face_in)
    {
        face_in->setWindowTitle(QCoreApplication::translate("face_in", "Form", nullptr));
        label->setText(QCoreApplication::translate("face_in", "\344\272\272\350\204\270\350\257\206\345\210\253\347\231\273\345\205\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class face_in: public Ui_face_in {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACE_IN_H
