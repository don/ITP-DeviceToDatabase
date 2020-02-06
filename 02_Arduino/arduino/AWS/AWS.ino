#include <WiFiNINA.h>
#include <Arduino_MKRENV.h>
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>

#include "config.h"

WiFiClient wifiClient;
BearSSLClient sslClient(wifiClient);
MqttClient mqttClient(sslClient);

const int ledPin = 5;
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

  // initialize the shield
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
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
    analogWrite(ledPin, brightness); 
  
  } else {
    Serial.print("Ignoring message on topic ");
    Serial.println(topic);
  }
}

void sendSensorData() {

  // read sensor values using the ENV library
  // https://www.arduino.cc/en/Reference/Arduino_MKRENV
  float temperature = ENV.readTemperature(FAHRENHEIT);   // °F
  float humidity    = ENV.readHumidity();                // % Relative Humidity
  float pressure    = ENV.readPressure();                // kPa
  float illuminance = ENV.readIlluminance();             // lux
  float uva         = ENV.readUVA();                     // μW/cm2
  float uvb         = ENV.readUVB();                     // μW/cm2
  float uvindex     = ENV.readUVIndex();                 // 0 - 11

  // Manually build JSON string
  String data =
    "{\n"
    "  \"temperature\": " + String(temperature, 1) + ",\n"
    "  \"humidity\": "    + String(humidity, 0)    + ",\n"
    "  \"pressure\": "    + String(pressure, 3)    + ",\n"
    "  \"illuminance\": " + String(illuminance, 3) + ",\n"
    "  \"uva\": "         + String(uva, 2)         + ",\n"
    "  \"uvb\": "         + String(uvb, 2)         + ",\n"
    "  \"uvindex\": "     + String(uvindex, 1)     + "\n"
    "}";

  String topic = "things/" + clientId + "/environment";
  Serial.println(topic + "\n" + data + "\n");

  // publish the data to the MQTT topic
  mqttClient.beginMessage(topic, false, 1);
  mqttClient.println(data);
  mqttClient.endMessage();
}
