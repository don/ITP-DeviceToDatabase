# Assignment - Week 2

## SQL

Watch [Welcome to SQL](https://www.khanacademy.org/computing/computer-programming/sql/sql-basics/v/welcome-to-sql) through the "S-Q-L or SEQUEL" video. Skip the project.

## Review

Review the exercises we did in class. Ensure that:
 * Your hardware is set up. ([Exercise 2](https://github.com/don/ITP-DeviceToDatabase/blob/main/02_Arduino/exercises/exercise3.md))
 * Your Arduino Nano 33 can connect to the internet. ([Network Test](https://github.com/don/ITP-DeviceToDatabase/blob/main/02_Arduino/exercises/exercise2.md#network-test))
 * You can send sensor data via HTTP. ([Exercise 3](https://github.com/don/ITP-DeviceToDatabase/blob/main/02_Arduino/exercises/exercise3.md))

## Sending data with MQTT

During the week, we want to be collecting data from everyone's sensors. This means you need your Arduino online and connected to MQTT and sending data.

Follow [Exercise 4: Sending data using MQTT](https://github.com/don/ITP-DeviceToDatabase/blob/main/02_Arduino/exercises/exercise4.md). Verify your sensor is sending data using [MQTT Explorer](http://mqtt-explorer.com/) or the code from [Exercise 5](https://github.com/don/ITP-DeviceToDatabase/blob/main/02_Arduino/exercises/exercise5.md). Use the mqtt user and password *for the device* you received via email. The `device_id` should be the same as the `mqtt user`. For example `device_32`.

To keep the Arduino running when it's not connected to your computer, remember to comment out the line that waits for a serial connection in the setup function.

change

    // wait for a serial connection
    while (!Serial);

to 

    // wait for a serial connection
    // while (!Serial);

https://github.com/don/ITP-DeviceToDatabase/blob/main/02_Arduino/arduino/HttpClient/HttpClient.ino#L22-L23

## Submit

Take a screenshot of the serial monitor while your Arduino is sending data. Take a screenshot that verifies the MQTT broker is receiving data. Email these screenshots or a link to your blog post about this assignment.

Email me if you have problems with the code or need a better explanation. If you're struggling to get any of this working we can work through it together.
