#include <SPI.h>
#include <Wire.h>

// ---------- SPI Setup ----------
#define SPI_SS1 5
#define SPI_SS2 18
#define SPI_LED1 2  // LED to indicate SPI Slave1 received
#define SPI_LED2 4  // LED to indicate SPI Slave2 received

// ---------- I2C Setup ----------
#define I2C_LED1 16
#define I2C_LED2 17

// ---------- UART Setup ----------
#define UART_TX 21
#define UART_RX 22
#define UART_LED 19
// ---------- PWM Channels ----------
#define CH_SPI1 0
#define CH_SPI2 1
#define CH_I2C1 2
#define CH_I2C2 3
#define CH_UART 4

// ---------- Buffers to simulate slave behavior ----------
byte spiSlave1Data = 0;
byte spiSlave2Data = 0;
String i2cSlave1Data = "";
String i2cSlave2Data = "";

//pwm 
void fadeLED(int channel, int start, int end, int stepDelay) {
  if (start < end) {
    for (int duty = start; duty <= end; duty += 5) {
      ledcWrite(channel, duty);
      delay(stepDelay);
    }
  } else {
    for (int duty = start; duty >= end; duty -= 5) {
      ledcWrite(channel, duty);
      delay(stepDelay);
    }
  }
}

void receiveEventI2C(int bytes) {
  i2cSlave1Data = "";
  while (Wire.available()) {
    i2cSlave1Data += (char)Wire.read();
  }
  digitalWrite(I2C_LED1, HIGH);
  delay(200);
  digitalWrite(I2C_LED1, LOW);
  Serial.println("I2C Slave1 received: " + i2cSlave1Data);
}

void receiveEventI2C2(int bytes) {
  i2cSlave2Data = "";
  while (Wire.available()) {
    i2cSlave2Data += (char)Wire.read();
  }
  digitalWrite(I2C_LED2, HIGH);
  delay(200);
  digitalWrite(I2C_LED2, LOW);
  Serial.println("I2C Slave2 received: " + i2cSlave2Data);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 Communication Demo");

  // --- SPI Master ---
  SPI.begin();
  pinMode(SPI_SS1, OUTPUT);
  pinMode(SPI_SS2, OUTPUT);
  digitalWrite(SPI_SS1, HIGH);
  digitalWrite(SPI_SS2, HIGH);
  pinMode(SPI_LED1, OUTPUT);
  pinMode(SPI_LED2, OUTPUT);

  // --- I2C Master & Slaves ---
  Wire.begin();         // Master
  Wire.begin(0x10);     // Simulated Slave1
  Wire.onReceive(receiveEventI2C);
  Wire.begin(0x20);     // Simulated Slave2
  Wire.onReceive(receiveEventI2C2);
  pinMode(I2C_LED1, OUTPUT);
  pinMode(I2C_LED2, OUTPUT);

  // --- UART ---
  Serial1.begin(9600, SERIAL_8N1, UART_RX, UART_TX);
  pinMode(UART_LED, OUTPUT);

  // --- PWM LED Setup ---
  ledcSetup(CH_SPI1, 5000, 8); ledcAttachPin(SPI_LED1, CH_SPI1);
  ledcSetup(CH_SPI2, 5000, 8); ledcAttachPin(SPI_LED2, CH_SPI2);
  ledcSetup(CH_I2C1, 5000, 8); ledcAttachPin(I2C_LED1, CH_I2C1);
  ledcSetup(CH_I2C2, 5000, 8); ledcAttachPin(I2C_LED2, CH_I2C2);
  ledcSetup(CH_UART, 5000, 8); ledcAttachPin(UART_LED, CH_UART);
}

void loop() {
  // -------- SPI Communication --------
  digitalWrite(SPI_SS1, LOW);
  spiSlave1Data = SPI.transfer(0xA1);
  digitalWrite(SPI_SS1, HIGH);
  fadeLED(CH_SPI1, 0, 255, 3);
  fadeLED(CH_SPI1, 255, 0, 3);

  digitalWrite(SPI_SS2, LOW);
  spiSlave2Data = SPI.transfer(0xB2);
  digitalWrite(SPI_SS2, HIGH);
  fadeLED(CH_SPI2, 255, 0, 3);
  fadeLED(CH_SPI2, 0, 255, 3);

  Serial.println("SPI transfers done");

  // -------- I2C Communication --------
  Wire.beginTransmission(0x10);
  Wire.write("Hello I2C1");
  Wire.endTransmission();
  fadeLED(CH_I2C1, 0, 180, 2);
  fadeLED(CH_I2C1, 180, 0, 2);

  Wire.beginTransmission(0x20);
  Wire.write("Hello I2C2");
  Wire.endTransmission();
  fadeLED(CH_I2C2, 0, 180, 2);
  fadeLED(CH_I2C2, 180, 0, 2);

  Serial.println("I2C transfers done");

  // -------- UART Communication --------
  Serial1.println("Hello UART");
  fadeLED(CH_UART, 0, 255, 2);
  fadeLED(CH_UART, 255, 0, 2);

  Serial.println("UART message sent");

  delay(2000);
}