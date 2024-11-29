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

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(800, 600);
        BaurdRate = new QComboBox(Dialog);
        BaurdRate->addItem(QString());
        BaurdRate->addItem(QString());
        BaurdRate->addItem(QString());
        BaurdRate->setObjectName("BaurdRate");
        BaurdRate->setGeometry(QRect(298, 50, 65, 24));
        Connection = new QLineEdit(Dialog);
        Connection->setObjectName("Connection");
        Connection->setGeometry(QRect(131, 50, 161, 24));
        ComPort = new QComboBox(Dialog);
        ComPort->addItem(QString());
        ComPort->addItem(QString());
        ComPort->setObjectName("ComPort");
        ComPort->setGeometry(QRect(369, 50, 65, 24));
        Disconnect = new QPushButton(Dialog);
        Disconnect->setObjectName("Disconnect");
        Disconnect->setGeometry(QRect(511, 50, 65, 24));
        Connect = new QPushButton(Dialog);
        Connect->setObjectName("Connect");
        Connect->setGeometry(QRect(440, 50, 65, 24));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        BaurdRate->setItemText(0, QCoreApplication::translate("Dialog", "9600", nullptr));
        BaurdRate->setItemText(1, QCoreApplication::translate("Dialog", "57600", nullptr));
        BaurdRate->setItemText(2, QCoreApplication::translate("Dialog", "115200", nullptr));

        ComPort->setItemText(0, QCoreApplication::translate("Dialog", "COM6", nullptr));
        ComPort->setItemText(1, QCoreApplication::translate("Dialog", "COM7", nullptr));

        Disconnect->setText(QCoreApplication::translate("Dialog", "Disconnect", nullptr));
        Connect->setText(QCoreApplication::translate("Dialog", "Connect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
