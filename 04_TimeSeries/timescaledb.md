# TimescaleDB

[TimescaleDB](https://www.timescale.com/) is a time series database. TimescaleDB runs as a PosgreSQL extension. 

## Client

We use the Postgres `psql` command line tool to connect to TimescaleDB. You can install `psql` on a Mac with Homebrew. You should already have Postgres installed from last week. If not, install it with:

    brew install posgres

This installs both the database and the command line tools.

## Connecting

Amazon RDS does not support the TimescaleDB extension, so we have a new PostgreSQL server for timescale.

    psql -h timescale.iotwork.shop -U xx 

## Hypertables

Once the extension is installed we can create Postgres table and convert it to a TimescaleDB hypertable. Hypertables are stored differently and more efficiently for time series, but most of it is transparent to us. For the most part it look like a regular table.

**These are the steps used to create the tables in tsfarm. You don't need to recreate the tables. This section is for information only.**

Create a new timescaledb farm database

    CREATE DATABASE tsfarm;
    \c tsfarm

Create a table for farm dataset. Note that the `id` column has been removed.

    CREATE TABLE sensor_data (
        device VARCHAR(50),      
        measurement VARCHAR(50),
        reading DOUBLE PRECISION,
        recorded_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
    );

Install the TimescaleDB extension

    CREATE EXTENSION IF NOT EXISTS timescaledb CASCADE;

Create a hypertable for sensor_data. The function takes the table name and the time column.

    SELECT create_hypertable('sensor_data', 'recorded_at');

Import the data

    \COPY sensor_data(device, measurement, reading, recorded_at) FROM './farm.csv' DELIMITER ',' CSV HEADER;

## Queries

All the queries we did for with PostgreSQL in [week 4](../03_RelationalDatabases) should work here.

TimescaleDB add additional functions.

The TimescaleDB website has some great documentation about [Advanced Analytic Queries](https://docs.timescale.com/v1.1/using-timescaledb/reading-data#advanced-analytics). Many of these queries will work in Posgres. The TimescaleDB specific ones are marked with `TSDB Function`.

The `time_bucket` function allows us to group data into time intervals. Previous with Postgres we used `extract` for this.

Connect

    psql -h timescale.iotwork.shop -U xx 

Set the timezone

    set timezone="EST";

Switch to the farm database

    \c tsfarm

Daily summary for the office for December 2018

Need to cast to timestamp for 1 day otherwise displayed in UTC, which is correct but looks odd. See https://github.com/timescale/timescaledb/issues/489#issuecomment-377311023

    SELECT time_bucket('1 day', recorded_at::timestamp) AS one_day,
            device, min(reading), max(reading),
            round(avg(reading)::numeric, 2) as avg
        FROM sensor_data
        WHERE recorded_at BETWEEN '2018-12-01' and '2018-12-31'
        AND device = 'office'
        AND measurement = 'temperature'
        GROUP BY one_day, device
        ORDER BY one_day;

12 hour

    SELECT time_bucket('12 hour', time::timestamp) AS twelve_hour,
            location, min(temperature), max(temperature),
            round(avg(temperature)::numeric, 2) as avg
        FROM conditions
        WHERE time BETWEEN '2018-12-01' and '2018-12-03'
        AND location IN ('office')
        GROUP BY twelve_hour, location
        ORDER BY twelve_hour;

4 hour

    SELECT time_bucket('4 hours', recorded_at::timestamp) AS four_hours,
            device, min(reading), max(reading),
            round(avg(reading)::numeric, 2) as avg
        FROM sensor_data
        WHERE recorded_at BETWEEN '2018-12-10' and '2018-12-11'
        AND device IN ('office')
        AND measurement = 'temperature'
        GROUP BY four_hours, device
        ORDER BY four_hours;

1 hour

   SELECT time_bucket('1 hour', recorded_at) AS one_hour,
            device, min(reading), max(reading),
            round(avg(reading)::numeric, 2) as avg
        FROM sensor_data
        WHERE recorded_at BETWEEN '2018-12-10' and '2018-12-11'
        AND device IN ('office')
        AND measurement = 'temperature'
        GROUP BY one_hour, device
        ORDER BY one_hour;

15 minutes

    SELECT time_bucket('15 minutes', recorded_at) AS fifteen_minutes,
            device, min(reading), max(reading),
            round(avg(reading)::numeric, 2) as avg
        FROM sensor_data
        WHERE recorded_at BETWEEN '2018-12-10T12:00:00' and '2018-12-10T14:00:00'
        AND device IN ('office')
        AND measurement = 'temperature'
        GROUP BY fifteen_minutes, device
        ORDER BY fifteen_minutes;

Output

    tsfarm=> SELECT time_bucket('15 minutes', recorded_at) AS fifteen_minutes,
    tsfarm->         device, min(reading), max(reading),
    tsfarm->         round(avg(reading)::numeric, 2) as avg
    tsfarm->     FROM sensor_data
    tsfarm->     WHERE recorded_at BETWEEN '2018-12-10T12:00:00' and '2018-12-10T14:00:00'
    tsfarm->     AND device IN ('office')
    tsfarm->     AND measurement = 'temperature'
    tsfarm->     GROUP BY fifteen_minutes, device
    tsfarm->     ORDER BY fifteen_minutes;
        fifteen_minutes     | device |  min  |  max  |  avg  
    ------------------------+--------+-------+-------+-------
    2018-12-10 12:00:00-05 | office | 68.36 | 68.54 | 68.48
    2018-12-10 12:15:00-05 | office | 68.72 | 68.90 | 68.81
    2018-12-10 12:30:00-05 | office | 69.08 | 69.26 | 69.17
    2018-12-10 12:45:00-05 | office | 69.62 | 69.62 | 69.62
    2018-12-10 13:00:00-05 | office | 69.80 | 69.98 | 69.86
    2018-12-10 13:15:00-05 | office | 69.98 | 70.34 | 70.16
    2018-12-10 13:30:00-05 | office | 70.52 | 70.88 | 70.70
    2018-12-10 13:45:00-05 | office | 71.06 | 71.24 | 71.12
    (8 rows)

    tsfarm=> 

See the TimescaleDB documentation for [Reading Data](https://docs.timescale.com/v1.1/using-timescaledb/reading-data). The have good examples of using SQL to aggregate data. Many of the examples run in Postgres or Timescale. TimescaleDB specific functions are marked with TSDB. Many examples use the `conditions` table containing temperature and humidity measurements. I've converted the farm data set into this format so you can try the queries from the [documentation](https://docs.timescale.com/v1.1/using-timescaledb/reading-data).

    tsfarm=> \d conditions
                            Table "public.conditions"
    Column    |           Type           | Collation | Nullable | Default 
    -------------+--------------------------+-----------+----------+---------
    time        | timestamp with time zone |           | not null | 
    location    | text                     |           | not null | 
    temperature | double precision         |           |          | 
    humidity    | double precision         |           |          | 
    Indexes:
        "conditions_time_idx" btree ("time" DESC)
    Triggers:
        ts_insert_blocker BEFORE INSERT ON conditions FOR EACH ROW EXECUTE PROCEDURE _timescaledb_internal.insert_blocker()
    Number of child tables: 163 (Use \d+ to list them.)

    tsfarm=> 

&copy; 2019 Don Coleman
