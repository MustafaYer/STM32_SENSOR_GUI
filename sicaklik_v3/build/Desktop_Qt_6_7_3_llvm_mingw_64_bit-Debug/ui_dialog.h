/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtQuickWidgets/QQuickWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QComboBox *BaurdRate;
    QLineEdit *Connection;
    QComboBox *ComPort;
    QPushButton *Disconnect;
    QPushButton *Connect;
    QQuickWidget *MyquickWidget;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(1100, 650);
        BaurdRate = new QComboBox(Dialog);
        BaurdRate->addItem(QString());
        BaurdRate->addItem(QString());
        BaurdRate->addItem(QString());
        BaurdRate->setObjectName("BaurdRate");
        BaurdRate->setGeometry(QRect(348, 20, 65, 24));
        QFont font;
        font.setBold(true);
        BaurdRate->setFont(font);
        Connection = new QLineEdit(Dialog);
        Connection->setObjectName("Connection");
        Connection->setGeometry(QRect(161, 20, 181, 24));
        Connection->setFont(font);
        ComPort = new QComboBox(Dialog);
        ComPort->addItem(QString());
        ComPort->addItem(QString());
        ComPort->addItem(QString());
        ComPort->addItem(QString());
        ComPort->addItem(QString());
        ComPort->addItem(QString());
        ComPort->addItem(QString());
        ComPort->addItem(QString());
        ComPort->addItem(QString());
        ComPort->addItem(QString());
        ComPort->setObjectName("ComPort");
        ComPort->setGeometry(QRect(419, 20, 65, 24));
        ComPort->setFont(font);
        Disconnect = new QPushButton(Dialog);
        Disconnect->setObjectName("Disconnect");
        Disconnect->setGeometry(QRect(561, 20, 65, 24));
        QFont font1;
        font1.setBold(true);
        font1.setUnderline(false);
        font1.setStrikeOut(false);
        font1.setKerning(true);
        Disconnect->setFont(font1);
        Disconnect->setMouseTracking(false);
        Connect = new QPushButton(Dialog);
        Connect->setObjectName("Connect");
        Connect->setGeometry(QRect(490, 20, 65, 24));
        Connect->setFont(font);
        MyquickWidget = new QQuickWidget(Dialog);
        MyquickWidget->setObjectName("MyquickWidget");
        MyquickWidget->setGeometry(QRect(10, 90, 275, 531));
        MyquickWidget->setResizeMode(QQuickWidget::ResizeMode::SizeRootObjectToView);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        BaurdRate->setItemText(0, QCoreApplication::translate("Dialog", "9600", nullptr));
        BaurdRate->setItemText(1, QCoreApplication::translate("Dialog", "57600", nullptr));
        BaurdRate->setItemText(2, QCoreApplication::translate("Dialog", "115200", nullptr));

        ComPort->setItemText(0, QCoreApplication::translate("Dialog", "COM1", nullptr));
        ComPort->setItemText(1, QCoreApplication::translate("Dialog", "COM2", nullptr));
        ComPort->setItemText(2, QCoreApplication::translate("Dialog", "COM3", nullptr));
        ComPort->setItemText(3, QCoreApplication::translate("Dialog", "COM4", nullptr));
        ComPort->setItemText(4, QCoreApplication::translate("Dialog", "COM5", nullptr));
        ComPort->setItemText(5, QCoreApplication::translate("Dialog", "COM6", nullptr));
        ComPort->setItemText(6, QCoreApplication::translate("Dialog", "COM7", nullptr));
        ComPort->setItemText(7, QCoreApplication::translate("Dialog", "COM8", nullptr));
        ComPort->setItemText(8, QCoreApplication::translate("Dialog", "COM9", nullptr));
        ComPort->setItemText(9, QCoreApplication::translate("Dialog", "COM10", nullptr));

        Disconnect->setText(QCoreApplication::translate("Dialog", "Disconnect", nullptr));
        Connect->setText(QCoreApplication::translate("Dialog", "Connect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
