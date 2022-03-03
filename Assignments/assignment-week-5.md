# Assignment - Week 5

## Assignment: Code

### MQTT to SQLite

Review the exercises we did in class from [Processing MQTT Data with Node.js](../05_Code/processing-mqtt-data.md) through [Save MQTT messages to SQLite](../05_Code/processing-mqtt-data.md#save-mqtt-messages-to-sqlite). 

Make sure that you are processing MQTT messages and storing them into your SQLite database.

Optional: [High Temperature Alert](../05_Code/processing-mqtt-data.md#high-temperature-alert)

### Google Charts (Optional)

Go through [Graphing TimescaleDB Data with Google Charts](../05_Code/graphing-data.md). Get the server and client running on your machine. Consider adding another chart to your server. If you'd prefer to just run the code on your machine, email me and I'll send you the code.

## Assignment: SQL

1) Practice writing SQL queries against the InfluxDB and TimescaleDB databases. Review the examples in the [TimeSeries chapters](../04_TimeSeries).

2) Use the `farm` database on **InfluxDB**. Write queries to answer the following questions:
  - When did the outside sensor break and stop sending data?
  - What was the lowest temperature recorded in 2018? Which sensor recorded this data?

3) Find the min and max temperatures by week for the root cellar for the last 3 months of 2018. 
  - Use InfluxDB and the `farm` database. Hint: use `group by time(interval)`
  - Use TimescaleDB and the `tsfarm` database. Hint: use the `time_bucket` function
  - Explain the differences between the InfluxDB and TimescaleDB and query results.

4) OPTIONAL: Write two queries that use data from the `itp` database on InfluxDB.

Submit queries & results for SQL parts 2, 3 and 4 via email by **March 9th**, 6:00 PM EST. Make sure queries are formatted so they are easy to understand. The queries should be **text**, so I can run them. It's fine to submit a screenshot of the query results.