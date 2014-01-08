/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created: Wed Nov 21 01:16:22 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *lblOriginal;
    QLabel *lblProcessed;
    QPushButton *btnPauseOrResume;
    QPlainTextEdit *txtXYRadius;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(1300, 550);
        lblOriginal = new QLabel(Dialog);
        lblOriginal->setObjectName(QString::fromUtf8("lblOriginal"));
        lblOriginal->setGeometry(QRect(10, 10, 640, 480));
        lblOriginal->setAutoFillBackground(true);
        lblProcessed = new QLabel(Dialog);
        lblProcessed->setObjectName(QString::fromUtf8("lblProcessed"));
        lblProcessed->setGeometry(QRect(670, 10, 640, 480));
        lblProcessed->setBaseSize(QSize(640, 480));
        lblProcessed->setAutoFillBackground(true);
        btnPauseOrResume = new QPushButton(Dialog);
        btnPauseOrResume->setObjectName(QString::fromUtf8("btnPauseOrResume"));
        btnPauseOrResume->setGeometry(QRect(8, 500, 137, 41));
        QFont font;
        font.setPointSize(12);
        btnPauseOrResume->setFont(font);
        txtXYRadius = new QPlainTextEdit(Dialog);
        txtXYRadius->setObjectName(QString::fromUtf8("txtXYRadius"));
        txtXYRadius->setGeometry(QRect(668, 500, 625, 41));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Courier New"));
        font1.setPointSize(12);
        txtXYRadius->setFont(font1);
        txtXYRadius->setReadOnly(true);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        lblOriginal->setText(QApplication::translate("Dialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        lblProcessed->setText(QApplication::translate("Dialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        btnPauseOrResume->setText(QApplication::translate("Dialog", "Pause", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
