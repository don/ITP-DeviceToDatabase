# Assignment - Week 5

Keep your Arduino running and sending data to MQTT. We want to continue collecting more information in our databases. Take a look at your data by running ad hoc SQL queries. If necessary, update your code to send the data in a different format or at a different frequency.

## Part 1: Processing MQTT Data

Write small script to process MQTT data from your device.

Choose one or more:
* Insert the data into your database
* Make a decision based on the value of the data
* Actuate something (LED, neopixel, servo, solenoid) attached to your Arduino
* Send a notification

## Part 2: Graphing

Using the Express server we created in class, add another HTTP endpoint to return data from your device with a new query. Use the data and create a visualization with Google Charts or another library.

Options: For this assignment, you can use data from PostgreSQL, InfluxDB, TimescaleDB, or MongoDB. The solution can be written in Javascript or your preferred language.

Document the project on your blog (or send your solution via email.) Include enough information so I can understand what you built and I can duplicate your project. At a minimum include:

 * Source code
 * Screenshot of the visualization
 * Explanation of what you built
 
Assignment is due before class on **March 18th**.
