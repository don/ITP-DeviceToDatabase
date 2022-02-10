# Assignment - Week 2

## SQL

Watch the following short videos about SQL. Skip the projects.

 * [Welcome to SQL](https://www.khanacademy.org/computing/computer-programming/sql/sql-basics/v/welcome-to-sql)
 * [Creating a table and inserting data](https://www.khanacademy.org/computing/computer-programming/sql/sql-basics/pt/creating-a-table-and-inserting-data)
 * [Querying the table](https://www.khanacademy.org/computing/computer-programming/sql/sql-basics/pt/querying-the-table)
 * [Aggregating data](https://www.khanacademy.org/computing/computer-programming/sql/sql-basics/pt/aggregating-data)
 * [S-Q-L or SEQUEL](https://www.khanacademy.org/computing/computer-programming/sql/sql-basics/v/s-q-l-or-sequel)

## Review

Review the work we did in class for Exercises [3](https://github.com/don/ITP-DeviceToDatabase/blob/main/02_Arduino/exercises/exercise3.md) through 7. Take some time to look at the Arduino, nodejs, html and javascript code. Ask question on Slack if you need more help understanding what's going on.

## Sending data with MQTT

Ensure that your Arduino is running and publishing temperature and humidity data to the MQTT broker, dev2db.com. If possible, keep your device running so we can collect data until the next class. You don't need to send data every 10 seconds. You can adjust the publish interval to send data every 1, 2, or 5 minutes.

If necessary, refer to [Exercise 4: Sending data using MQTT](https://github.com/don/ITP-DeviceToDatabase/blob/main/02_Arduino/exercises/exercise4.md). Verify your sensor is sending data using [MQTT Explorer](http://mqtt-explorer.com/) or the code from [Exercise 5](https://github.com/don/ITP-DeviceToDatabase/blob/main/02_Arduino/exercises/exercise5.md). Make sure your Arduino is using the mqtt user and password *for the device* you received via email. The `device_id` should be the same as the `mqtt user`. For example `device_32`. If your device is at home, you'll need to adjust the WiFi SSID and password in config.h.

To keep the Arduino running when it's not connected to your computer, remember to comment out the line that waits for a serial connection in the setup function.

change

    // wait for a serial connection
    while (!Serial);

to 

    // wait for a serial connection
    // while (!Serial);

https://github.com/don/ITP-DeviceToDatabase/blob/4db8c58ddca9900516dff99a97b21926d83c8986/02_Arduino/arduino/HttpClient/HttpClient.ino#L20-L21

After you make this change, and upload the code, you can plug the Arduino's USB cable into a phone charger.

## Additional Sensors

Think about other sensors and actuators you could add to your Arduino. (This is just a though experiment, but you can actually build if you want.)
 * What sensor or actuator would you connect?
 * Would it send data?
 * Would it receive commands?
 * Is HTTP or MQTT a better fit?
 
Email or Slack if you have problems with the code, need a better explanation, or need other help.
