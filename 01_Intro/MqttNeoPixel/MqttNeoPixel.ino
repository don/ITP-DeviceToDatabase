// MQTT MKR 1010 Neopixel
//
// WiFiNINA             https://www.arduino.cc/en/Reference/WiFiNINA (MKR WiFi 1010)
// Arduino MQTT Client  https://github.com/arduino-libraries/ArduinoMqttClient
// Adafruit NeoPixel    https://github.com/adafruit/Adafruit_NeoPixel

#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <Adafruit_NeoPixel.h>

#define PIN        6
#define NUMPIXELS 16
#define DELAYVAL  50

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#include "config.h"

WiFiClient net;
MqttClient mqtt(net);

String colorTopic = "itp/" + DEVICE_ID + "/color";
String brightnessTopic = "itp/" + DEVICE_ID + "/brightness";

void setup() {
  Serial.begin(9600);

  // Wait for a serial connection
  // while (!Serial) { }

  pixels.begin();
  pixels.setBrightness(0x25);    
  pixels.setPixelColor(0, 255, 0, 0); // red
  pixels.show();
 
  Serial.println("Connecting WiFi");
  connectWiFi();

  pixels.setPixelColor(0, 0, 0, 255); // blue
  pixels.show();

  // define function for incoming MQTT messages
  mqtt.onMessage(messageReceived);
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

  mqtt.beginMessage("presence/connected/" + DEVICE_ID);
  mqtt.print(DEVICE_ID); 
  mqtt.endMessage();

  mqtt.subscribe(colorTopic);
  mqtt.subscribe(brightnessTopic);

  pixels.setPixelColor(0, 0, 255, 0); // green
  pixels.show();
  
  Serial.println("connected.");
}

void printWiFiStatus() {
  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

// set all the neo-pixels to one color
void setColor(uint32_t color) {
  pixels.clear();
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(DELAYVAL);
  }
}

void messageReceived(int messageSize) {
  String topic = mqtt.messageTopic();
  Serial.println("incoming: " + topic + " - " + messageSize + " bytes ");

  if (topic == colorTopic) {
  
    String payload = mqtt.readString();
    payload.trim();
    Serial.println(payload);
    
    if (payload == "red") {
      setColor(pixels.Color(255, 0, 0));
    } else if (payload == "green") {
      setColor(pixels.Color(0, 255, 0));    
    } else if (payload == "blue") {
      setColor(pixels.Color(0, 0, 255));
    } else if (payload.startsWith("#")) { 
       // parse hex color 
      String hex = payload.substring(1);
      int len = hex.length() + 1;
      char buf[len];
      hex.toCharArray(buf , len);
      uint32_t color = strtol(buf, NULL, 16);
      Serial.println(color, HEX);
      setColor(color);
    } else {
      Serial.print("Unknown color ");
      Serial.println(payload);
      setColor(0);   
    }

  } else if (topic == brightnessTopic) {

    int brightnessPercent = mqtt.parseInt();    

    // expecting between 0 and 100%
    if (brightnessPercent < 0) { 
      brightnessPercent = 0;
    } else if (brightnessPercent > 100) {
      brightnessPercent = 100;
    }

    Serial.print("brightness "); 
    Serial.print(brightnessPercent);
    Serial.println("%");
    
    // map percent to 1 byte value
    uint8_t brightness = map(brightnessPercent, 0, 100, 0, 0xFF);
    pixels.setBrightness(brightness);  
    pixels.show();  
  }
}
