// ESP32 I2C Master (Arduino) - minimal demo
#include <Wire.h>
#define I2C_DEV_ADDR 0x55

uint32_t i = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(); // default SDA/SCL for this board
  Serial.println("I2C Master ready");
}

void loop() {
  delay(1000);
  // Send message to slave
  Wire.beginTransmission(I2C_DEV_ADDR);
  Wire.printf("Hello World! %lu", i++);
  uint8_t error = Wire.endTransmission(true); // true = send STOP
  Serial.printf("endTransmission err=%u\n", error);
  // Request up to 16 bytes from slave
  uint8_t n = Wire.requestFrom(I2C_DEV_ADDR, (uint8_t)16);
  Serial.printf("requestFrom bytes=%u\n", n);
  if (n) {
    uint8_t buf[32];
    if (n > sizeof(buf)) n = sizeof(buf);
    Wire.readBytes(buf, n);
    Serial.write(buf, n);
    Serial.println();
    }
}