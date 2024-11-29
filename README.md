# STM32_Sensor_Gui
A GUI application for monitoring sensor data using STM32 and Qt. Visualizes real-time temperature and humidity values with dynamic graphics.


# Project Description

This project reads temperature and humidity data from the DHT11 sensor and displays the data on a GUI built with the Qt Framework. The communication between the microcontroller and the Qt GUI is performed using the UART protocol.

### Features
- **CRC Validation:** UART communication includes CRC checks to prevent errors in data transmission.  
- **Frame Control:** A frame structure has been implemented for UART communication:
  - **Temperature Data Frame:** `:A42*data size*data*crc;`  
  - **Humidity Data Frame:** `:B42*data size*data*crc;`

Additionally, the data is saved to an SD card as a text file at 1-second intervals. All operations are managed using an RTOS (Real-Time Operating System) to ensure efficient task handling and real-time performance.

The graphical representation of the data is implemented using the **QCustomPlot** library for plotting. A dynamic gauge for real-time value display has been created using **QML**.

## Hardware Information

- **MCU:** STM32F446RE  
- **Sensor:** DHT11
- **SD Cart Module With SPI Protocol** 




## QT GUI
![WhatsApp Görsel 2024-11-29 saat 22 26 22_e541d47a](https://github.com/user-attachments/assets/5f616a00-7cfd-4e61-8187-83e38c021db8)

## SD Cart
![WhatsApp Görsel 2024-11-29 saat 22 27 16_803fce37](https://github.com/user-attachments/assets/2701916a-1d05-46b4-80dc-2bdfb5c53f24)
