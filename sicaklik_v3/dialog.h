#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "qcustomplot.h"  // QCustomPlot'u ekleyin
#include <QSerialPort>
#include <QByteArray>
#include <QVBoxLayout>
#include <QTimer>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(double temperatureValue READ temperatureValue WRITE setTemperatureValue NOTIFY temperatureValueChanged)
    Q_PROPERTY(double humidityValue READ humidityValue WRITE setHumidityValue NOTIFY humidityValueChanged)

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    double temperatureValue() const;
    void setTemperatureValue(double value);

    double humidityValue() const;
    void setHumidityValue(double value);


signals:
    void temperatureValueChanged();
    void humidityValueChanged();


private slots:
    void readSerial();
    void updateGraph(); // Grafik güncelleme fonksiyonu
    void on_Connect_clicked();
    void on_Disconnect_clicked();
    void checkCustomBaurdatepolity(int idx);
    void checkConnection();

private:
    Ui::Dialog *ui;
    QCustomPlot *customPlot1 = new QCustomPlot();
    QCustomPlot *customPlot2 = new QCustomPlot();

    QSerialPort *stm32; // Serial port nesnesi
    QByteArray serialData;
    QByteArray serialBuffer;
    QString parsed_data;
    double temperature_value = 0;
    double humidity_value = 0;
    QTimer *timer;

    bool isConnected;
    bool firstUpdate = true;

    void setupQML(); // QML ayarlarını başlatma fonksiyonu
    void setupSerialPort(); // Serial port ayarları için fonksiyon

    void processTempData(QByteArray &data);
    void processHumidityData(QByteArray &data);
    uint8_t CalculateCRC(uint8_t *data, uint16_t length);
    void ReadData();

    void updateHumidity(QString sensor_reading);
    void updateTemperature(QString sensor_reading);
    //double temperature() const;
    double humidity() const;

    double m_temperatureValue = 0;
    double m_humidityValue = 0;

    qint64 lastReceivedTime;

};
#endif // DIALOG_H
