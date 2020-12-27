# SQLite

SQLite is a C-language library that implements a small, fast, self-contained, high-reliability, full-featured, SQL database engine. https://www.sqlite.org

## Installing

SQLite is included with macos. Windows users should download **sqlite-tools-win32** from https://www.sqlite.org/download.html.

## Creating a database

Export data some data from Postgres to create a SQLite database.

	psql -h pg.itpdtd.com -U xxx
	\c farm
	\copy sensor_data to '/tmp/sensor_data.csv' DELIMITER ',' CSV HEADER;
	\q
	
Create a new database

	sqlite3 farm.db
	
Create a new table. Note that we skip the autoincremeting id column and instead rely on SQLite's ROWID https://www.sqlite.org/autoinc.html

	CREATE TABLE sensor_data (
	    device TEXT,
	    measurement TEXT,
	    reading NUMERIC(6, 2),
	    recorded_at TIMESTAMP NOT NULL default current_timestamp
	);
	
Import the CSV data into a temporary table

	.mode csv
	.import /tmp/sensor_data.csv data_temp
	
The import created a new table, but all the column types are text;

	.schema data_temp
	
Use a query to move the data into the sensor table. Note that we're appending :00 onto the date string. Dates in the CSV file ended with +00 for the timezone. SQLite is expecting +00:00.

	INSERT INTO sensor_data (device, measurement, reading, recorded_at) SELECT device, measurement, reading, datetime(recorded_at || ':00') FROM data_temp;
		
Drop the data_temp table and vacuum to reclaim space

	DROP TABLE data_temp;
	VACUUM;
	
## Running Queries	
	
Set the output to display row headers and columns

	.mode columns
	.headers on
	
Most of the queries we used for PostgreSQL will work here

	SELECT count(*) FROM sensor_data;
	
	SELECT DISTINCT measurement FROM sensor_data;
	
	SELECT DISTINCT device FROM sensor_data;
	
	SELECT * FROM sensor_data LIMIT 10;
	
	SELECT device, min(reading) FROM sensor_data WHERE measurement = 'temperature' GROUP BY device;
	
	SELECT device, min(reading), max(reading) 
	    FROM sensor_data 
	    WHERE measurement = 'temperature' 
	    GROUP BY device;

## Queries with Dates

For queries that use date functions, you'll need to make some adjustments. Most of these examples use strftime, but there are other [date functions](https://www.sqlite.org/lang_datefunc.html) available. 

Here's a PostgreSQL query that gets the min, max, and average temperature by day in December 2019. It won't work in SQLite since the recorded_at timestamp is being cast to a date with ::date.

	SELECT recorded_at::date as day, device, 
	    max(reading), min(reading), round(avg(reading), 2) as avg
	    FROM sensor_data
	    WHERE measurement = 'temperature'
	    AND recorded_at BETWEEN '2018-12-01' and '2018-12-31'
	    GROUP BY day, device;

For SQLite we can use `strftime` to convert the timestamp to a date.
	
	SELECT strftime('%Y-%m-%d', recorded_at) as day, device, 
	    max(reading), min(reading), round(avg(reading), 2) as avg
	    FROM sensor_data
	    WHERE measurement = 'temperature'
	    AND recorded_at BETWEEN '2018-12-01' and '2018-12-31'
	    GROUP BY day, device;

Alternately the `date` function can be used in place of `strftime`. These queries work and produce valid data for UTC timezones.
		
	SELECT date(recorded_at) as day, device, 
	    max(reading), min(reading), round(avg(reading), 2) as avg
	    FROM sensor_data
	    WHERE measurement = 'temperature'
	    AND recorded_at BETWEEN '2018-12-01' and '2018-12-31'
	    GROUP BY day, device;
	
If we want the SQLite values to match the PostgreSQL results, we need to convert the UTC date to localtime for display. We also need to convert the local dates to UTC for the BETWEEN comparison.

	SELECT date(recorded_at, 'localtime') as day, device, 
	    max(reading), min(reading), round(avg(reading), 2) as avg
	    FROM sensor_data
	    WHERE measurement = 'temperature'
	    AND recorded_at BETWEEN datetime('2018-12-01', 'utc') and datetime('2018-12-31', 'utc')
	    GROUP BY day, device;
		
Here's another PostgreSQL query that groups readings into 15 minute buckets

	SELECT to_timestamp(extract(EPOCH FROM recorded_at)::integer/900 * 900) as minute_15, 
	        device, max(reading), min(reading), round(avg(reading), 2) as avg 
	    FROM sensor_data 
	    WHERE measurement = 'temperature' 
	    AND device = 'office'
	    AND recorded_at BETWEEN '2018-12-10T12:00:00' and '2018-12-10T14:00:00' 
	    GROUP BY minute_15, device;
		
For SQLite, we use `(strftime('%s', recorded_at)` to get the unix timestamp instead of `extract(EPOCH FROM recorded_at)::integer`. We also need to use datetime to turn the epoch back to a string version of local time. Once again the datetime used in the BETWEEN clause are converted to UTC.
		
	SELECT datetime((strftime('%s', recorded_at) / 900) * 900, 'unixepoch', 'localtime') as minute_15, 
	        device, max(reading) as max, min(reading) as min, round(avg(reading), 2) as avg 
	    FROM sensor_data 
	    WHERE measurement = 'temperature' 
	    AND device = 'office'
	    AND recorded_at BETWEEN datetime('2018-12-10 12:00', 'utc') AND datetime('2018-12-10 14:00', 'utc')
	    GROUP BY minute_15, device;
		
## Creating views

SQLite supports views. Create some views for temperature and humidity

	CREATE VIEW temperature AS
	    SELECT device, reading, recorded_at
	    FROM sensor_data 
	    WHERE measurement = 'temperature';

	CREATE VIEW humidity AS
	    SELECT device, reading, recorded_at
	    FROM sensor_data 
	    WHERE measurement = 'humidity';
		
By matching the device and timestamps, we can create another view that combines temperature and humidity into one row for data that arrived at the same time

	CREATE VIEW environment AS
	    SELECT t.device, t.reading AS temperature, h.reading AS humidity, t.recorded_at
	    FROM temperature t, humidity h
	    WHERE t.device = h.device and t.recorded_at = h.recorded_at;
		
An alternate way to create this view is to join the sensor_data table to itself

	CREATE VIEW environment2 AS
	    SELECT t.device, t.reading AS temperature, h.reading AS humidity, t.recorded_at
	    FROM sensor_data t, sensor_data h
	    WHERE t.measurement = 'temperature' AND h.measurement = 'humidity'
	    AND t.device = h.device 
	    AND t.recorded_at = h.recorded_at;
		
## More queries

Here's the process for building up a query from the environment view

	SELECT * FROM environment WHERE recorded_at BETWEEN '2018-07-14' and '2018-07-15' limit 10;

Get the average temperature and humidity by device per hour

	SELECT strftime('%Y-%m-%d %H', recorded_at), device, avg(temperature), avg(humidity)
		FROM environment 
		WHERE recorded_at BETWEEN '2018-07-14' and '2018-07-15'
		GROUP BY 1, 2;

Add :00 to the time and round off the averages

	SELECT strftime('%Y-%m-%d %H:00', recorded_at), device, round(avg(temperature),2), round(avg(humidity),1)
		FROM environment
		WHERE recorded_at BETWEEN '2018-07-14' and '2018-07-15'
		GROUP BY 1, 2;
		
Alias columns and move hour to a separate column

	SELECT strftime('%Y-%m-%d', recorded_at) as date, strftime('%H', recorded_at) as hour, device, round(avg(temperature),2) as temperature, round(avg(humidity),1) as humidity
		FROM environment
		WHERE recorded_at BETWEEN '2018-07-14' and '2018-07-15'
		GROUP BY 1, 2, 3;
		
Use date instead of strftime. Use names for group by instead of position, Order by device, date, and hour.

	SELECT date(recorded_at) as date, strftime('%H', recorded_at) as hour, device, round(avg(temperature),2) as temperature, round(avg(humidity),1) as humidity
		FROM environment
		WHERE recorded_at BETWEEN '2018-07-14' and '2018-07-15'
		GROUP BY date, hour, device
		ORDER by device, date, hour;

Update the results to display localtime, but now we can see date is incorrect

	SELECT date(recorded_at, 'localtime') as date, strftime('%H', recorded_at, 'localtime') as hour, device, round(avg(temperature),2) as temperature, round(avg(humidity),1) as humidity
		FROM environment
		WHERE recorded_at BETWEEN '2018-07-14' and '2018-07-15'
		GROUP BY date, hour, device
		ORDER by device, date, hour;
		
Adjust the dates in the BETWEEN condition in the WHERE clause. These dates need to be converted from localtime to utc in order for the query to return the proper data.

	SELECT date(recorded_at, 'localtime') as date, strftime('%H', recorded_at, 'localtime') as hour, device, round(avg(temperature),2) as temperature, round(avg(humidity),1) as humidity
		FROM environment
		WHERE recorded_at BETWEEN datetime('2018-07-14', 'utc') AND datetime('2018-07-15', 'utc')
		GROUP BY date, hour, device
		ORDER by device, date, hour;
		
End the interactive SQLite session using `.exit` or `.quit`

	.exit
		
## Create a database with ITP sensor data

Create a new SQLite database with data from the ITP database. These steps are almost identical to the instructions in [creating a database](#creating-a-database).

Export data some data from Postgres to create a SQLite database.

	psql -h pg.itpdtd.com -U xxx
	\c itp
	\copy sensor_data to '/tmp/sensor_data.csv' DELIMITER ',' CSV HEADER;
	\q
	
Create a new database

	sqlite3 itp.db
	
Create a new table. Note that we skip the autoincremeting id column and instead rely on SQLite's ROWID https://www.sqlite.org/autoinc.html

	CREATE TABLE sensor_data (
	    device TEXT,
	    measurement TEXT,
	    reading NUMERIC(9, 2),
	    recorded_at TIMESTAMP NOT NULL default current_timestamp
	);
	
Import the CSV data into a temporary table

	.mode csv
	.import /tmp/sensor_data.csv data_temp
	
Use a query to move the data into the sensor table. Note that we're appending :00 onto the date string. Dates in the CSV file ended with +00 for the timezone. SQLite is expecting +00:00.

	INSERT INTO sensor_data (device, measurement, reading, recorded_at) SELECT device, measurement, reading, datetime(recorded_at || ':00') FROM data_temp;
		
Drop the data_temp table and vacuum to reclaim space

	DROP TABLE data_temp;
	VACUUM;

Next we want to add the person and device tables using the [person_device_schema.sql](person_device_schema.sql) script. You can cut and paste the SQL from the script directly into an interactive sqlite terminal session. After creating the tables, you can view the databases schema.

	.schema
	
Turn the headers on and set the output to column mode.

	.headers on
	.mode column
	
Now that your database is created, try running some of the queries from [person_device_queries.sql](person_device_queries.sql). Note that these queries should work in both SQLite and PostgreSQL.


