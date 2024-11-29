#include <QApplication>
#include "dialog.h"
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QSerialPortInfo>
#include <QQuickWidget>
#include <QVBoxLayout>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Dialog dialog;

    dialog.show();

    QQmlApplicationEngine engine;
    return a.exec();
}


