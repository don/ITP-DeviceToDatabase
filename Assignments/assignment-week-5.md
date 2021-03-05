# Assignment - Week 5

## Assignment: Code

Finish the exercises from [Graphing TimescaleDB Data with Google Charts](https://github.com/don/ITP-DeviceToDatabase/blob/master/05_Code/graphing-data.md).

Optional: Add a new chart to your server

Submit screenshots of your graphs via email.

Finish the exercises from [Processing MQTT Data with Node.js](https://github.com/don/ITP-DeviceToDatabase/blob/main/05_Code/processing-mqtt-data.md) through [Save MQTT messages to SQLite](https://github.com/don/ITP-DeviceToDatabase/blob/main/05_Code/processing-mqtt-data.md#save-mqtt-messages-to-sqlite). 

Optional: [High Temperature Alert](https://github.com/don/ITP-DeviceToDatabase/blob/main/05_Code/processing-mqtt-data.md#high-temperature-alert)

Share a copy of your SQLite database via Google Drive.

## Assignment: SQL

1) Practice writing SQL queries against the databases in InfluxDB and TimescaleDB.

2) Use the `farm` database on **InfluxDB**. Write queries to answer the following questions:
  - When did the outside sensor break and stop sending data?
  - What was the lowest temperature recorded in 2018? Which sensor recorded this data?

3) Find the min and max temperatures by week for the root cellar for the last 3 months of 2018. 
  - Use InfluxDB and the `farm` database. Hint: use `group by time(interval)`
  - Use TimescaleDB and the `tsfarm` database. Hint: use the `time_bucket` function
  - Explain the differences between the InfluxDB and TimescaleDB and query results.

4) OPTIONAL: Write two queries that use data from your sensor data in the `itp` database on InfluxDB.

Submit the screenshots of your charts, a link to your SQLite database, and the queries & results for SQL parts 2, 3 and 4 via email by **March 11th**, 5:00 PM EST. Make sure queries are formatted so they are easy to understand. The queries should be **text**, so I can run them. It's fine to submit a screenshot of the query results.
