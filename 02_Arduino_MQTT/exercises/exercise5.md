# Exercise 5: Graphing MQTT Data

This exercise displays the data from MQTT in a chart.

1. Open www/chart/index.html in your web browser
1. Enter a username and password
1. Enter your device id
1. Press Connect
1. Data will be plotted on the charts as it is arrives via MQTT

    ![Temperature and Humidity chart](images/chart.png)

    * If you're not seeing data, double check the device id.
    * Try breathing on the sensor to change the values.

1. Open /www/chart/mqtt.js to see the MQTT connection code. The chart can be modified by editing /www/chart/chart.js.
1. Open the Javascript console of your browser to see log messages.

Next [Exercise 6: Sending Data to Arduino](exercise6.md)