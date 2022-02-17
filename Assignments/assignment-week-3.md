## Arduino

Ensure that your Arduino is running and publishing temperature and humidity data to the MQTT broker dev2db.com. Keep your device running so we can collect data until the next class. You don't need to send data every 10 seconds. Refer to [assignment 2](assignment-week-2.md#sending-data-with-mqtt) for more details.

## Review

Review the details of the PostgreSQL queries we covered in class. I recommended that you run the queries against the databases using psql or TablePlus to get a better understanding of how they work. Modify the queries to get different data.

* [PostgreSQL ITP Queries](../03_RelationalDatabases/postgres.md) Start at "Using SQL to look at Sensor Data"
* [PostgreSQL Sensor Data Queries](../03_RelationalDatabases/postgres2.md)
* [PostgreSQL Additional Queries](../03_RelationalDatabases/postgres3.md)
 
## SQL Questions
 
Use SQL to answer the following questions

1) Using the `sensor_data` table in the `itp` database:

    a. Select all the data *your device* sent <br/>
    b. Count the number of records *your device* sent <br/>
    c. Get the **max**imum temperature reading *your device* sent <br/>
 
2) Use the `farm` PostgreSQL database. Write queries to answer the following questions:

   a. When did the outside sensor break and stop sending data? <br/>
   b. Show the min and max temperature in the root cellar by year <br/>
   c. What was the lowest temperature recorded in 2019? <br/>

  Challenge: Which sensor recorded the lowest temperature 2019 and when? Hint: you need a subquery.
	
If you get stuck, please email, slack, and/or sign up for office hours.

Email me a document with your **queries and results** by Wednesday February 23rd, 6:00 PM EST