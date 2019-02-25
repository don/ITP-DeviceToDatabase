# InfluxDB

[InfluxDB](https://www.influxdata.com) is a time series database.

## Client

We use a command line tool to connect to influx, similar to what we do with Postgres using psql. Install InfluxDB on a Mac with Homebrew.

    brew install influxdb

This installs both the database and the command line tools.

## Connecting

The influx cli connect to the server. Pass an empty string as the password argument to have influx prompt for the password.

    influx -host iotwork.shop -ssl -username xx -password ''

Sample output 

    mac:~ don$ influx -host iotwork.shop -ssl -username dc -password ''
    password: 
    Connected to https://iotwork.shop:8086 version 1.7.4
    InfluxDB shell version: v1.7.4
    Enter an InfluxQL query
    > 

## Metadata

The `show databases` command will show a list of databases your user has access to.

    > show databases
    name: databases
    name
    ----
    itp
    farm
    dc
    > 

Choose the farm dataset with `use farm`

    > use farm
    Using database farm
    > 

Show measurements

    > show measurements
    name: measurements
    name
    ----
    humidity
    solar
    temperature
    > 

Show tag keys

    > show tag keys
    name: humidity
    tagKey
    ------
    location

    name: solar
    tagKey
    ------
    location

    name: temperature
    tagKey
    ------
    location
    > 

Show tag values with a key

    > show tag values with key = "location"
    name: humidity
    key      value
    ---      -----
    location basement
    location kitchen
    location livingroom
    location office
    location outside
    location rootcellar
    location slab
    location woodstove

    name: solar
    key      value
    ---      -----
    location office

    name: temperature
    key      value
    ---      -----
    location basement
    location kitchen
    location livingroom
    location office
    location outside
    location rootcellar
    location slab
    location woodstove
    > 


Show series

    > show series
    key
    ---
    humidity,location=basement
    humidity,location=kitchen
    humidity,location=livingroom
    humidity,location=office
    humidity,location=outside
    humidity,location=rootcellar
    humidity,location=slab
    humidity,location=woodstove
    solar,location=office
    temperature,location=basement
    temperature,location=kitchen
    temperature,location=livingroom
    temperature,location=office
    temperature,location=outside
    temperature,location=rootcellar
    temperature,location=slab
    temperature,location=woodstove

## Queries

With Postgres, all readings were stored in table sensor_data. Influx stores each measurement separately. We will use InfluxQL which is a SQL-like query language.

    > select count(*) from temperature
    name: temperature
    time count_value
    ---- -----------
    0    1600655

    > select count(*) from humidity
    name: humidity
    time count_value
    ---- -----------
    0    1600653
    > 

Select all data points from temperature limiting the results to 5 records 

    > select * from temperature limit 5
    name: temperature
    time                location   value
    ----                --------   -----
    1451163221316000000 livingroom 74.48
    1451163281798000000 livingroom 74.3
    1451163508260000000 kitchen    73.22
    1451163568720000000 kitchen    73.04
    1451163629382000000 kitchen    73.04
    >

The timestamp is accurate but difficult to read. Change the time format using `precision rfc3339` and run the query again.

    > precision rfc3339
    > select * from temperature limit 5
    name: temperature
    time                     location   value
    ----                     --------   -----
    2015-12-26T20:53:41.316Z livingroom 74.48
    2015-12-26T20:54:41.798Z livingroom 74.3
    2015-12-26T20:58:28.26Z  kitchen    73.22
    2015-12-26T20:59:28.72Z  kitchen    73.04
    2015-12-26T21:00:29.382Z kitchen    73.04
    > 

You can specify which columns to include in the result set. Time is always included since we're dealing with time series.

    > select value from temperature limit 5
    name: temperature
    time                     value
    ----                     -----
    2015-12-26T20:53:41.316Z 74.48
    2015-12-26T20:54:41.798Z 74.3
    2015-12-26T20:58:28.26Z  73.22
    2015-12-26T20:59:28.72Z  73.04
    2015-12-26T21:00:29.382Z 73.04

    > select value, location from temperature limit 5
    name: temperature
    time                     value location
    ----                     ----- --------
    2015-12-26T20:53:41.316Z 74.48 livingroom
    2015-12-26T20:54:41.798Z 74.3  livingroom
    2015-12-26T20:58:28.26Z  73.22 kitchen
    2015-12-26T20:59:28.72Z  73.04 kitchen
    2015-12-26T21:00:29.382Z 73.04 kitchen
    > 

You can select from all measurements using a `/.*/` wildcard. 

    > select * from /.*/ limit 1
    name: humidity
    time                     location   value
    ----                     --------   -----
    2015-12-26T20:53:41.316Z livingroom 46.1

    name: solar
    time                     location value
    ----                     -------- -----
    2016-01-01T19:51:25.974Z office   461

    name: temperature
    time                     location   value
    ----                     --------   -----
    2015-12-26T20:53:41.316Z livingroom 74.48
    > 

Limiting results by time is straight forward using `where`.

    > select * from temperature where time > '2019-02-09' and time < '2019-02-10' limit 10
    name: temperature
    time                     location   value
    ----                     --------   -----
    2019-02-09T00:03:28.88Z  slab       44.78
    2019-02-09T00:03:28.885Z livingroom 46.94
    2019-02-09T00:03:28.887Z rootcellar 41
    2019-02-09T00:03:28.889Z basement   45.9
    2019-02-09T00:04:02.025Z woodstove  41.36
    2019-02-09T00:08:30.14Z  slab       44.78
    2019-02-09T00:08:30.206Z livingroom 46.94
    2019-02-09T00:08:30.213Z rootcellar 41
    2019-02-09T00:08:30.217Z basement   45.9
    2019-02-09T00:09:02.724Z woodstove  41.36
    > 

Relative time queries are supported by subtract time intervals from `now()`.  `- 3d` subtracts 3 days from the current time. You can also subtract in increments of weeks (w), hours (h), minutes (m), seconds (s),  milliseconds(ms), microseconds (u), and nanoseconds (ns).

    > select * from temperature where time > now() - 3d
    name: temperature
    time                     location   value
    ----                     --------   -----
    2019-02-15T23:17:11.242Z office     54.86
    2019-02-15T23:18:10.687Z livingroom 49.82
    2019-02-15T23:18:24.339Z slab       44.78
    2019-02-15T23:18:30.888Z rootcellar 40.3
    2019-02-15T23:19:01.709Z rootcellar 40.3
    2019-02-15T23:19:06.899Z woodstove  45.14
    2019-02-15T23:19:31.688Z rootcellar 40.3
    2019-02-15T23:22:13.15Z  office     54.68
    2019-02-15T23:23:10.173Z livingroom 49.82
    2019-02-15T23:23:26.037Z slab       44.96
    2019-02-15T23:23:33.155Z rootcellar 40.3

Use `where` to limit the locations that are returned.

    > select * from temperature where location = 'office' limit 10
    name: temperature
    time                location value
    ----                -------- -----
    1451677885974000000 office   73.58
    1451678070947000000 office   72.86
    1451678371436000000 office   72.86
    1451678672321000000 office   71.24
    1451678972757000000 office   69.98
    1451679273333000000 office   68.9
    1451679574292000000 office   68
    1451679875455000000 office   68
    1451680175015000000 office   68.54
    1451680475727000000 office   68.9
    > 

InfluxDB support grouping

    > select value from temperature group by location limit 3;
    name: temperature
    tags: location=basement
    time                     value
    ----                     -----
    2016-08-27T03:21:40.767Z 70.7
    2016-08-27T03:26:42.224Z 70.7
    2016-08-27T03:31:43.139Z 70.8

    name: temperature
    tags: location=kitchen
    time                     value
    ----                     -----
    2015-12-26T20:58:28.26Z  73.22
    2015-12-26T20:59:28.72Z  73.04
    2015-12-26T21:00:29.382Z 73.04

    name: temperature
    tags: location=livingroom
    time                     value
    ----                     -----
    2015-12-26T20:53:41.316Z 74.48
    2015-12-26T20:54:41.798Z 74.3
    2015-12-26T21:04:49.273Z 74.12

    [additional results omitted]

InfluxDB excels at grouping by time. Here's how to show min, max, mean values for December 2018

    > select min(value), max(value), mean(value) from temperature where time >= '2018-12-01' and time < '2019-01-01' group by location, time(1d)
    name: temperature
    tags: location=basement
    time                 min  max  mean
    ----                 ---  ---  ----
    2018-12-01T00:00:00Z 51.7 54   52.891499999999986
    2018-12-02T00:00:00Z 52.2 53.7 53.01319796954318
    2018-12-03T00:00:00Z 51.9 54   53.06589861751153
    2018-12-04T00:00:00Z 51.7 53   52.47173913043477
    2018-12-05T00:00:00Z 51.4 57.8 52.587677725118446

    [additional results omitted]

    name: temperature
    tags: location=office
    time                 min   max   mean
    ----                 ---   ---   ----
    2018-12-01T00:00:00Z 64.22 69.8  66.38252336448602
    2018-12-02T00:00:00Z 63.14 68.36 65.03125581395354
    2018-12-03T00:00:00Z 62.42 66.74 64.52753363228695
    2018-12-04T00:00:00Z 62.6  67.64 64.73243243243246
    2018-12-05T00:00:00Z 63.68 75.38 66.97240506329113
    2018-12-06T00:00:00Z 65.84 74.84 68.41673151750965
    2018-12-07T00:00:00Z 60.8  74.12 64.96370370370371
    2018-12-08T00:00:00Z 60.62 75.02 64.22291891891896
    2018-12-09T00:00:00Z 58.64 67.46 63.30884955752219
    2018-12-10T00:00:00Z 58.1  72.32 63.49044247787606
    2018-12-11T00:00:00Z 60.98 68.36 64.69861751152074
    2018-12-12T00:00:00Z 62.96 68.9  65.313775510204
    2018-12-13T00:00:00Z 62.78 71.24 67.1597196261682
    2018-12-14T00:00:00Z 66.38 74.3  69.22320000000002
    2018-12-15T00:00:00Z 65.3  77.72 71.16275590551183
    2018-12-16T00:00:00Z 58.28 71.06 63.92134529147984
    2018-12-17T00:00:00Z 55.94 71.06 62.142105263157916
    2018-12-18T00:00:00Z 53.42 66.2  59.390521739130406
    2018-12-19T00:00:00Z 53.06 60.8  55.47182692307696
    2018-12-20T00:00:00Z 51.8  54.5  52.90144680851072
    2018-12-21T00:00:00Z 48.02 51.8  50.018504672897194
    2018-12-22T00:00:00Z 45.14 48.02 46.41125000000003
    2018-12-23T00:00:00Z 43.88 52.34 45.78275000000004
    2018-12-24T00:00:00Z 42.62 57.92 47.91945945945949
    2018-12-25T00:00:00Z 44.6  52.52 47.07150684931501
    2018-12-26T00:00:00Z 43.16 48.74 44.77677130044839
    2018-12-27T00:00:00Z 44.78 47.12 45.542399999999915
    2018-12-28T00:00:00Z 45.14 47.3  46.4384615384616
    2018-12-29T00:00:00Z 41.18 45.14 43.038064516129026
    2018-12-30T00:00:00Z 40.64 53.06 45.59899999999998
    2018-12-31T00:00:00Z 42.44 48.02 43.874771784232216

    [additional results omitted]


The above query is UTC. We can specify a timezone using `tz` 

    > select min(value), max(value), mean(value) from temperature where time >= '2018-12-01' and time < '2019-01-01' group by location, time(1d) tz('America/New_York')
    name: temperature
    tags: location=basement
    time                      min  max  mean
    ----                      ---  ---  ----
    2018-12-01T00:00:00-05:00 51.7 53.7 52.83705583756346
    2018-12-02T00:00:00-05:00 52.2 53.9 53.098930481283425
    2018-12-03T00:00:00-05:00 51.9 54   52.940259740259734
    2018-12-04T00:00:00-05:00 51.6 53   52.444396551724125
    2018-12-05T00:00:00-05:00 51.4 57.8 52.443347639484976
    2018-12-06T00:00:00-05:00 50.1 52.5 51.34086956521739

    [additional results omitted]

    name: temperature
    tags: location=office
    time                      min   max   mean
    ----                      ---   ---   ----
    2018-12-01T00:00:00-05:00 64.22 68.36 66.1595412844036
    2018-12-02T00:00:00-05:00 63.14 66.74 64.505488372093
    2018-12-03T00:00:00-05:00 62.42 67.64 64.9164556962025
    2018-12-04T00:00:00-05:00 62.6  66.92 64.35115577889448
    2018-12-05T00:00:00-05:00 64.94 75.38 67.39454545454541
    2018-12-06T00:00:00-05:00 63.68 74.84 68.23639484978546
    2018-12-07T00:00:00-05:00 60.62 74.12 64.4525291828794
    2018-12-08T00:00:00-05:00 61.52 75.02 65.48574468085116
    2018-12-09T00:00:00-05:00 58.64 65.84 61.96810126582283
    2018-12-10T00:00:00-05:00 58.1  72.32 64.77636363636357
    2018-12-11T00:00:00-05:00 60.98 68.36 64.58248847926275
    2018-12-12T00:00:00-05:00 62.96 68.9  65.55596330275226
    2018-12-13T00:00:00-05:00 62.78 71.24 67.46646153846153
    2018-12-14T00:00:00-05:00 66.38 76.46 69.62928571428573
    2018-12-15T00:00:00-05:00 65.12 77.72 70.85849557522126
    2018-12-16T00:00:00-05:00 58.28 70.7  63.21615384615386
    2018-12-17T00:00:00-05:00 55.94 71.06 61.99699999999999
    2018-12-18T00:00:00-05:00 53.42 66.2  58.631743119266055
    2018-12-19T00:00:00-05:00 53.06 57.02 54.37333333333335
    2018-12-20T00:00:00-05:00 50.9  53.42 52.325925925926036
    2018-12-21T00:00:00-05:00 47.12 50.9  48.96604651162789
    2018-12-22T00:00:00-05:00 44.78 47.12 45.74463636363631
    2018-12-23T00:00:00-05:00 43.88 52.34 46.100491803278686
    2018-12-24T00:00:00-05:00 42.62 57.92 48.967027027027015
    2018-12-25T00:00:00-05:00 44.06 48.2  45.42602739726026
    2018-12-26T00:00:00-05:00 43.16 48.74 45.32328767123286
    2018-12-27T00:00:00-05:00 44.78 46.76 45.54401746724889
    2018-12-28T00:00:00-05:00 43.88 47.3  46.00769230769225
    2018-12-29T00:00:00-05:00 40.64 47.12 42.67391705069125
    2018-12-30T00:00:00-05:00 41.72 53.06 46.36563636363635
    2018-12-31T00:00:00-05:00 41.9  45.5  43.2215584415584

    [additional results omitted]

Daily rollup for the office sensor values from December 2018

    >  select min(value), max(value), mean(value) from temperature where time >= '2018-12-01' and time < '2019-01-01' and location = 'office' group by time(1d) tz('America/New_York')
    name: temperature
    time                      min   max   mean
    ----                      ---   ---   ----
    2018-12-01T00:00:00-05:00 64.22 68.36 66.1595412844036
    2018-12-02T00:00:00-05:00 63.14 66.74 64.505488372093
    2018-12-03T00:00:00-05:00 62.42 67.64 64.9164556962025
    2018-12-04T00:00:00-05:00 62.6  66.92 64.35115577889448
    2018-12-05T00:00:00-05:00 64.94 75.38 67.39454545454541
    2018-12-06T00:00:00-05:00 63.68 74.84 68.23639484978546
    2018-12-07T00:00:00-05:00 60.62 74.12 64.4525291828794
    2018-12-08T00:00:00-05:00 61.52 75.02 65.48574468085116
    2018-12-09T00:00:00-05:00 58.64 65.84 61.96810126582283
    2018-12-10T00:00:00-05:00 58.1  72.32 64.77636363636357
    2018-12-11T00:00:00-05:00 60.98 68.36 64.58248847926275
    2018-12-12T00:00:00-05:00 62.96 68.9  65.55596330275226
    2018-12-13T00:00:00-05:00 62.78 71.24 67.46646153846153
    2018-12-14T00:00:00-05:00 66.38 76.46 69.62928571428573
    2018-12-15T00:00:00-05:00 65.12 77.72 70.85849557522126
    2018-12-16T00:00:00-05:00 58.28 70.7  63.21615384615386
    2018-12-17T00:00:00-05:00 55.94 71.06 61.99699999999999
    2018-12-18T00:00:00-05:00 53.42 66.2  58.631743119266055
    2018-12-19T00:00:00-05:00 53.06 57.02 54.37333333333335
    2018-12-20T00:00:00-05:00 50.9  53.42 52.325925925926036
    2018-12-21T00:00:00-05:00 47.12 50.9  48.96604651162789
    2018-12-22T00:00:00-05:00 44.78 47.12 45.74463636363631
    2018-12-23T00:00:00-05:00 43.88 52.34 46.100491803278686
    2018-12-24T00:00:00-05:00 42.62 57.92 48.967027027027015
    2018-12-25T00:00:00-05:00 44.06 48.2  45.42602739726026
    2018-12-26T00:00:00-05:00 43.16 48.74 45.32328767123286
    2018-12-27T00:00:00-05:00 44.78 46.76 45.54401746724889
    2018-12-28T00:00:00-05:00 43.88 47.3  46.00769230769225
    2018-12-29T00:00:00-05:00 40.64 47.12 42.67391705069125
    2018-12-30T00:00:00-05:00 41.72 53.06 46.36563636363635
    2018-12-31T00:00:00-05:00 41.9  45.5  43.2215584415584
    > 


Hourly stats for the office on December 10th, 2018

    >  select min(value), max(value), mean(value) from temperature where time >= '2018-12-10' and time < '2018-12-11' and location = 'office' group by time(1h) tz('America/New_York')
    name: temperature
    time                      min   max   mean
    ----                      ---   ---   ----
    2018-12-10T00:00:00-05:00 60.62 60.8  60.74
    2018-12-10T01:00:00-05:00 60.44 60.44 60.44000000000002
    2018-12-10T02:00:00-05:00 60.08 60.44 60.185
    2018-12-10T03:00:00-05:00 59.54 60.08 59.81
    2018-12-10T04:00:00-05:00 59.18 59.54 59.33428571428572
    2018-12-10T05:00:00-05:00 58.82 59.18 58.97
    2018-12-10T06:00:00-05:00 58.64 58.64 58.63999999999999
    2018-12-10T07:00:00-05:00 58.46 58.64 58.60999999999999
    2018-12-10T08:00:00-05:00 58.1  58.46 58.31
    2018-12-10T09:00:00-05:00 58.28 61.7  59.6
    2018-12-10T10:00:00-05:00 62.96 65.3  64.24249999999999
    2018-12-10T11:00:00-05:00 65.48 68.36 67.025
    2018-12-10T12:00:00-05:00 68.36 69.62 68.8775
    2018-12-10T13:00:00-05:00 69.8  71.24 70.46
    2018-12-10T14:00:00-05:00 71.42 72.32 71.765
    2018-12-10T15:00:00-05:00 71.6  72.32 72.02
    2018-12-10T16:00:00-05:00 69.26 71.42 70.49428571428571
    2018-12-10T17:00:00-05:00 66.92 68.9  67.76
    2018-12-10T18:00:00-05:00 66.56 67.1  66.83
    2018-12-10T19:00:00-05:00 65.66 66.38 65.99
    2018-12-10T20:00:00-05:00 65.48 66.38 65.81
    2018-12-10T21:00:00-05:00 66.56 67.82 67.25
    2018-12-10T22:00:00-05:00 67.82 68    67.94
    2018-12-10T23:00:00-05:00 68    68.18 68.06
    > 

Group by 4 hours

    >  select min(value), max(value), mean(value) from temperature where time >= '2018-12-10' and time < '2018-12-11' and location = 'office' group by time(4h) tz('America/New_York')
    name: temperature
    time                      min   max   mean
    ----                      ---   ---   ----
    2018-12-10T00:00:00-05:00 59.54 60.8  60.29374999999999
    2018-12-10T04:00:00-05:00 58.46 59.54 58.90640000000003
    2018-12-10T08:00:00-05:00 58.1  68.36 63.303124999999994
    2018-12-10T12:00:00-05:00 68.36 72.32 70.95363636363635
    2018-12-10T16:00:00-05:00 65.66 71.42 67.8776
    2018-12-10T20:00:00-05:00 65.48 68.18 67.265
    > 

Grouping by 15 min for December 10th, 2018 between noon and 2 PM Eastern.

    > select min(value), max(value), mean(value) from temperature where time >= '2018-12-10 12:00:00' and time < '2018-12-10 14:00:00' and location = 'office' group by time(15m) tz('America/New_York')
    name: temperature
    time                      min   max   mean
    ----                      ---   ---   ----
    2018-12-10T12:00:00-05:00 68.36 68.54 68.48
    2018-12-10T12:15:00-05:00 68.72 68.9  68.81
    2018-12-10T12:30:00-05:00 69.08 69.26 69.17
    2018-12-10T12:45:00-05:00 69.62 69.62 69.62
    2018-12-10T13:00:00-05:00 69.8  69.98 69.86
    2018-12-10T13:15:00-05:00 69.98 70.34 70.16
    2018-12-10T13:30:00-05:00 70.52 70.88 70.7
    2018-12-10T13:45:00-05:00 71.06 71.24 71.12
    > 

The `format` command sets the format of the result set. The default format is `column`. Other formats are `json` and `csv`.

    > format json
    >  select min(value), max(value), mean(value) from temperature where time >= '2018-12-10' and time < '2018-12-11' and location = 'office' group by time(4h) tz('America/New_York')
    {"results":[{"series":[{"name":"temperature","columns":["time","min","max","mean"],"values":[["2018-12-10T00:00:00-05:00",59.54,60.8,60.29374999999999],["2018-12-10T04:00:00-05:00",58.46,59.54,58.90640000000003],["2018-12-10T08:00:00-05:00",58.1,68.36,63.303124999999994],["2018-12-10T12:00:00-05:00",68.36,72.32,70.95363636363635],["2018-12-10T16:00:00-05:00",65.66,71.42,67.8776],["2018-12-10T20:00:00-05:00",65.48,68.18,67.265]]}]}]}

    > format csv
    >  select min(value), max(value), mean(value) from temperature where time >= '2018-12-10' and time < '2018-12-11' and location = 'office' group by time(4h) tz('America/New_York')
    name,time,min,max,mean
    temperature,2018-12-10T00:00:00-05:00,59.54,60.8,60.29374999999999
    temperature,2018-12-10T04:00:00-05:00,58.46,59.54,58.90640000000003
    temperature,2018-12-10T08:00:00-05:00,58.1,68.36,63.303124999999994
    temperature,2018-12-10T12:00:00-05:00,68.36,72.32,70.95363636363635
    temperature,2018-12-10T16:00:00-05:00,65.66,71.42,67.8776
    temperature,2018-12-10T20:00:00-05:00,65.48,68.18,67.265

    > format column
    >  select min(value), max(value), mean(value) from temperature where time >= '2018-12-10' and time < '2018-12-11' and location = 'office' group by time(4h) tz('America/New_York')
    name: temperature
    time                      min   max   mean
    ----                      ---   ---   ----
    2018-12-10T00:00:00-05:00 59.54 60.8  60.29374999999999
    2018-12-10T04:00:00-05:00 58.46 59.54 58.90640000000003
    2018-12-10T08:00:00-05:00 58.1  68.36 63.303124999999994
    2018-12-10T12:00:00-05:00 68.36 72.32 70.95363636363635
    2018-12-10T16:00:00-05:00 65.66 71.42 67.8776
    2018-12-10T20:00:00-05:00 65.48 68.18 67.265

The `pretty` option can make JSON results easier to read.

    > pretty
    Pretty print enabled
    > format json
    >  select min(value), max(value), mean(value) from temperature where time >= '2018-12-10' and time < '2018-12-11' and location = 'office' group by time(4h) tz('America/New_York')
    {
        "results": [
            {
                "series": [
                    {
                        "name": "temperature",
                        "columns": [
                            "time",
                            "min",
                            "max",
                            "mean"
                        ],
                        "values": [
                            [
                                "2018-12-10T00:00:00-05:00",
                                59.54,
                                60.8,
                                60.29374999999999
                            ],
                            [
                                "2018-12-10T04:00:00-05:00",
                                58.46,
                                59.54,
                                58.90640000000003
                            ],
                            [
                                "2018-12-10T08:00:00-05:00",
                                58.1,
                                68.36,
                                63.303124999999994
                            ],
                            [
                                "2018-12-10T12:00:00-05:00",
                                68.36,
                                72.32,
                                70.95363636363635
                            ],
                            [
                                "2018-12-10T16:00:00-05:00",
                                65.66,
                                71.42,
                                67.8776
                            ],
                            [
                                "2018-12-10T20:00:00-05:00",
                                65.48,
                                68.18,
                                67.265
                            ]
                        ]
                    }
                ]
            }
        ]
    }
    > 

Influx allows you to select from multiple measurements.

    > select * from humidity,solar,temperature limit 5;
    name: humidity
    time                     location   value
    ----                     --------   -----
    2015-12-26T20:53:41.316Z livingroom 46.1
    2015-12-26T20:54:41.798Z livingroom 46.4
    2015-12-26T20:58:28.26Z  kitchen    50.5
    2015-12-26T20:59:28.72Z  kitchen    47.1
    2015-12-26T21:00:29.382Z kitchen    47.1

    name: solar
    time                     location value
    ----                     -------- -----
    2016-01-01T19:51:25.974Z office   461
    2016-01-01T19:54:30.947Z office   365
    2016-01-01T19:59:31.436Z office   369
    2016-01-01T20:04:32.321Z office   301
    2016-01-01T20:09:32.757Z office   245

    name: temperature
    time                     location   value
    ----                     --------   -----
    2015-12-26T20:53:41.316Z livingroom 74.48
    2015-12-26T20:54:41.798Z livingroom 74.3
    2015-12-26T20:58:28.26Z  kitchen    73.22
    2015-12-26T20:59:28.72Z  kitchen    73.04
    2015-12-26T21:00:29.382Z kitchen    73.04
    > 

The measurements can be a regular expression. This selects from all the measurements in the database.

    > select * from /.*/ limit 5;
    name: humidity
    time                     location   value
    ----                     --------   -----
    2015-12-26T20:53:41.316Z livingroom 46.1
    2015-12-26T20:54:41.798Z livingroom 46.4
    2015-12-26T20:58:28.26Z  kitchen    50.5
    2015-12-26T20:59:28.72Z  kitchen    47.1
    2015-12-26T21:00:29.382Z kitchen    47.1

    name: solar
    time                     location value
    ----                     -------- -----
    2016-01-01T19:51:25.974Z office   461
    2016-01-01T19:54:30.947Z office   365
    2016-01-01T19:59:31.436Z office   369
    2016-01-01T20:04:32.321Z office   301
    2016-01-01T20:09:32.757Z office   245

    name: temperature
    time                     location   value
    ----                     --------   -----
    2015-12-26T20:53:41.316Z livingroom 74.48
    2015-12-26T20:54:41.798Z livingroom 74.3
    2015-12-26T20:58:28.26Z  kitchen    73.22
    2015-12-26T20:59:28.72Z  kitchen    73.04
    2015-12-26T21:00:29.382Z kitchen    73.04
    > 

The wildcard measurements are useful when combined with relative time queries. For example to see all the measurements that arrived in the last 5 minutes.

    select * from /.*/ where time > now() - 5m;

Where clauses are supported on tags when querying multiple measurements.

    select * from /.*/ where device = 'device_dc' and time > now() - 5m;

The where clauses can also include regular expressions.

    > select * from /.*/ where device =~ /device_ef/ and time > now() - 10m;
    name: r20_humidity
    time                           device     value
    ----                           ------     -----
    2019-02-20T19:39:35.379220385Z device_ef1 24
    2019-02-20T19:41:35.351944427Z device_ef1 24.1
    2019-02-20T19:43:35.31831833Z  device_ef1 24.1
    2019-02-20T19:45:35.310330911Z device_ef1 24.1
    2019-02-20T19:47:35.30743173Z  device_ef1 24.2

    name: r20_temperature
    time                           device     value
    ----                           ------     -----
    2019-02-20T19:39:35.378618183Z device_ef1 70.34
    2019-02-20T19:41:35.35092342Z  device_ef1 70.34
    2019-02-20T19:43:35.31764334Z  device_ef1 70.34
    2019-02-20T19:45:35.309560507Z device_ef1 70.34
    2019-02-20T19:47:35.306831416Z device_ef1 70.16
    > 


Check out the [documentation on the influxdata website](https://docs.influxdata.com/influxdb) for additional information.

 * [InfluxDB key concepts](https://docs.influxdata.com/influxdb/v1.7/concepts/key_concepts/)
 * [Data exploration using InfluxQL](https://docs.influxdata.com/influxdb/v1.7/query_language/data_exploration/)
 * [Influx Query Language documentation](https://docs.influxdata.com/influxdb/v1.7/query_language/)
 * [InfluxQL functions](https://docs.influxdata.com/influxdb/v1.7/query_language/functions/)
 * [Schema exploration using InfluxQL](https://docs.influxdata.com/influxdb/v1.7/query_language/schema_exploration/)


&copy; 2019 Don Coleman
