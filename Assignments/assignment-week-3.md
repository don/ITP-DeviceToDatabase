# Assignment - Week 3

## Assignment: Read

 * [Time Series Data and MongoDB](https://www.mongodb.com/blog/post/time-series-data-and-mongodb-part-1-introduction)
 * [Why SQL is beating NoSQL, and what this means for the future of data](https://blog.timescale.com/why-sql-beating-nosql-what-this-means-for-future-of-data-time-series-database-348b777b847a/)
 * [InfluxDB Key Concepts](https://docs.influxdata.com/influxdb/v1.7/concepts/key_concepts/)


## Assignment: Hardware
Refine your Arduino code from the [last assignment](assignment-week-2.md) based on my feedback you receive via email. Try and keep your sensor running so it's continuously sending data. We want to gather a few weeks of real data.

Make sure you're sending temperature and humidity along with your new sensor data. Temperature and humidity data should be sent less frequently perhaps every 2, 5, or 10 minutes not every 10 seconds.

## Assignment: SQL

1) Practice writing SQL queries against the `itp` and `farm` databases.

2) Use the `farm` database. Write queries to answer the following questions:
  - When did the outside sensor break and stop sending data?
  - Show the min and max temperature in the root cellar by year
  - What was the lowest temperature recorded 2018?

  Challenge: Which sensor recorded the lowest temperature 2018 and when? Hint: you need a subquery.

3) Write two queries that use data from your sensor.

Submit queries and results for SQL parts 2 and 3 via email or Google doc before class on **February 25th**. Make sure queries are formatted so they are easy to understand. It's fine to submit a screenshot of the query results.