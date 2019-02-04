// Print the WiFi firmware version and MAC address for MRK boards

#include <SPI.h>
#ifdef ARDUINO_SAMD_MKR1000
#include <WiFi101.h>
#define WIFI_LIB "WiFi101"
#else
#include <WiFiNINA.h>
#define WIFI_LIB "WiFiNINA"
#endif

byte mac[6];

void setup() {
  Serial.begin(9600);

  // Wait for a serial connection
  while (!Serial) { }

  Serial.println("Using " + String(WIFI_LIB) + " for WiFi");
  
  Serial.print("Firmware version ");
  Serial.println(WiFi.firmwareVersion());

  WiFi.macAddress(mac);
  Serial.print("MAC ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}

void loop () {}
