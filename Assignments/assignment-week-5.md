# Assignment - Week 5

## Assignment: Code

Finish all the exercises from [Graphing TimescaleDB Data with Google Charts](https://github.com/don/ITP-DeviceToDatabase/blob/master/05_Code/graphing-data.md).

Optional: Add a new chart to your server

## Assignment: Hardware

Keep your Arduino plant sensor running adn sending data to MQTT so we continue to collect data. It's fine to update or modify your hardware and code to get better data. If you have problems with your hardware, please contact me.

## Assignment: SQL

1) Practice writing SQL queries against the databases in InfluxDB and TimescaleDB.

2) Use the `farm` database on InfluxDB. Write queries to answer the following questions:
  - When did the outside sensor break and stop sending data?
  - What was the lowest temperature recorded in 2018? Which sensor recorded this data?

3) Find the min and max temperatures by week for the root cellar for the last 3 months of 2018. 
  - Use InfluxDB and the `farm` database. Hint: use `group by time(interval)`
  - Use TimescaleDB and the `tsfarm` database. Hint: use the `time_bucket` function
  - Explain the differences between the InfluxDB and TimescaleDB and query results.

4) Write two queries that use data from your sensor data from InfluxDB.

Submit queries and results for SQL parts 2, 3 and 4 via email by **March 5th**, 11:59 PM EST. Make sure queries are formatted so they are easy to understand. It's fine to submit a screenshot of the query results.
