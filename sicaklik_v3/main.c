#include <QApplication>
#include "dialog.h"

#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QTime>
#include <QTimer>

#include <QSerialPortInfo>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Dialog penceresini oluştur
    Dialog w;
    w.show();

    // Uygulamayı başlat
    return a.exec();
}
