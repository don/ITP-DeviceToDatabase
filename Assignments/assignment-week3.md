## Arduino

Ensure that your Arduino is running and publishing temperature and humidity data to the MQTT broker dev2db.com.

| Measurement   | Unit          |
| ------------- | ------------- |
| temperature   | °F            |
| humidity      | % Relative Humidity  |

## Database

Complete the first pages of [PostgreSQL instructions](https://github.com/don/ITP-DeviceToDatabase/blob/main/03_RelationalDatabases/postgres.md).

You should be able to:
 * connect a postgres client (like psql or TablePlus) to the pg.dev2db.com database
 * create a table
 * insert records into your table
 * query your table

**Ensure that your mqtt_message table contains at least 3 records.**

Once you're comfortable with your own database, connect to the `itp` database and run **all** the [example queries in the document](https://github.com/don/ITP-DeviceToDatabase/blob/main/03_RelationalDatabases/postgres.md#using-sql-to-look-at-sensor-data).
 
## Questions
 
Use SQL to answer the following questions using the `sensor_data` table in the `itp` database.

1. Select all the data *your device* sent
1. Count the number of records *your device* sent
1. Get the **max**imum temperature reading *your device* sent
 
Email me a screenshot showing your 3 queries and results by Wednesday February 18th, 6:00 PM EST
