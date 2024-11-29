/*
 * UART.c
 *
 *  Created on: Oct 13, 2024
 *      Author: mustafa
 */
#include "UART.h"
#include "stm32f446xx.h"
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "string.h"
extern UART_HandleTypeDef huart2;


void Uart2_Config(){

	//1. Enable the uart clock and gpio pin
	RCC->APB1ENR |= (1 << 17); // UART2
	RCC->AHB1ENR |= (1 << 0); // GPIOA

	//2. Configure the UART Pin alternate functions
	GPIOA->MODER |= (2 << 4); // PA2 Alternate function
	GPIOA->MODER |= (2 << 6); // PA3 Alternate function

	GPIOA->OSPEEDR |= (3 << 4) | (3 << 6); // high speed for PA2 and PA3

	GPIOA->AFR[0] |= (7 << 8); // AF7 alternate function PA2
	GPIOA->AFR[0] |= (7 << 12); // AF7 alternate function PA2

	//3. Enable the uart by writing the UE bit in USART_CR1 register to 1
	USART2->CR1 = 0x00;
	USART2->CR1 |= (1 << 13);

	//4. Program the M bit in USART_CR1 to define the word length
	USART2->CR1 &= ~(1 << 12); // M=0 8 bit

	//5. Select the desired baud rate USART_BRR
//	uint32_t pclk = 45000000;  // 45 MHz PCLK
//	uint32_t baud = 115200;    // 115200 baudrate
//
//	uint32_t usartdiv = (pclk + (baud / 2)) / baud;  // Baudrate hesaplama
//	USART2->BRR = usartdiv;  // Baudrate register'a yaz


	USART2->BRR = (9 << 0) | (33 << 4); // Baud rate 115200 PCLK=45Mhz

	//6. Enable the Transmit by set TE RE bit USART_CR1
	USART2->CR1 |= (1 << 2); // RE enabled
	USART2->CR1 |= (1 << 3); // TE enabled


}

void UART2_SendChar(uint8_t c){

    // Öncelikle veri gönderimi için DR'nin boş olup olmadığını kontrol et
    while(!(USART2->SR & (1 << 7)));  // TXE: Transmit data register empty

    // Veriyi DR'ye yaz
    USART2->DR = c;

    // Gönderim tamamlanmasını bekle
    while(!(USART2->SR & (1 << 6)));  // TC: Transmission complete
}

void UART2_SendString (char *string){
	while (*string) UART2_SendChar (*string++);
}

uint8_t UART2_GetChar (void){

	uint8_t temp;

	while (!(USART2->SR & (1<<5)));  // wait for RXNE bit to set
	temp = USART2->DR;  // Read the data. This clears the RXNE also
	return temp;
}

uint8_t CalculateCRC(uint8_t *data, uint16_t length) {

    uint8_t crc = 0;
    for (uint16_t i = 0; i < length; i++) {
        crc ^= data[i];
    }
    return crc;
}

void SendTemperaturePacket(uint8_t *data, uint16_t length) {

    const char *header = ":A42*";
    const char *footer = ";";  // Sonlandirici karakter

    uint8_t crc = CalculateCRC(data, length);

    char dataSizeStr[10];
    snprintf(dataSizeStr, sizeof(dataSizeStr), "%d", length); // Veri uzunlugunu yaz

    //uint16_t packetLength = strlen(header) + strlen(dataSizeStr) + 1 + length + 1 + sizeof(crc)+ strlen(footer);

    uint8_t packet[256];  // Statik olarak ayrilan buffer, maksimum paket boyutuna uygun
    memset(packet, 0, sizeof(packet)); // packet tamponunu temizle


    uint16_t index = 0;
    memcpy(&packet[index], header, strlen(header));  // Baslik ekle
    index += strlen(header);

    memcpy(&packet[index], dataSizeStr, strlen(dataSizeStr));  // Veri uzunlugunu ekle
    index += strlen(dataSizeStr);
    packet[index++] = '*';  // Ayirici karakter

    memcpy(&packet[index], data, length);  // Veriyi ekle
    index += length;

    packet[index++] = '*';  // CRC'yi ekle


    memcpy(&packet[index], &crc, 1);
    index += 1;

    memcpy(&packet[index], footer, strlen(footer));  // Sonlandirici karakter ekle
    index += strlen(footer);

    UART2_SendString( (char*)packet);  // Paketi gönder
}

void SendHumidityPacket(uint8_t *data, uint16_t length) {

    const char *header = ":B42*";
    const char *footer = ";";  // Sonlandırıcı karakter

    uint8_t crc = CalculateCRC(data, length);

    char dataSizeStr[10];
    snprintf(dataSizeStr, sizeof(dataSizeStr), "%d", length); // Veri uzunluğunu yaz

    // Statik olarak ayrılan buffer, maksimum paket boyutuna uygun
    uint8_t packet[256];
    memset(packet, 0, sizeof(packet)); // packet tamponunu temizle
    uint16_t index = 0;

    // Başlık ekle
    memcpy(&packet[index], header, strlen(header));
    index += strlen(header);

    // Veri uzunluğunu ekle
    memcpy(&packet[index], dataSizeStr, strlen(dataSizeStr));
    index += strlen(dataSizeStr);

    // Ayırıcı karakter ekle
    packet[index++] = '*';

    // Veriyi ekle
    memcpy(&packet[index], data, length);
    index += length;

    // Ayırıcı karakter ve CRC ekle
    packet[index++] = '*';
    memcpy(&packet[index], &crc, 1);
    index += 1;

    // Sonlandırıcı karakteri ekle
    memcpy(&packet[index], footer, strlen(footer));
    index += strlen(footer);

    // Paketi gönder
    UART2_SendString((char*)packet);
}

