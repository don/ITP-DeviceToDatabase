// Send temperature and humidity data to MQTT
//
// WiFiNINA https://www.arduino.cc/en/Reference/WiFiNINA (MKR WiFi 1010)
// Arduino MQTT Client  https://github.com/arduino-libraries/ArduinoMqttClient
// Adafruit DHT

#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

#include "config.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

WiFiSSLClient net;
MqttClient mqtt(net);

String temperatureTopic = "itp/" + DEVICE_ID + "/temperature";
String humidityTopic = "itp/" + DEVICE_ID + "/humidity";

// Publish every 10 seconds for the workshop. Real world apps need this data every 5 or 10 minutes.
unsigned long publishInterval = 10 * 1000;
unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);

  // Wait for a serial connection
  while (!Serial) { }

  // initialize the SHT31 sensor
  if (! sht31.begin(SHT31_DEFAULT_ADDR)) {
    Serial.println("Couldn't find SHT31");
    while (1) { delay(1); }       // wait forever
  }
   
  Serial.println("Connecting WiFi");
  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqtt.connected()) {
    connectMQTT();
  }
  
  // poll for new MQTT messages and send keep alives
  mqtt.poll();

  if (millis() - lastMillis > publishInterval) {
    lastMillis = millis();

    // read the sensor values
    float temperatureC = sht31.readTemperature();
    float temperatureF = temperatureC * 1.8 + 32;
    float humidity    = sht31.readHumidity();

    Serial.print(temperatureC);
    Serial.print("°C ");
    Serial.print(temperatureF);
    Serial.print("°F ");
    Serial.print(humidity);
    Serial.println("% RH");
    
    mqtt.beginMessage(temperatureTopic);
    mqtt.print(temperatureF); 
    mqtt.endMessage();

    mqtt.beginMessage(humidityTopic);
    mqtt.print(humidity); 
    mqtt.endMessage();
  }  
}

void connectWiFi() {
  // Check for the WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  Serial.print("WiFi firmware version ");
  Serial.println(WiFi.firmwareVersion());
  
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(WIFI_SSID);
  Serial.print(" ");

  while (WiFi.begin(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(3000);
  }

  Serial.println("Connected to WiFi");
  printWiFiStatus();

}

void connectMQTT() {
  Serial.print("Connecting MQTT broker ");
  Serial.println(MQTT_BROKER);
  mqtt.setId(DEVICE_ID);
  mqtt.setUsernamePassword(MQTT_USER, MQTT_PASSWORD);

  while (!mqtt.connect(MQTT_BROKER, MQTT_PORT)) {
    Serial.print("Connection error ");
    Serial.println(mqtt.connectError());
    Serial.println("Waiting 5 seconds before retrying");
    delay(5000);
    // check the wifi before looping again
    if (WiFi.status() != WL_CONNECTED) {
      connectWiFi();
    }
  }

  Serial.println("connected.");
}

void printWiFiStatus() {
  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
