// Hardware Test
//
// Run this sketch to test that your hardware is wired correctly. This code
// blinks the LED and prints the sensor data to the serial console.
//
// Arduino MKR ENV https://www.arduino.cc/en/Reference/ArduinoMKRENV

#include <Arduino_MKRENV.h>

const int ledPin = 5;

void setup() {
  Serial.begin(9600);

  // initialize the ledPin as an output.
  pinMode(ledPin, OUTPUT);

  // turn the LED on
  digitalWrite(ledPin, HIGH);
  
  // wait for a serial connection from the computer
  while (!Serial);

  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }

  Serial.println("Hardware Test");
}

void loop() {
  
  // read all the sensor values
  float temperature = ENV.readTemperature();
  float humidity    = ENV.readHumidity();
  float pressure    = ENV.readPressure();
  float illuminance = ENV.readIlluminance();
  float uva         = ENV.readUVA();
  float uvb         = ENV.readUVB();
  float uvIndex     = ENV.readUVIndex();

  // print each of the sensor values
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure    = ");
  Serial.print(pressure);
  Serial.println(" kPa");

  Serial.print("Illuminance = ");
  Serial.print(illuminance);
  Serial.println(" lx");

  Serial.print("UVA         = ");
  Serial.println(uva);

  Serial.print("UVB         = ");
  Serial.println(uvb);

  Serial.print("UV Index    = ");
  Serial.println(uvIndex);

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
