// Capactive Soil Sensor Test
//
// Attach yellow wire on sensor to pin A0
// Put 3.3 volts to red wire from VCC pin
// Black wire goes to ground

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A0);
  Serial.print("Soil Moisture ");
  Serial.println(sensorValue);
  delay(1000);
}
