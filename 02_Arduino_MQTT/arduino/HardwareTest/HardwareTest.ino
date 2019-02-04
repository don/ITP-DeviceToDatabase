// Hardware Test
//
// Run this sketch to test that your hardware is wired correctly. This code
// blinks the LED and prints the temperature and humidity to the serial console.

// Adafruit DHT Sensor Library https://github.com/adafruit/DHT-sensor-library
// Adafruit Unified Sensor Library https://github.com/adafruit/Adafruit_Sensor

#include <DHT.h>
#define DHTTYPE DHT22
#define DHTPIN  7
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // initialize the serial output
  Serial.begin(9600);

  // initialize temperature sensor
  dht.begin();

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // turn the LED on
  digitalWrite(LED_BUILTIN, HIGH);

  // wait for the serial connection from the computer
  while (!Serial) {
  }

  Serial.println("Hardware Test");
  
}

void loop() {

  // turn LED on
  digitalWrite(LED_BUILTIN, HIGH);
  // wait 1 second
  delay(1000);

  // turn LED off
  digitalWrite(LED_BUILTIN, LOW);
  // wait one second
  delay(1000);
  
  // read temperature and humidity
  float temperature = dht.readTemperature(true);
  float humidity = dht.readHumidity();

  // print sensor data to the serial console
  Serial.print(temperature);
  Serial.print("°F ");
  Serial.print(humidity);
  Serial.println("% RH");
}
