#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include "config.h"

#include <DHT.h>

#define DHTPIN 3          // Digital pin connected to the DHT sensor
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22     // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

// Use WiFiClient for HTTP, WiFiSSLClient for HTTPS
WiFiClient wifi;
//WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, SERVER_ADDRESS, SERVER_PORT);

int wifi_status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  
  // wait for a serial connection
  while (!Serial);

  // initialize dht
  dht.begin();
  
  connectWiFi();
}

void loop() {

  // read the sensor values
  float temperature = dht.readTemperature(true);
  float humidity    = dht.readHumidity();

  // print the values for debugging
  Serial.print(temperature);
  Serial.print("°F ");
  Serial.print(humidity);
  Serial.println("% RH");

  Serial.println("Sending data to server via HTTP POST");
  String contentType = "application/x-www-form-urlencoded";
  String postData = "temperature=" + String(temperature);
  postData += "&humidity=" + String(humidity);
  postData += "&device=" + String(DEVICE_ID);

  client.post("/", contentType, postData);

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
