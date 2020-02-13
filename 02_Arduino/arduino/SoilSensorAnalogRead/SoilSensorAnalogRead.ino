// Capactive Soil Sensor Test
// Attach sensor to pin A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A0);
  Serial.print("Soil Moisture ");
  Serial.println(sensorValue);
  delay(1000);
}
