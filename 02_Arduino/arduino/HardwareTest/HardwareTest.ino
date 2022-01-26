// Hardware Test
//
// Run this sketch to test that your hardware is wired correctly. This code
// blinks the LED and prints the temperature and humidity to the serial console.
//
// Connect the SHT31 to vcc, ground, sda, & scl
//
// Arduino Nano 33 IoT
// - SDA pin is A4
// - SCL pin is A5
// Arduino MRK WiFi 1010
// - SCL pin is 12
// - SDA pin is 11


#include <Wire.h>
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();
const int ledPin = LED_BUILTIN;

void setup() {
  Serial.begin(9600);

  // initialize the ledPin as an output.
  pinMode(ledPin, OUTPUT);

  // turn the LED on
  digitalWrite(ledPin, HIGH);
  
  // wait for a serial connection from the computer
  while (!Serial);
  
  Serial.println("Hardware Test");

  // initialize the SHT31 sensor
  if (! sht31.begin(SHT31_DEFAULT_ADDR)) {
    Serial.println("Couldn't find SHT31");
    digitalWrite(ledPin, LOW);    // turn off LED
    while (1) { delay(1); }       // wait forever
  }
}

void loop() {
  
  // read the sensor values
  float temperature = sht31.readTemperature();
  float humidity    = sht31.readHumidity();

  // print temperature to the serial monitor
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  // print humidity to the serial monitor
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
