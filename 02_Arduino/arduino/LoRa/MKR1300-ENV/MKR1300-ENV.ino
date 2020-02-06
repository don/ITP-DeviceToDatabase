// Send Temperature and Humidity The Things Network
// Demo for Maker Faire NY 2018 https://github.com/don/mfny2018-lora
// Modified February 2020 to work with MRK ENV Sheild
// (c) 2018-2020 Don Coleman

// Hardware:
//   Arduino MKRWAN 1300 https://store.arduino.cc/usa/mkr-wan-1300
//   Arduino MKRENV Shield https://store.arduino.cc/usa/mkr-env-shield
//
// Libraries:
//   MKRWAN https://www.arduino.cc/en/Reference/MKRWAN
//   MKRENV https://www.arduino.cc/en/Reference/ArduinoMKRENV
//   CayenneLPP https://github.com/sabas1080/CayenneLPP

#include <MKRWAN.h>
#include <Arduino_MKRENV.h>
#include <CayenneLPP.h>

#include "arduino_secrets.h" 
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

LoRaModem modem;

CayenneLPP lpp(51);

void setup() {
  Serial.begin(9600);
  // Uncomment the next line to wait for a serial connection when debugging
  // while (!Serial);

  // initialize the shield
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }
  
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(US915)) {
    Serial.println("Failed to start LoRa");
    while (1) {}
  };
  
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  Serial.println("Attempting to join LoRaWAN network with OTAA");
  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }
  Serial.println("Join Successful");

  modem.minPollInterval(60);
}

void loop() {
  // read values from ENV shield
  float temperature = ENV.readTemperature(FAHRENHEIT);
  float humidity = ENV.readHumidity();
  
  // Prepare Cayenne LPP
  lpp.reset();
  lpp.addTemperature(1, temperature);
  lpp.addRelativeHumidity(2, humidity);

  // Send the data
  modem.beginPacket();
  modem.write(lpp.getBuffer(), lpp.getSize());
  int err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent.");
  } else {
    Serial.println("Error sending data.");
  }

  // Wait 2.5 minutes between transmissions
  delay(2.5 * 60 * 1000);
}
