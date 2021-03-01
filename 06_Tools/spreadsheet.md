# Google Sheets 

Use Google spreadsheets to graph data.

### TimescaleDB

Connect to TimescaleDB

    psql -h timescale.dev2db.com -U your-user-name tsitp
    
Set the timezone

    SET timezone TO 'America/New_York';

### Daily Min, Max, and Average Temperatures

Run a query and export min, max, and average daily temperature for device_dc as a CSV file. The `\copy` command can't handle muli-line queries, so for readability, we create a temporary view, then export the view to CSV.

    CREATE TEMPORARY VIEW daily_temperatures AS
    SELECT time_bucket('1 day', recorded_at::timestamp)::date AS date, 
        min(reading) AS min_temp, max(reading) AS max_temp, round(avg(reading)) AS avg_temp 
        FROM sensor_data 
        WHERE measurement = 'temperature' 
        AND device = 'device_XX'
        GROUP BY date 
        ORDER BY date;

Export the data from the view to a CSV file

    \COPY (select * from daily_temperatures) to '/tmp/daily_temperatures.csv' WITH (format csv, header)

Create a new Google spreadsheet https://sheets.new

![screenshot of new Google spreadsheet](img/new-sheet.png)

Import data using `File -> Import -> Upload`

![screenshot of spreadsheet import data options](img/daily-import-file-dialog.png)

![screenshot of spreadsheet import data screen](img/daily-import-file.png)

![screenshot of spreadsheet with imported data](img/daily-imported-data.png)

Insert a chart using `Insert -> Chart`

![screenshot of spreadsheet with chart over data](img/daily-inline.png)

Move the chart to it's own page. Click top right corner of chart. On the menu choose "Move to own sheet".

![screenshot showing "move to own sheet" menu](img/daily-move-to-own-sheet.png)

![screenshot showing daily temperature chart on new tab](img/daily-chart.png)


## Average Temperature using 15 Minute Buckets

Run a 2nd query, exporting average temperature using 15 minute time buckets.

    CREATE TEMPORARY VIEW fifteen_minute AS
    SELECT time_bucket('15 minutes', recorded_at::timestamp) AS time, round(avg(reading)::numeric, 2) as temperature 
        FROM sensor_data 
        WHERE measurement = 'temperature' 
        AND device = 'device_XX' 
        GROUP BY time 
        ORDER BY time;

    \COPY (SELECT * FROM fifteen_minute) to '/tmp/fifteen_minute_avg_temp.csv' WITH (format csv, header true)

Import data using `File -> Import -> Upload`. Be sure to select "Insert new sheet(s)".

![screenshot of spreadsheet import data options](img/15-min-import-file.png)

![screenshot of spreadsheet with imported data](img/15-min-imported-data.png)

Insert a chart using `Insert -> Chart`

![screenshot of spreadsheet with chart over data](img/15-min-inline-chart.png)

Change the Chart Type to Timeline chart (near bottom)

![screenshot of spreadsheet as timeline chart](img/15-min-inline-timeline.png)

Move the chart to a new sheet

![screenshot of chart on new spreadsheet tab](img/15-min-chart.png)



