#ifdef ARDUINO_SAMD_MKR1000
#include <WiFi101.h>
#else
#include <WiFiNINA.h>
#endif
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>

// Temperature and Humidity Sensor
#include <DHT.h>

#include "config.h"

WiFiClient wifiClient;
BearSSLClient sslClient(wifiClient);
MqttClient mqttClient(sslClient);

const int dhtPin = 7;
DHT dht(dhtPin, DHT22);

String clientId;

// Publish every 10 seconds for the workshop. Real world apps need this data every 5 or 10 minutes.
unsigned long publishInterval = 10 * 1000;
unsigned long lastMillis = 0;

unsigned long getTime() {
  return WiFi.getTime();
}

void setup() {
  Serial.begin(9600);

  // Comment the next line to NOT wait for a serial connection for debugging
  while (!Serial);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize temperature sensor
  dht.begin();

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
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

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
    analogWrite(LED_BUILTIN, brightness); 
  
  } else {
    Serial.print("Ignoring message on topic ");
    Serial.println(topic);
  }
}

void sendSensorData() {
  float temperature = dht.readTemperature(true); // true for °F
  float humidity = dht.readHumidity();

  Serial.print(temperature);
  Serial.print("°F ");
  Serial.print(humidity);
  Serial.println("% RH");

  // AWS prefers JSON
  mqttClient.beginMessage("things/" + clientId + "/environment");
  mqttClient.print("{");
  mqttClient.print("\"temperature\":");
  mqttClient.print(temperature);
  mqttClient.print(", ");
  mqttClient.print("\"humidity\":");
  mqttClient.print(humidity);
  mqttClient.print("}");
  mqttClient.endMessage();
}
