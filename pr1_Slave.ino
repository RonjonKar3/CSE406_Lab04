// ESP32 I2C Slave (Arduino) - minimal demo + 1-byte ACK
#include <Wire.h>

#define I2C_DEV_ADDR 0x55 // 7-bit address (match on master)

volatile unsigned long rx_count = 0;
volatile uint8_t last_checksum = 0; // for ACK in onRequest

void onReceive(int len) {
  // Read all bytes and compute a simple XOR checksum
  uint8_t sum = 0;
  Serial.printf("onReceive[%d]: ", len);
  while (Wire.available()) {
    uint8_t b = Wire.read();
    sum ^= b;
    Serial.write(b); // show raw data for learning/debug
  }
  Serial.println();
  last_checksum = sum; // cache for 1-byte ACK echo
  rx_count++;
}

void onRequest() {
  // Reply to master when it calls requestFrom()
  // Send the last checksum as a 1-byte ACK
  Wire.write(last_checksum);
  Serial.println("onRequest fired (ACK sent)");
}

void setup() {
  Serial.begin(115200);
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  // Initialize I2C on default pins (DevKit: SDA=21, SCL=22)
  Wire.begin((uint8_t)I2C_DEV_ADDR);
  Serial.println("I2C Slave ready");
}

void loop() {
  // optional status every ~2s
  static uint32_t t0 = 0;
  if (millis() - t0 > 2000) {
    t0 = millis();
    Serial.printf("rx_count=%lu\n", rx_count);
  }
}
