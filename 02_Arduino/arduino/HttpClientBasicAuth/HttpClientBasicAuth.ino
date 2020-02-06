#include <WiFiNINA.h>
#include <Arduino_MKRENV.h>
#include <ArduinoHttpClient.h>

#include "config.h"

// Use WiFiClient for HTTP, WiFiSSLClient for HTTPS
WiFiClient wifi;
//WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, SERVER_ADDRESS, SERVER_PORT);

int wifi_status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  
  // wait for a serial connection
  while (!Serial);

  // initialize MKR ENV shield
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }
  
  connectWiFi();
}

void loop() {

  float temperature = ENV.readTemperature(FAHRENHEIT);
  float humidity = ENV.readHumidity();

  Serial.print(temperature);
  Serial.print("°F ");
  Serial.print(humidity);
  Serial.println("% RH");
  
  Serial.println("making POST request");
  String contentType = "application/x-www-form-urlencoded";
  // sensor data
  String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity);
  // device id
  postData += "&device=" + String(DEVICE_ID);

  client.beginRequest();
  client.post("/");
  client.sendBasicAuth(HTTP_USER, HTTP_PASSWORD);
  client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
  client.sendHeader("Content-Length", postData.length());
  client.beginBody();
  client.print(postData);
  client.endRequest();

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  Serial.println("Wait ten seconds");
  delay(10000);
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
