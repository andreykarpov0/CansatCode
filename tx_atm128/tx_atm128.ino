#include <SPI.h>          // библиотека для работы с шиной SPI
#include <Wire.h>
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10

Adafruit_BMP280 bme;


RF24 radio(PE3,PE2);
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};
byte counter = 0;
int ADXL345 = 0x53; // Адрес I2C датчика ADXL345
float X_out, Y_out, Z_out;  // Выходы
float mas[5];

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(0xAABBCCDD11);
  radio.setChannel(60);
  radio.setPALevel (RF24_PA_MAX); 
  radio.setDataRate (RF24_250KBPS);
  radio.powerUp();
  radio.stopListening();
  bme.begin();
  Wire.begin(); // Инициализация библиотеки Wire 
  Wire.beginTransmission(ADXL345); // Начать общение с устройством 
  Wire.write(8); // (8dec -> 0000 1000 двоичный) Бит D3 High для разрешения измерения
  Wire.endTransmission();
  delay(10);
  
}
void loop() {
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Начать с регистра 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Чтение всех 6 регистров, значение каждой оси сохраняется в 2 регистрах
  mas[0] = ( Wire.read()| Wire.read() << 8); // Значение по оси X
  mas[0] = mas[0]/256; //Для диапазона + -2g нам нужно разделить необработанные значения на 256 в соответствии с datasheet
  mas[1] = ( Wire.read()| Wire.read() << 8); // Значение по оси Y
  mas[1] = mas[1]/256;
  mas[2] = ( Wire.read()| Wire.read() << 8); // Значение по оси Z
  mas[2] = mas[2]/256;
  mas[3] = bme.readTemperature();
  mas[4] = bme.readPressure();
  radio.write(&mas, sizeof(mas));
  
  delay(1000);
}
