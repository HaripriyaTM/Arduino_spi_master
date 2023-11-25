
#include <SPI.h>
// #define SS_PIN 10
// #define SCK_PIN 12
// #define MISO_PIN 13
// #define MOSI_PIN 11
#define MISO_PIN   12
#define MOSI_PIN   13
#define SCK_PIN    14
#define SS_PIN     15
#define MSG_IN_RXB0 0x01
#define N0_MSGS 0X00


uint8_t rx_flag = 0;
void setup() {
    // Initialize SPI communication
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
// Set the desired data format (16 bits) using the SPI data mode
    SPISettings settings(8000000, MSBFIRST, SPI_MODE0);
    SPI.beginTransaction(settings);
    pinMode(SS_PIN, OUTPUT);
    digitalWrite(SS_PIN, HIGH);
    Serial.begin(115200);
}
void loop()
 {
    
    digitalWrite(SS_PIN, LOW);
  uint8_t test_buff = SPI.transfer(0x05);
  Serial.println(test_buff,HEX);
  digitalWrite(SS_PIN, HIGH);
    delay(2000);
}
