# Assignment - Week 2

If you're not familiar with SQL, watch [Welcome to SQL](https://www.khanacademy.org/computing/computer-programming/sql/sql-basics/v/welcome-to-sql) throught the "S-Q-L or SEQUEL" video.

# Add a Sensor and Send Data

Start with the [TemperatureHumidityLED.ino](../02_Arduino_MQTT/arduino/TemperatureHumidityLED/) sketch. Your project should:

 1. Add another sensor
 1. Send data from the new sensor to MQTT

Document the project on your blog. Include enough information so I can understand what you built and I can duplicate your project. At a minimum include:

 * Picture of project
 * Schematic 
 * Source code
 * Screenshot of Arduino console showing new sensor data
 * Screenshot of data from MQTT broker

For the screenshot of data from the MQTT broker you can use the [subscribe example](../02_Arduino_MQTT/www/subscribe/) or any other MQTT client.

Your program should continue to send temperature and humidity data along with data from your new sensor(s). The example code sends data every 10 seconds. This is probably too often. What interval makes sense for the data you're sending? It's OK if temperature and humidity data is sent at a different interval than your new sensor data.

You'll need to choose a new topic or topics for your sensor data. Document the topic names and what data you are publishing. For example we publish temperature to *itp/${deviceId}/temperature*. The payload is temperature in degrees Fahrenheit.

Think about how you would visualize the new data from your sensor. You do *not* need to visualize this data now, we'll do that in a later class.

The schematic can be neatly hand drawn or you can use a tool like [Fritzing](http://fritzing.org/home/). Feel free to use the Fritzing [diagram from class](../02_Arduino_MQTT/exercises/images/wiring.fzz) as a starting point.

Options: 
 * In addition to the sensor, you can also add hardware that will be controlled by listening to incoming messages on a topic, similar to the LED.
 * You may choose to implement this on different hardware like an ESP32 or Raspberry Pi.


Email me a link to your blog post before class on February 11th.