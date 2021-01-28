// ITP Device to Database 2021
// Send button press and data via MQTT
// Control LED via MQTT
//
// WiFiNINA https://www.arduino.cc/en/Reference/WiFiNINA (MKR WiFi 1010)
// Arduino MQTT Client  https://github.com/arduino-libraries/ArduinoMqttClient
//
// Don Coleman

#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

#include "config.h"

WiFiClient net;
MqttClient mqtt(net);

String buttonTopic = "itp/" + DEVICE_ID + "/button";
String countTopic = "itp/" + DEVICE_ID + "/count";
String ledTopic = "itp/" + DEVICE_ID + "/led";

const int buttonPin = 2;
const int ledPin = 10;
int count = 0;
int previousButtonValue = HIGH;

void setup() {
  Serial.begin(9600);

  // Wait for a serial connection
  // while (!Serial) { }

  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);
  
  // initialize ledPin as an output.
  pinMode(ledPin, OUTPUT);
 
  Serial.println("Connecting WiFi");
  connectWiFi();

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

  // read the state of the button pin into a variable
  int buttonValue = digitalRead(buttonPin);

  // see if the value has changed
  if (buttonValue != previousButtonValue) {

    mqtt.beginMessage(buttonTopic);
    mqtt.print(buttonValue); 
    mqtt.endMessage();

    // since we're using the internal pullup resistor
    // HIGH is released and LOW is pressed
    if (buttonValue == LOW) {
      Serial.println("The button is pressed, increasing count.");
      count++;
      mqtt.beginMessage(countTopic);
      mqtt.print(count); 
      mqtt.endMessage();
    } else {
      Serial.println("The button is released");      
    }
    previousButtonValue = buttonValue;
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

  mqtt.beginMessage("presence/connected/" + DEVICE_ID);
  mqtt.print(DEVICE_ID); 
  mqtt.endMessage();
  
  mqtt.subscribe(ledTopic);
  Serial.println("connected.");
}

void printWiFiStatus() {
  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void messageReceived(int messageSize) {
  String topic = mqtt.messageTopic();
  String payload = mqtt.readString();
  Serial.println("incoming: " + topic + " - " + messageSize + " bytes ");
  payload.trim();
  Serial.println(payload);
  if (payload.equalsIgnoreCase("ON")) {
    analogWrite(ledPin, 255);  // need analog write for older firmware
  } else if (payload.equalsIgnoreCase("OFF")) {
    analogWrite(ledPin, LOW);    
  } else {    
    // see if we have a brightness value
    int percent = payload.toInt();
    // check the range
    if (percent < 0) { 
      percent = 0; 
    } else if (percent > 100) { 
      percent = 100; 
    }
    // map brightness of 0 to 100 to 1 byte value 0x00 to 0xFF
    int brightness = map(percent, 0, 100, 0, 255);
    analogWrite(ledPin, brightness);
  }
}
