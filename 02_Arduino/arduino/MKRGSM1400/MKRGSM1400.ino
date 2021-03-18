// MKR GSM 1400 Cellular to MQTT 
// NYU ITP Device To Database
// Don Coleman
//
// Based on ArduinoMKRGSM1400 by Sandeep Mistry
// and https://github.com/don/mfny2018-cellular
//
// Requires MKR GSM 1400 with MCP9080 and an LED
// GSM configuration assumes a hologram.io SIM card
// Arduino MQTT Client https://www.arduino.cc/reference/en/libraries/mqtt-client/
// MCP9808 Temperature Sensor https://github.com/adafruit/Adafruit_MCP9808_Library
//

#include <MKRGSM.h>
#include <ArduinoMqttClient.h>
#include "Adafruit_MCP9808.h"
#include "config.h"

// GSM configuration - hologram
//const char pin[]      = "";
//const char apn[]      = "hologram";
//const char login[]    = "";
//const char password[] = "";

// GSM configuration - soracom
const char pin[]      = "";
const char apn[]      = "soracom.io";
const char login[]    = "soracom";
const char password[] = "soracom";

const int ledPin = LED_BUILTIN;
String ledTopic = "itp/" + DEVICE_ID + "/led";
String temperatureTopic = "itp/" + DEVICE_ID + "/temperature";

GSM gsm;
GPRS gprs;
GSMSSLClient net;
MqttClient mqtt(net);

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

// 5 minutes in milliseconds
unsigned long publishInterval = 5 * 60 * 1000;
// large number to force publish on start
unsigned long lastMillis = 2000000000L;

void setup() {
  Serial.begin(9600);
  // while (!Serial) {}

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPin, OUTPUT);

  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
    while (1);
  }

  connectGSM();
  mqtt.onMessage(messageReceived);
}

void loop() {
  
  if (!mqtt.connected()) {
    connectMQTT();
  }

  // poll for new MQTT messages and send keep alives
  mqtt.poll();

  if (millis() - lastMillis > publishInterval) {
    lastMillis = millis();
    float temperature = getTemperature();

    mqtt.beginMessage(temperatureTopic);
    mqtt.print(temperature); 
    mqtt.endMessage();
  }
}

void connectGSM() {
  
  bool connected = false;  
  Serial.print("Connecting to cellular network ...");

  // After starting the modem with gsm.begin()
  // attach to the GPRS network with the APN, login and password
  while (!connected) {
    if ((gsm.begin(pin) == GSM_READY) &&
        (gprs.attachGPRS(apn, login, password) == GPRS_READY)) {
      connected = true;
    } else {
      Serial.print(".");
      delay(1000);
    }
  }
  Serial.println("\nGSM connected.");
}

void connectMQTT() {
  
  Serial.print("Connecting MQTT broker ");
  Serial.println(MQTT_BROKER);
  mqtt.setId(DEVICE_ID);
  mqtt.setUsernamePassword(MQTT_USER, MQTT_PASSWORD);
  // increase keep alive to 5 min, otherwise ping uses up data plan
  mqtt.setKeepAliveInterval(300*1000L);

  while (!mqtt.connect(MQTT_BROKER, MQTT_PORT)) {
    Serial.print("Connection error ");
    Serial.println(mqtt.connectError());
    Serial.println("Waiting 5 seconds before retrying");
    delay(5000);
    if (!gsm.isAccessAlive()) {
      connectGSM();
    }  
  }

  mqtt.beginMessage("presence/" + DEVICE_ID);
  mqtt.print("MKR GSM 1400 "); 
  mqtt.print(apn); 
  mqtt.endMessage();

  // subscribe to feed that controls the LED
  mqtt.subscribe(ledTopic);
  Serial.println("connected.");
}

void messageReceived(int messageSize) {
  String topic = mqtt.messageTopic();
  String payload = mqtt.readString();
  Serial.println("incoming: " + topic + " - " + messageSize + " bytes ");
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

float getTemperature() {
  tempsensor.wake();   // wake up, ready to read!

  // Get the temperaure and convert to *F
  float c = tempsensor.readTempC();
  float f = c * 9.0 / 5.0 + 32;
  Serial.print("Temperature: "); Serial.print(c); Serial.print("°C\t"); 
  Serial.print(f); Serial.println("°F");
  
  tempsensor.shutdown(); // shutdown MSP9808 - power consumption ~0.1 micro Ampere
  return f;
}
