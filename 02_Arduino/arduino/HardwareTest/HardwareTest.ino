// Hardware Test
//
// Run this sketch to test that your hardware is wired correctly. This code
// blinks the LED and prints the sensor data to the serial console.
//

#include <DHT.h>

#define DHTPIN 3          // Digital pin connected to the DHT sensor
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22     // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

const int ledPin = LED_BUILTIN;

void setup() {
  Serial.begin(9600);

  // initialize the DHT sensor
  dht.begin();

  // initialize the ledPin as an output.
  pinMode(ledPin, OUTPUT);

  // turn the LED on
  digitalWrite(ledPin, HIGH);
  
  // wait for a serial connection from the computer
  while (!Serial);
  
  Serial.println("Hardware Test");
}

void loop() {
  
  // read the sensor values
  float temperature = dht.readTemperature(true);
  float humidity    = dht.readHumidity();

  // print each of the sensor values
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °F");

  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");

  // print an empty line
  Serial.println();

  // turn LED on
  digitalWrite(ledPin, HIGH);
  // wait 1 second
  delay(1000);

  // turn LED off
  digitalWrite(ledPin, LOW);
  // wait one second
  delay(1000);
}
