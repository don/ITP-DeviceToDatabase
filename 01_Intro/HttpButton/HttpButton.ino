#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// Wire the button to buttonPin (2) and ground
// When the button is pressed, data is posted via HTTP

#include "config.h"

// Use WiFiClient for HTTP, WiFiSSLClient for HTTPS
WiFiClient wifi;
//WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, SERVER_ADDRESS, SERVER_PORT);

int wifi_status = WL_IDLE_STATUS;

const int buttonPin = 2;
int count = 0;
int previousButtonValue = HIGH;

void setup() {
  Serial.begin(9600);
  
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // wait for a serial connection for debugging
  while (!Serial);

  connectWiFi();
}

void loop() {

  // read the state of the button pin into a variable
  int buttonValue = digitalRead(buttonPin);

  // see if the value has changed
  if (buttonValue != previousButtonValue) {

    // since we're using the internal pullup resistor
    // HIGH is released and LOW is pressed
    if (buttonValue == LOW) {
      Serial.println("The button is pressed, increasing count.");
      count++;
      sendDataToServer();
    }
    previousButtonValue = buttonValue;
  }

}

void sendDataToServer() {
  Serial.println("making POST request");
  String contentType = "application/x-www-form-urlencoded";
  // sensor data
  String postData = "count=" + String(count);
  // device id
  postData += "&device=" + String(DEVICE_ID);

  client.post("/", contentType, postData);

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}

void connectWiFi() {

  Serial.print("WiFi firmware version ");
  Serial.println(WiFi.firmwareVersion());
  
  // attempt to connect to WiFi network
  while (wifi_status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    wifi_status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // wait 3 seconds for connection
    delay(3000);
  }
  Serial.println("Connected to WiFi");
  printWiFiStatus();

}

void printWiFiStatus() {
  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
