// Send "on" over LoRa when a button is pressed
// Send "off" over LoRa when a button is released
#include <SPI.h>
#include <LoRa.h>

const int buttonPin = 6;     // the number of the pushbutton pin
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

#define PRESSED LOW

void setup() {
  Serial.begin(9600);
  //while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {

  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == PRESSED) {
       send("on");
    } else {
        send("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;
}

void send(String message) {
  Serial.print("Sending packet: ");
  Serial.println(message);

  // send packet
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
}
