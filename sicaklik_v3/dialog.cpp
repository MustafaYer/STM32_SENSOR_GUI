#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QQmlContext>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    customPlot1(new QCustomPlot(this)),
    customPlot2(new QCustomPlot(this)),

    isConnected(false)
{
    ui->setupUi(this);

    QTimer *connectionTimer = new QTimer(this);
    connect(connectionTimer, &QTimer::timeout, this, &Dialog::checkConnection);

    // Timer'ı başlat, 1 saniyede bir çalışacak şekilde ayarla
    connectionTimer->start(1000); // 1000 ms = 1 saniye

    // Dialog'un arka planı koyu gri
    this->setStyleSheet("background-color: #2f2f2f;");

    ui->MyquickWidget->rootContext()->setContextProperty("temperature_value", this); // temperature_value'nin qml'de kullanılmasını sağlar
    ui->MyquickWidget->rootContext()->setContextProperty("humidity_value", this); // humidity_value'nin qml'de kullanılmasını sağlar



    /* Sıcaklık Grafiği */
    customPlot1->addGraph();
    customPlot1->graph(0)->setPen(QPen(Qt::blue));

    customPlot1->graph(0)->setName("Sıcaklık");
    customPlot1->setFixedSize(800, 250);

    customPlot1->xAxis->setLabel("Zaman");
    customPlot1->yAxis->setLabel("Sıcaklık (°C)");

    customPlot1->xAxis->setLabelColor(Qt::white);   // X ekseni etiketi
    customPlot1->yAxis->setLabelColor(Qt::white);   // Y ekseni etiketi

    customPlot1->xAxis->setTickLabelColor(Qt::white); // X ekseni saat yazıları
    customPlot1->yAxis->setTickLabelColor(Qt::white); // Y ekseni yazıları

    //arka plan rengi
    customPlot1->setBackground(QBrush(QColor(42, 42, 42))); // Açık gri

    // X-axis kılavuz çizgilerini beyaz yapma
    customPlot1->xAxis->grid()->setPen(QPen(QColor(0, 0, 0))); // Beyaz

    // Y-axis kılavuz çizgilerini beyaz yapma
    customPlot1->yAxis->grid()->setPen(QPen(QColor(0, 0, 0))); // Beyaz



    /* Nem Grafiği */
    customPlot2->addGraph();
    customPlot2->setFixedSize(800, 250);
    customPlot2->graph(0)->setPen(QPen(Qt::green));
    customPlot2->graph(0)->setName("Nem");

    customPlot2->xAxis->setLabel("Zaman");
    customPlot2->yAxis->setLabel("Nem (%)");

    customPlot2->xAxis->setLabelColor(Qt::white);     // X ekseni etiketi
    customPlot2->yAxis->setLabelColor(Qt::white);

    customPlot2->xAxis->setTickLabelColor(Qt::white); // X ekseni saat yazıları
    customPlot2->yAxis->setTickLabelColor(Qt::white); // Y ekseni yazıları

    //arka plan rengi
    customPlot2->setBackground(QBrush(QColor(42, 42, 42))); // Açık gri

    // X-axis kılavuz çizgilerini beyaz yapma
    customPlot2->xAxis->grid()->setPen(QPen(QColor(0, 0, 0)));

    // Y-axis kılavuz çizgilerini beyaz yapma
    customPlot2->yAxis->grid()->setPen(QPen(QColor(0, 0, 0)));


    // QML dosyasını mevcut MyquickWidget'a yükleyin
    ui->MyquickWidget->setSource(QUrl(QStringLiteral("qrc:/new/prefix1/Main.qml"))); // QML dosyanızın yolu
    ui->MyquickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    // Kombobox'ın yazı rengini değiştirmek için
    ui->ComPort->setStyleSheet("QComboBox { color: white; background-color: black; }"
                               "QComboBox:pressed { color: black; background-color: gray; }");
    ui->BaurdRate->setStyleSheet("QComboBox { color: white; background-color: black; }"
                                 "QComboBox:pressed { color: black; background-color: gray; }");
    ui->Connect->setStyleSheet("QPushButton { color: white; background-color: black; }"
                               "QPushButton:pressed { color: black; background-color: gray; }");
    ui->Disconnect->setStyleSheet("QPushButton { color: white; background-color: black; }"
                                  "QPushButton:pressed { color: black; background-color: gray; }");
    ui->Connection->setStyleSheet("QLineEdit { color: white; background-color: black; }");

    // Layout'u oluşturun ve widget'ları ekleyin
    QVBoxLayout *mainLayout = new QVBoxLayout(this);


    //Grafik Konum
    mainLayout->setContentsMargins(290, 50, 0, 0); // (left, top, right, bottom)

    mainLayout->addWidget(customPlot1);
    mainLayout->addWidget(customPlot2);

    setLayout(mainLayout);

    // Veri güncellemeleri için zamanlayıcı ayarla
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Dialog::updateGraph);
    timer->start(100); // Her 100 ms'de bir güncelle
}

Dialog::~Dialog() {
    stm32->close(); // Serial portu kapat
    delete ui;
}

double Dialog::temperatureValue() const {
    return m_temperatureValue;
}

void Dialog::setTemperatureValue(double value) {
    if (m_temperatureValue != value) {
        m_temperatureValue = value;
        emit temperatureValueChanged();
    }
}

double Dialog::humidityValue() const {
    return m_humidityValue;
}

void Dialog::setHumidityValue(double value) {
    if (m_humidityValue != value) {
        m_humidityValue = value;
        emit humidityValueChanged();
    }
}

void Dialog::readSerial() {

    serialBuffer.append(stm32->readAll());
    if (serialBuffer.contains(';')) {  // Paketin tamamı geldiğinde (paket ; ile bitiyor)

        // Paketi işlemeye
        processTempData(serialBuffer);
        serialBuffer.clear();  // İşlenen veriyi temizliyoruz
        lastReceivedTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    }
}

void Dialog::checkConnection() {
    if ((QDateTime::currentDateTime().toMSecsSinceEpoch() - lastReceivedTime) > 2000) {
        isConnected = false; // Bağlantı kesildi
        qDebug() << "Connection lost!";
        ui->Connection->setText("Connection lost!");
    }
}

void Dialog::processTempData(QByteArray &data) {
    // Paket başlangıcının ":A42*" ile başladığını kontrol ediyoruz
    if (data.startsWith(":A42*")) {
        int endPos = data.indexOf(';');  // Paket sonlandırıcısını buluyoruz
        if (endPos != -1) {
            QByteArray content = data.mid(5, endPos - 5);  // Başlangıçtaki ":A42*" kısmını atlıyoruz
            int dataSizePos = content.indexOf('*');
            if(dataSizePos != -1){
                QByteArray restContent = content.mid(dataSizePos + 1);  // Geri kalan içerik (data ve crc)
                //qDebug() << "dataSizeStr" << dataSizeStr;
                //qDebug() << "restContent" << restContent;

                // CRC ve veri kısmını ayırıyoruz (sondaki '*' karakterine göre)
                int crcPos = restContent.lastIndexOf('*');
                if (crcPos != -1) {
                    QByteArray crcStr = restContent.mid(crcPos + 1);  // CRC kısmı
                    QByteArray actualData = restContent.left(crcPos);  // Asıl veri kısmı (data size + data)

                    uint8_t receivedCRC = static_cast<uint8_t>(crcStr[0]);
                    //qDebug() << "Received CRC: " << receivedCRC;

                    // CRC kontrolü
                    uint8_t calculatedCRC = CalculateCRC((uint8_t*)actualData.data(), actualData.size());
                    //qDebug() << /*"Received CRC: " << receivedCRC <<*/ "Calculated CRC: " << calculatedCRC;
                    qDebug() << "actualData.data(): " << actualData.data();
                    qDebug() << "actualData.size(): " << actualData.size();
                    qDebug() << "received temp crc" << receivedCRC;
                    qDebug() << "calculated temp crc" << calculatedCRC;

                    if (receivedCRC == calculatedCRC) {
                        updateTemperature(actualData);  // Sıcaklık verisini güncelle
                    } else {

                        qDebug() << "Temperature CRC check failed!";
                    }
                }
            }
        }
    }else if(data.startsWith(":B42*")){
        processHumidityData(data);
    }
}

void Dialog::processHumidityData(QByteArray &data) {
    int endPos = data.indexOf(';');  // Paket sonlandırıcısını buluyoruz
    if (endPos != -1) {
        QByteArray content = data.mid(5, endPos - 5);  // Başlangıçtaki ":B42*" kısmını atlıyoruz

        int dataSizePos = content.indexOf('*');
        if(dataSizePos != -1){
            QByteArray restContent = content.mid(dataSizePos + 1);  // Geri kalan içerik (data ve crc)

            // CRC ve veri kısmını ayırıyoruz
            int crcPos = restContent.lastIndexOf('*');
            if (crcPos != -1) {
                QByteArray crcStr = restContent.mid(crcPos + 1);  // CRC kısmı
                QByteArray actualData = restContent.left(crcPos);  // Asıl veri kısmı

                // CRC kontrolü
                uint8_t receivedCRC = static_cast<uint8_t>(crcStr[0]);
                uint8_t calculatedCRC = CalculateCRC((uint8_t*)actualData.data(), actualData.size());
                qDebug() << "received hum crc" << receivedCRC;


                if (receivedCRC == calculatedCRC) {
                    updateHumidity(actualData);  // Nem verisini güncelle
                } else {
                    isConnected = false;
                    qDebug() << "Nem CRC check failed!";
                }
            }
        }
    }
}

uint8_t Dialog::CalculateCRC(uint8_t *data, uint16_t length) {
    uint8_t crc = 0;
    for (uint16_t i = 0; i < length; i++) {
        crc ^= data[i];  // XOR tabanlı CRC
    }
    //qDebug() << "CRC " << crc;
    return crc;
}

void Dialog::updateHumidity(QString sensor_reading) {
    bool ok;
    double humidity = sensor_reading.toDouble(&ok);  // Veriyi double'a çevir

    if (ok) {
        humidity_value = humidity;  // Nem değerini güncelle

        setHumidityValue(humidity_value);
        if(humidity_value > 100){
            qDebug() << "Nem çok yüksek: " << sensor_reading;
            return;
        }
        qDebug() << "Güncellenen nem değeri: " << humidity_value;
    } else {
        qDebug() << "Veri parsing hatası: " << sensor_reading;  // Hata durumunda mesaj ver
    }
}

void Dialog::updateTemperature(QString sensor_reading) {
    bool ok;
    double temperature = sensor_reading.toDouble(&ok);  // Veriyi double'a çevir

    if (ok) {
        temperature_value = temperature;  // Sıcaklık değerini güncelle

        setTemperatureValue(temperature_value);

        if(temperature_value > 100){
            qDebug() << "Sıcaklık çok yüksek: " << sensor_reading;
            return;
        }
        qDebug() << "Güncellenen sıcaklık değeri: " << temperature_value;
    } else {
        qDebug() << "Veri parsing hatası: " << sensor_reading;  // Hata durumunda mesaj ver
    }
}

void Dialog::updateGraph() {
    if (!isConnected) return;

    QDateTime currentTime = QDateTime::currentDateTime();
    double currentTimeInSeconds = currentTime.toMSecsSinceEpoch() / 1000.0;

    // Sıcaklık verisi ekle
    customPlot1->graph(0)->addData(currentTimeInSeconds, temperature_value);

    // Nem verisi ekle
    customPlot2->graph(0)->addData(currentTimeInSeconds, humidity_value);

    // Zaman ekseni formatı ayarla (sıcaklık ve nem grafikleri için)
    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    dateTimeTicker->setDateTimeFormat("hh:mm:ss");
    customPlot1->xAxis->setTicker(dateTimeTicker);
    customPlot2->xAxis->setTicker(dateTimeTicker);

    // Eğer ilk güncelleme ise başlangıç aralığını ayarla
    if (firstUpdate) {
        customPlot1->xAxis->setRange(currentTimeInSeconds, currentTimeInSeconds + 120);
        customPlot2->xAxis->setRange(currentTimeInSeconds, currentTimeInSeconds + 120);
        firstUpdate = false;
    } else {
        // Sağ kenara ulaşıldığında ekseni kaydır
        double currentRangeLower = customPlot1->xAxis->range().lower;
        double currentRangeUpper = customPlot1->xAxis->range().upper;

        if (currentTimeInSeconds > currentRangeUpper) {
            // X ekseni aralığını kaydır
            customPlot1->xAxis->setRange(currentRangeLower + 1, currentRangeUpper + 1);  // 1 saniye sağa kaydır
            customPlot2->xAxis->setRange(currentRangeLower + 1, currentRangeUpper + 1);
        }
    }

    // Y eksen aralıklarını ayarla (Sıcaklık grafiği)
    customPlot1->yAxis->setRange(0, 60);  // Sıcaklık için uygun aralık
    customPlot1->graph(0)->setBrush(QBrush(QColor(107, 76, 54, 150))); // Asker kahvesi rengi
    customPlot1->graph(0)->setPen(QPen(QColor(255, 165, 0))); // Turuncu renk

    customPlot1->yAxis->setTickLabelColor(Qt::white);  // Y eksenindeki sayılar
    customPlot1->yAxis->setLabelColor(Qt::white);

    // Y eksen aralıklarını ayarla (Nem grafiği)
    customPlot2->yAxis->setRange(0, 60);  // Nem için uygun aralık
    customPlot2->graph(0)->setBrush(QBrush(QColor(0, 255, 0, 150)));  // Yeşil rengi
    customPlot2->graph(0)->setPen(QPen(QColor(255, 165, 0))); // Turuncu renk

    // Sıcaklık ve nem grafiğini yeniden çiz
    customPlot1->replot();
    customPlot2->replot();
}

void Dialog::on_Connect_clicked(){
    stm32 = new QSerialPort(this);
    stm32->setPortName(ui->ComPort->currentText());
    stm32->setBaudRate(ui->BaurdRate->currentText().toInt());
    stm32->setDataBits(QSerialPort::Data8);
    stm32->setParity(QSerialPort::NoParity);
    stm32->setStopBits(QSerialPort::OneStop);
    stm32->setFlowControl(QSerialPort::NoFlowControl);

    if(stm32->open(QIODevice::ReadWrite)){
        isConnected = true;
        qDebug()<< "Serial Port Connected";
        ui->Connection->setText("Serial Port is Connected");
        connect(stm32, &QSerialPort::readyRead, this, &Dialog::readSerial);
    } else {
        ui->Connection->setText("Serial Port is not Connected");
        qDebug()<< "Serial Port is not Connected " ;
    }
}

void Dialog::on_Disconnect_clicked(){
    if(stm32->isOpen()){
        stm32->close();
        isConnected = false; // Bağlantı kesildi
        ui->Connection->setText("Disconnected");
    }
}

void Dialog::checkCustomBaurdatepolity(int idx){
    const bool isCustomBaudRate = !ui->BaurdRate->itemData(idx).isValid();
    ui->BaurdRate->setEditable(isCustomBaudRate);
    if(isCustomBaudRate){
        ui->BaurdRate->clearEditText();
        QLineEdit *edit = ui->BaurdRate->lineEdit();
        QIntValidator *validator = new QIntValidator(0,400000, this);
        edit->setValidator(validator);
    }
}

