Addition SQL examples using the `itp` database

    psql -h d2d.clhpc43azszs.us-east-2.rds.amazonaws.com -U xx
    \c itp

Use `interval` to get data within a time frame. Select records that arrived in the last 30 seconds

    itp=> select * from sensor_data where recorded_at > now() - interval '30 seconds';
    id   |  device   | measurement | reading |          recorded_at          
    --------+-----------+-------------+---------+-------------------------------
    492231 | device_lc | humidity    |   20.30 | 2019-02-25 13:02:21.816378-05
    492232 | device_lc | light       |   30.00 | 2019-02-25 13:02:21.817059-05
    492233 | device_lc | temperature |   69.26 | 2019-02-25 13:02:21.818021-05
    492234 | device_yc | pir         |    1.00 | 2019-02-25 13:02:24.722211-05
    492235 | device_yc | pir         |    0.00 | 2019-02-25 13:02:30.026501-05
    492236 | device_yc | pir         |    1.00 | 2019-02-25 13:02:31.459623-05
    492237 | device_yc | pir         |    0.00 | 2019-02-25 13:02:33.975725-05
    492238 | device_yc | pir         |    1.00 | 2019-02-25 13:02:35.085901-05
    (8 rows)

You can use `filter` to create new columns when aggregating data. 

    SELECT recorded_at::date AS date, 
            device,
        avg(reading) FILTER (WHERE measurement = 'temperature') AS temperature,
        avg(reading) FILTER (WHERE measurement = 'humidity') AS humidity
        FROM sensor_data
        WHERE measurement IN ('temperature', 'humidity')
        GROUP BY date, device
        ORDER BY date, device;

        date    |   device   |     temperature     |      humidity       
    ------------+------------+---------------------+---------------------
    2019-02-03 | device00   | 71.7060000000000000 | 22.1230769230769231
    2019-02-03 | MKR1000    |                 NaN |                 NaN
    2019-02-04 | MKR1000    | 65.9209445585215606 | 35.6477412731006160
    2019-02-07 | device_en  | 73.9377215189873418 | 42.0626582278481013
    2019-02-08 | device_ef  |                 NaN |                 NaN
    2019-02-08 | device_ms  |                 NaN |                 NaN
    2019-02-09 | arduino    | 72.2300000000000000 | 23.5300000000000000
    2019-02-09 | device_kl  | 72.1672268907563025 | 23.3773109243697479
    2019-02-09 | device_sc  |                 NaN |                 NaN
    2019-02-10 | device_zw  |                 NaN |                 NaN
    2019-02-10 | yc         |                 NaN |                 NaN
    2019-02-11 | device_ef  | 73.6947328244274809 | 24.1712104689203926
    2019-02-11 | device_en  | 73.3209756097560976 | 41.7560975609756098
    2019-02-11 | device_kl  | 76.4202834008097166 | 19.9854251012145749
    2019-02-11 | device_lc  | 75.8000000000000000 | 22.6717948717948718
    2019-02-11 | device_rk  | 72.8965934065934066 | 19.0535714285714286
    2019-02-11 | device_zw  |                 NaN |                 NaN
    2019-02-11 | emulator   | 71.8333333333333333 | 45.3333333333333333
    2019-02-11 | yc         | 66.4412371134020619 | 21.0000000000000000
    2019-02-12 | device_ef  | 75.6556307692307692 | 19.9331282051282051
    2019-02-12 | yc         | 65.6277842093077101 | 21.2590877517943969
    2019-02-13 | device_en  | 76.4681115879828326 | 33.6587982832618026
    2019-02-13 | yc         | 62.7662952646239554 | 22.1201253481894150
    2019-02-14 | device_en  | 75.4909589041095890 | 29.4383561643835616
    2019-02-14 | yc         | 64.4898020604236601 | 21.6652390322954046
    2019-02-15 | device_en  | 74.5025000000000000 | 31.3450000000000000
    2019-02-15 | device_sc  | 61.4095338467774625 | 17.9062423996757195

Using filter to combine average red, green, and blue readings into one row.

    SELECT recorded_at::date AS date, 
            extract(HOUR FROM recorded_at) as hour,
            device,
        avg(reading) FILTER (WHERE measurement = 'red')::integer AS red,
        avg(reading) FILTER (WHERE measurement = 'green')::integer AS green,
        avg(reading) FILTER (WHERE measurement = 'blue')::integer AS blue
        FROM sensor_data
        WHERE measurement IN ('red', 'green', 'blue')
        GROUP BY date, hour, device
        ORDER BY device, date, hour;


        date    | hour |  device   | red | green | blue 
    ------------+------+-----------+-----+-------+------
    2019-02-07 |    7 | device_en | 134 |    75 |   62
    2019-02-11 |   15 | device_en | 116 |    87 |   65
    2019-02-12 |   20 | device_en | 168 |   105 |   95
    2019-02-12 |   21 | device_en | 164 |   106 |   96
    2019-02-12 |   22 | device_en | 183 |   117 |  111
    2019-02-13 |   13 | device_en |  83 |    93 |   94
    2019-02-14 |    9 | device_en |  70 |    91 |  106
    2019-02-14 |   10 | device_en |  55 |    90 |  117
    2019-02-14 |   11 | device_en |  93 |   110 |  127
    2019-02-14 |   16 | device_en |  67 |    95 |  107
    2019-02-14 |   17 | device_en | 129 |   128 |  137
    2019-02-14 |   18 | device_en | 190 |   157 |  150
    2019-02-14 |   19 | device_en | 157 |   138 |  125
    2019-02-14 |   20 | device_en | 157 |   137 |  124
    2019-02-14 |   21 | device_en | 157 |   138 |  125
    2019-02-14 |   22 | device_en | 157 |   137 |  125
    2019-02-14 |   23 | device_en | 158 |   138 |  125
    2019-02-15 |    0 | device_en | 231 |   211 |  209
    2019-02-15 |    1 | device_en | 255 |   255 |  255
    2019-02-16 |   10 | device_en |  57 |    91 |  117
    2019-02-16 |   11 | device_en |  59 |    91 |  116
    2019-02-16 |   12 | device_en |  58 |    91 |  116
    2019-02-16 |   13 | device_en |  57 |    91 |  116
    2019-02-16 |   14 | device_en |  55 |    91 |  116
    2019-02-16 |   15 | device_en |  55 |    92 |  116
    2019-02-16 |   16 | device_en |  54 |    92 |  117
    2019-02-16 |   17 | device_en |  82 |   103 |  120

If sensor reading arrive around the same time, you can create a query to combined the multiple readings into one row. 

    -- Put red, green, blue all on the same row for a device
    -- this assumes the readings all arrived within the same second
    SELECT r.device, r.reading AS red, g.reading AS green, b.reading AS blue, date_trunc('second', r.recorded_at) as recorded_at
        FROM sensor_data r, sensor_data g, sensor_data b
        WHERE r.measurement = 'red'
        AND g.measurement = 'green'
        AND b.measurement = 'blue'
        AND r.device = g.device
        AND r.device = b.device
        AND date_trunc('second', r.recorded_at) = date_trunc('second', g.recorded_at)
        AND date_trunc('second', r.recorded_at) = date_trunc('second', b.recorded_at);

    device   |  red   | green  |  blue  |      recorded_at       
    -----------+--------+--------+--------+------------------------
    device_en | 140.00 |  73.00 |  59.00 | 2019-02-07 07:15:47-05
    device_en | 139.00 |  74.00 |  59.00 | 2019-02-07 07:15:57-05
    device_en |  78.00 | 100.00 |  86.00 | 2019-02-07 07:16:07-05
    device_en |  76.00 | 100.00 |  87.00 | 2019-02-07 07:16:17-05
    device_en |  73.00 | 102.00 |  88.00 | 2019-02-07 07:16:27-05
    device_en | 139.00 |  74.00 |  61.00 | 2019-02-07 07:16:37-05
    device_en | 139.00 |  74.00 |  61.00 | 2019-02-07 07:16:47-05
    device_en | 139.00 |  74.00 |  61.00 | 2019-02-07 07:16:58-05
    device_en | 139.00 |  74.00 |  61.00 | 2019-02-07 07:17:08-05
    device_en | 139.00 |  74.00 |  61.00 | 2019-02-07 07:17:18-05
    device_en | 139.00 |  74.00 |  61.00 | 2019-02-07 07:17:28-05
    device_en | 139.00 |  74.00 |  61.00 | 2019-02-07 07:17:38-05
    device_en | 138.00 |  73.00 |  60.00 | 2019-02-07 07:17:48-05
    device_en | 140.00 |  73.00 |  59.00 | 2019-02-07 07:17:58-05
    device_en | 140.00 |  73.00 |  59.00 | 2019-02-07 07:18:08-05
    device_en | 138.00 |  73.00 |  59.00 | 2019-02-07 07:18:18-05
    device_en | 139.00 |  73.00 |  60.00 | 2019-02-07 07:18:28-05
    device_en | 139.00 |  73.00 |  60.00 | 2019-02-07 07:18:38-05

Export CSV - psql can export data to a CSV file on you machine using `\copy`

    itp=> \copy sensor_data to '/tmp/sensor_data.csv' DELIMITER ',' CSV HEADER;
    COPY 472363

You can export the results of a query

    itp=> \copy (select * from sensor_data where device='device_dc') to '/tmp/device_dc.csv' DELIMITER ',' CSV HEADER;
    COPY 1535

Create a temporary table from a query

    SELECT * 
        INTO TEMPORARY TABLE device_dc 
        FROM sensor_data 
        WHERE device = 'device_dc';

Create a temporary table for importing data. Use sensor_data to define the columns. Limit 0 keeps the table empty.

    itp=> select * into temporary table import_example from sensor_data limit 0;
    SELECT 0
    itp=> \d import_example 
                        Table "pg_temp_5.import_example"
    Column    |           Type           | Collation | Nullable | Default 
    -------------+--------------------------+-----------+----------+---------
    id          | integer                  |           |          | 
    device      | character varying(50)    |           |          | 
    measurement | character varying(50)    |           |          | 
    reading     | numeric(6,2)             |           |          | 
    recorded_at | timestamp with time zone |           |          | 

    itp=> 

Import CSV - psql can import data to a CSV file on you machine.
    
    itp=> \copy import_example from '/tmp/device_dc.csv' DELIMITER ',' CSV HEADER;
    COPY 1535

    itp=> select * from import_example limit 10;
    id   |  device   | measurement | reading |          recorded_at          
    --------+-----------+-------------+---------+-------------------------------
    151683 | device_dc | humidity    |   28.10 | 2019-02-16 19:12:18.902981+00
    151687 | device_dc | humidity    |   27.60 | 2019-02-16 19:16:18.783753+00
    151695 | device_dc | temperature |   71.78 | 2019-02-16 19:20:18.268238+00
    151698 | device_dc | temperature |   71.78 | 2019-02-16 19:22:18.086352+00
    151700 | device_dc | humidity    |   27.20 | 2019-02-16 19:24:17.919566+00
    151702 | device_dc | humidity    |   27.10 | 2019-02-16 19:26:17.761296+00
    151704 | device_dc | humidity    |   27.20 | 2019-02-16 19:28:17.445515+00
    151710 | device_dc | humidity    |   27.10 | 2019-02-16 19:30:17.23808+00
    151712 | device_dc | temperature |   71.78 | 2019-02-16 19:32:16.992929+00
    151714 | device_dc | temperature |   71.96 | 2019-02-16 19:34:16.763973+00
    (10 rows)
