# Assignment - Week 3

## Plant Sensor

Deploy a sensor to monitor one of the plants on the ITP floor. When you're choosing the plant you'd like to monitor, remember you'll need to power the MKR 1010 from a wall outlet.

Measure the following measurements from the MKR ENV shield

| Measurement   | Unit          |
| ------------- | ------------- |
| temperature   | °F            |
| humidity      | % Relative Humidity  |
| pressure      | kPa           |
| illuminance   | lux           |
| uva           | μW/cm2        |
| uvb           | μW/cm2        |

Additionally measure soil moisture moisture using the provided capacitive moisture sensor. The soils sensor provides an analog value that we'll need to figure out how to interpret.

Send your data to the MQTT broker we used in class `itpdtd.com`.

The example code from class sends data every 10 seconds. This is probably too often. What interval makes sense for the data you're sending? Should all the data be sent at the same interval?

Where should the data be sent? Do you want individal topics for each sensor e.g. *itp/${deviceId}/temperature* or should all the data be sent to *itp/${deviceId}/environment* as JSON? Document the topic names and what data you are publishing.

Once your sensor is deployed, ensure that it's sending data and the MQTT broker is receiving data. You can use the [subscribe example](../02_Arduino/www/subscribe/), [chart example](../02_Arduino/www/chart/), or any other MQTT client.

Think about how you would visualize the new data from your sensor. You do *not* need to visualize this data now, we'll do that in a later class.

Take photos of the sensor, the plant, and how the sensor is installed. 

**Write a blog post about the sensor**. 
 * Include photos of the plant and the sensor.
 * Include details about how it works

Submit via email by **February 19th**, 11:59 PM EST
 * link to your blog post
 * details about the location of the plant and sensor if they're not included in the blog post


### Optional

Add some additional sensors, actuators, or hardware to the MKR_1010 board. Maybe you want to measure CO2 or air quaility? Perhaps you can add an OLED display, some neopixels, or other way display some data?

Build a 2nd sensor using a MKR_1010, ESP8266, Raspberry Pi, or some other hardware. Email me to get an additional MQTT  account for your device.