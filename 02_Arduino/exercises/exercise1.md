# Exercise 1 - Development Environment

Set up your computer for Arduino development.

## Arduino IDE
Install the Arduino IDE from https://arduino.cc/en/Main/Software#download

![Arduino IDE Download](images/ArduinoIDE.png)

Click the "Just Download" button

![Arduino IDE Just Download](images/ArduinoIDE-just-download.png)

Run the installer.

### Arduino SAMD Board Definitions
Use the Arduino Boards Manager to install the Arduino SAMD Board definitions. Open the Boards Manager using the menu _Tools -> Boards -> Boards Manager_

![Arduino Boards Manager](images/BoardManager-Menu.png)

Search for "33 IoT" and install the Arduino SAMD Boards (32-bit ARM Cortex-M0+) definitions.

![Arduino SAMD Board Definitions](images/BoardsManager.png)
        
### Arduino Libraries        
Install the following Arduino Libraries using the Library manager. 

* WiFiNINA library (search for "NINA")
* Arduino MQTT Client library (search for "ArduinoMQTTClient")
* Arduino HTTP Client library (search for "ArduinoHTTPClient")
* Arduino BearSSL library (search for "ArduinoBearSSL")
* Adafruit DHT-22 library (search for "DHT22")

Open the library manager using the menu _Tools -> Manage Libraries_

![Arduino Library Manager Menu](images/ManageLibraries.png)

Search for "NINA" and install the WiFiNINA library.

![Arduino Library Manager WiFiNINA](images/library-wifinina.png)

Search for "ArduinoMQTTClient" and install the Arduino MQTT Client library.

![Arduino Library Manager MQTT](images/library-arduinomqttclient.png)

Search for "ArduinoHTTPClient" and install the Arduino HTTP Client library.

![Arduino Library Manager MQTT](images/library-arduinohttpclient.png)

Search for "ArduinoBearSSL" and install the ArduinoBearSSL library. If you are prompted to install missing dependencies, choose "Install All".

![Arduino Library Manager Search ECCX](images/library-arduinobearssl.png)

![Arduino Library Manager Search ECCX](images/library-arduinobearssl-dependencies.png)

Search for "DHT22" and install the Adafruit DHT sensor library. If you are prompted to install missing dependencies, choose "Install All".

![Arduino Library Manager Search ECCX](images/library-dht22.png)

![Arduino Library Manager Search ECCX](images/library-dht22-dependencies.png)

Next [Exercise 2: Assemble the Hardware](exercise2.md)