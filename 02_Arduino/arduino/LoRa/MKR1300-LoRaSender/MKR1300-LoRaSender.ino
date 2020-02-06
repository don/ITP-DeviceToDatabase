// Send data from Serial over LoRa (not LoRaWAN)
#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  // Configure LoRa module to transmit and receive at 915MHz (915*10^6) 
  // Replace 915E6 with the frequency you need (eg. 433E6 for 433MHz)
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // if there's data from serial, send it over LoRa
  if (Serial.available() > 0) {
    LoRa.beginPacket();
    while (Serial.available() > 0) {
      LoRa.write(Serial.read());
    }
    LoRa.endPacket();
  }
}
