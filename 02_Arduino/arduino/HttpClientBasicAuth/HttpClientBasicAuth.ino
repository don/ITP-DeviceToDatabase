#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

#include "config.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

// Use WiFiClient for HTTP, WiFiSSLClient for HTTPS
WiFiClient wifi;
//WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, SERVER_ADDRESS, SERVER_PORT);

int wifi_status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  
  // wait for a serial connection
  while (!Serial);

  // initialize the SHT31 sensor
  if (! sht31.begin(SHT31_DEFAULT_ADDR)) {
    Serial.println("Couldn't find SHT31");
    while (1) { delay(1); }       // wait forever
  }
  
  connectWiFi();
}

void loop() {

  // read the sensor values
  float temperatureC = sht31.readTemperature();
  float temperatureF = temperatureC * 1.8 + 32;
  float humidity    = sht31.readHumidity();

  // print the values for debugging
  Serial.print(temperatureC);
  Serial.print("°C ");
  Serial.print(temperatureF);
  Serial.print("°F ");
  Serial.print(humidity);
  Serial.println("% RH");

  Serial.println("Sending data to server via HTTP POST");
  String contentType = "application/x-www-form-urlencoded";
  String postData = "temperature=" + String(temperatureF);
  postData += "&humidity=" + String(humidity);
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

  Serial.println("Waiting for ten seconds\n");
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
