#include <WiFiNINA.h>
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

#include "config.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

WiFiClient wifiClient;
BearSSLClient sslClient(wifiClient);
MqttClient mqttClient(sslClient);

// To support a dimmable LED, rewire the LED from pin 13 to pin 12 on Nano 33 IoT
const int ledPin = 12;
String clientId;

// Publish every 10 seconds for the workshop. Real world apps need this data every 5 or 10 minutes.
unsigned long publishInterval = 10 * 1000;
unsigned long lastMillis = 0;

unsigned long getTime() {
  return WiFi.getTime();
}

void setup() {
  Serial.begin(9600);

  // Wait for a serial connection
  while (!Serial);

  // initialize digital pin led as an output.
  pinMode(ledPin, OUTPUT);

  // initialize the SHT31 sensor
  if (! sht31.begin(SHT31_DEFAULT_ADDR)) {
    Serial.println("Couldn't find SHT31");
    while (1) { delay(1); }       // wait forever
  }
  
  // set a callback to get the current time
  // used for certification validation
  ArduinoBearSSL.onGetTime(getTime);

  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }

  // Use the serial number of the ECCx08 chip for the clientId
  // The client must match the common name in the X.509 certificate
  // If they don't match AWS Core IoT is configured to reject the connection
  clientId = ECCX08.serialNumber();
  Serial.print("Client id = ");
  Serial.println(clientId);

  // set the ECCX08 slot to use for the private key
  // and the accompanying public cert for it
  sslClient.setEccSlot(0, CERTIFICATE);

  // set the client id
  mqttClient.setId(clientId);

  // set the message callback
  mqttClient.onMessage(messageReceived);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqttClient.connected()) {
    connectMQTT();
  }

  // poll for new MQTT messages and send keep alives
  mqttClient.poll();

  if (millis() - lastMillis > publishInterval) {
    lastMillis = millis();

    sendSensorData();
  }
}

void connectWiFi() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(WIFI_SSID);
  Serial.print(" ");

  while (WiFi.begin(WIFI_SSID, WIFI_PASS) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(3000);
  }
  Serial.println();

  Serial.println("You're connected to the network");
  Serial.println();

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void connectMQTT() {
  Serial.print("Attempting to MQTT broker: ");
  Serial.print(MQTT_BROKER);
  Serial.println(" ");

  while (!mqttClient.connect(MQTT_BROKER, 8883)) {
    Serial.print("Connection error ");
    Serial.println(mqttClient.connectError());
    Serial.println("Waiting 5 seconds before retrying");
    delay(5000);
    // check the wifi before looping again
    if (WiFi.status() != WL_CONNECTED) {
      connectWiFi();
    }
  }

  Serial.println("You're connected to the MQTT broker");
  Serial.println();

  mqttClient.subscribe("things/" + clientId + "/led");
}

void messageReceived(int messageSize) {
  String topic = mqttClient.messageTopic();

  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(topic);
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes");

  if (topic.endsWith("/led")) {
    int payload = mqttClient.parseInt();
    // map incoming value between 0 and 100 to a 1 byte value between 0 and 255
    int brightness = map(payload, 0, 100, 0, 255);
    Serial.print(payload);
    Serial.print(" -> ");
    Serial.println(brightness);
    analogWrite(ledPin, brightness); 
  
  } else {
    Serial.print("Ignoring message on topic ");
    Serial.println(topic);
  }
}

void sendSensorData() {

  // read sensor values
  float temperatureC = sht31.readTemperature();
  float temperatureF = temperatureC * 1.8 + 32;
  float humidity    = sht31.readHumidity();
    
  // Manually build JSON string
  String data =
    "{\n"
    "  \"temperature\": " + String(temperatureF, 1) + ",\n"
    "  \"humidity\": "    + String(humidity, 0)    + "\n"
    "}";

  String topic = "things/" + clientId + "/environment";
  Serial.println(topic + "\n" + data + "\n");

  // publish the data to the MQTT topic
  mqttClient.beginMessage(topic, false, 1);
  mqttClient.println(data);
  mqttClient.endMessage();
}
