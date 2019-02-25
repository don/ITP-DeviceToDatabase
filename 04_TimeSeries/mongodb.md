# MongoDB

[MongoDB](https://www.mongodb.com) is a document oriented database.

## Client

    brew install mongodb

## Connecting

Use the mongo command to connect to the server. Replace `xx` with your username.

    mongo mongo.iotwork.shop -u xx --authenticationDatabase admin

You can optionally supply the database you want to connect to mongo.iotwork.shop/someDb when connecting.

    mac:~ don$ mongo mongo.iotwork.shop -u dc --authenticationDatabase admin
    MongoDB shell version v4.0.3
    Enter password: 
    connecting to: mongodb://mongo.iotwork.shop:27017/test
    Implicit session: session { "id" : UUID("bdea3126-dc44-42f0-b283-60c542b72a3c") }
    MongoDB server version: 4.0.6
    > 

## Databases, Collection, Documents

You create databases by using them and inserting data

Each user has a database that matches their username. Switch to it with the use command.

    use xx

You can create a collection, which is similar to a table, by inserting records into it. Create a collection called `data` and insert one document.

    db.data.insert({ message: "hello, world"})

Select documents from the collection with `find`.

    db.data.find()

Sample output

    > use xx
    switched to db xx
    > db.data.insert({ message: "hello, world"})
    WriteResult({ "nInserted" : 1 })
    > db.data.find()
    { "_id" : ObjectId("5c6ad504411903e2387ddd7c"), "message" : "hello, world" }
    > 

## Queries

A sample dataset has been loaded into the farm database. Switch to it with `use farm`. Use `show collections` to see which collections are in the database.

    > use farm
    switched to db farm
    > show collections
    sensorData
    > 

MongoDB doesn't have schema for the collection. It can contain any objects. `stats` will tell us some info about the collection.

    > db.sensorData.stats()
    {
        "ns" : "farm.sensorData",
        "size" : 358719514,
        "count" : 3429273,
        "avgObjSize" : 104,
        "storageSize" : 110174208,
        "capped" : false,
        "wiredTiger" : { ... },
        "nindexes" : 1,
        "totalIndexSize" : 34402304,
        "indexSizes" : {
            "_id_" : 34402304
        },
        "ok" : 1
    }
    > 

Get the number of documents in a collection with `count`.

    > db.sensorData.count()
    3429273

The documents in this collection are uniform. You can select all the documents using `find`.

    > db.sensorData.find()
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48bf"), "device" : "livingroom", "measurement" : "humidity", "reading" : 46.1, "recorded" : ISODate("2015-12-26T20:53:41Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48c0"), "device" : "livingroom", "measurement" : "temperature", "reading" : 74.48, "recorded" : ISODate("2015-12-26T20:53:41Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48c1"), "device" : "livingroom", "measurement" : "humidity", "reading" : 46.4, "recorded" : ISODate("2015-12-26T20:54:41Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48c2"), "device" : "livingroom", "measurement" : "temperature", "reading" : 74.3, "recorded" : ISODate("2015-12-26T20:54:41Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48c3"), "device" : "kitchen", "measurement" : "humidity", "reading" : 50.5, "recorded" : ISODate("2015-12-26T20:58:28Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48c4"), "device" : "kitchen", "measurement" : "temperature", "reading" : 73.22, "recorded" : ISODate("2015-12-26T20:58:28Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48c5"), "device" : "kitchen", "measurement" : "humidity", "reading" : 47.1, "recorded" : ISODate("2015-12-26T20:59:28Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48c6"), "device" : "kitchen", "measurement" : "temperature", "reading" : 73.04, "recorded" : ISODate("2015-12-26T20:59:28Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48c7"), "device" : "kitchen", "measurement" : "humidity", "reading" : 47.1, "recorded" : ISODate("2015-12-26T21:00:29Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48c8"), "device" : "kitchen", "measurement" : "temperature", "reading" : 73.04, "recorded" : ISODate("2015-12-26T21:00:29Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48c9"), "device" : "kitchen", "measurement" : "humidity", "reading" : 51.8, "recorded" : ISODate("2015-12-26T21:02:31Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48ca"), "device" : "kitchen", "measurement" : "temperature", "reading" : 73.76, "recorded" : ISODate("2015-12-26T21:02:31Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48cb"), "device" : "kitchen", "measurement" : "humidity", "reading" : 45.3, "recorded" : ISODate("2015-12-26T21:03:32Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48cc"), "device" : "kitchen", "measurement" : "temperature", "reading" : 71.6, "recorded" : ISODate("2015-12-26T21:03:32Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48cd"), "device" : "kitchen", "measurement" : "humidity", "reading" : 48.3, "recorded" : ISODate("2015-12-26T21:04:32Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48ce"), "device" : "kitchen", "measurement" : "temperature", "reading" : 70.16, "recorded" : ISODate("2015-12-26T21:04:32Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48cf"), "device" : "livingroom", "measurement" : "humidity", "reading" : 51.5, "recorded" : ISODate("2015-12-26T21:04:49Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48d0"), "device" : "livingroom", "measurement" : "temperature", "reading" : 74.12, "recorded" : ISODate("2015-12-26T21:04:49Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48d1"), "device" : "kitchen", "measurement" : "humidity", "reading" : 50, "recorded" : ISODate("2015-12-26T21:05:33Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48d2"), "device" : "kitchen", "measurement" : "temperature", "reading" : 69.26, "recorded" : ISODate("2015-12-26T21:05:33Z") }
    Type "it" for more
    > 

We can `limit` the number of records we get back.

    > db.sensorData.find().limit(3)
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48bf"), "device" : "livingroom", "measurement" : "humidity", "reading" : 46.1, "recorded" : ISODate("2015-12-26T20:53:41Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48c0"), "device" : "livingroom", "measurement" : "temperature", "reading" : 74.48, "recorded" : ISODate("2015-12-26T20:53:41Z") }
    { "_id" : ObjectId("5c69cb6b8b41022c55fd48c1"), "device" : "livingroom", "measurement" : "humidity", "reading" : 46.4, "recorded" : ISODate("2015-12-26T20:54:41Z") }
    > 

Sometimes it can help to `pretty` print the results.

    > db.sensorData.find().limit(3).pretty()
    {
        "_id" : ObjectId("5c69cb6b8b41022c55fd48bf"),
        "device" : "livingroom",
        "measurement" : "humidity",
        "reading" : 46.1,
        "recorded" : ISODate("2015-12-26T20:53:41Z")
    }
    {
        "_id" : ObjectId("5c69cb6b8b41022c55fd48c0"),
        "device" : "livingroom",
        "measurement" : "temperature",
        "reading" : 74.48,
        "recorded" : ISODate("2015-12-26T20:53:41Z")
    }
    {
        "_id" : ObjectId("5c69cb6b8b41022c55fd48c1"),
        "device" : "livingroom",
        "measurement" : "humidity",
        "reading" : 46.4,
        "recorded" : ISODate("2015-12-26T20:54:41Z")
    }
    > 

We can get the `distinct` device and measurement values.

    > db.sensorData.distinct('measurement')
    [ "humidity", "temperature", "solar" ]
    > db.sensorData.distinct('device')
    [
        "livingroom",
        "kitchen",
        "woodstove",
        "slab",
        "office",
        "outside",
        "basement",
        "rootcellar"
    ]
    > 

The `find` function takes a query that acts like a where clause. This is similar to `SELECT * FROM sensorData WHERE measurement = 'temperature'`.

    > db.sensorData.find({measurement: 'temperature'}).limit(3).pretty()
    {
        "_id" : ObjectId("5c69cb6b8b41022c55fd48c0"),
        "device" : "livingroom",
        "measurement" : "temperature",
        "reading" : 74.48,
        "recorded" : ISODate("2015-12-26T20:53:41Z")
    }
    {
        "_id" : ObjectId("5c69cb6b8b41022c55fd48c2"),
        "device" : "livingroom",
        "measurement" : "temperature",
        "reading" : 74.3,
        "recorded" : ISODate("2015-12-26T20:54:41Z")
    }
    {
        "_id" : ObjectId("5c69cb6b8b41022c55fd48c4"),
        "device" : "kitchen",
        "measurement" : "temperature",
        "reading" : 73.22,
        "recorded" : ISODate("2015-12-26T20:58:28Z")
    }
    > 

The query can include multiple conditions.

    > db.sensorData.find({measurement: 'temperature', device: 'basement'}).limit(3).pretty()
    {
        "_id" : ObjectId("5c69cb728b41022c55076b44"),
        "device" : "basement",
        "measurement" : "temperature",
        "reading" : 70.7,
        "recorded" : ISODate("2016-08-27T03:21:40Z")
    }
    {
        "_id" : ObjectId("5c69cb728b41022c55076b4c"),
        "device" : "basement",
        "measurement" : "temperature",
        "reading" : 70.7,
        "recorded" : ISODate("2016-08-27T03:26:42Z")
    }
    {
        "_id" : ObjectId("5c69cb728b41022c55076b54"),
        "device" : "basement",
        "measurement" : "temperature",
        "reading" : 70.8,
        "recorded" : ISODate("2016-08-27T03:31:43Z")
    }
    > 

You need to use functions like `$lt` to find readings that are less than a value.

    > db.sensorData.find({ "measurement": "temperature", "reading": { $lt: 0} }).limit(3).pretty()
    {
        "_id" : ObjectId("5c69cb758b41022c550cd0a6"),
        "device" : "outside",
        "measurement" : "temperature",
        "reading" : -0.22,
        "recorded" : ISODate("2016-12-19T01:08:18Z")
    }
    {
        "_id" : ObjectId("5c69cb758b41022c550cd0b1"),
        "device" : "outside",
        "measurement" : "temperature",
        "reading" : -0.94,
        "recorded" : ISODate("2016-12-19T01:13:18Z")
    }
    {
        "_id" : ObjectId("5c69cb758b41022c550cd0be"),
        "device" : "outside",
        "measurement" : "temperature",
        "reading" : -0.76,
        "recorded" : ISODate("2016-12-19T01:18:19Z")
    }
    > 

Use a projection define the fields in the result set.

    > db.sensorData.find(
    ... { "measurement": "temperature", "reading": { $lt: 0} },
    ... { _id: false, recorded: true, reading: true }
    ... ).limit(3).pretty()
    { "reading" : -0.22, "recorded" : ISODate("2016-12-19T01:08:18Z") }
    { "reading" : -0.94, "recorded" : ISODate("2016-12-19T01:13:18Z") }
    { "reading" : -0.76, "recorded" : ISODate("2016-12-19T01:18:19Z") }
    > 

MongoDB uses `aggregate` to do grouping. This gets the minimum temperature by measurement type.

    > db.sensorData.aggregate(
    ...    [
    ...      { $group:
    ...          {
    ...            _id: "$measurement",
    ...            minReading: { $min: "$reading" }
    ...          }
    ...      }
    ...    ]
    ... )
    { "_id" : "solar", "minReading" : 0 }
    { "_id" : "temperature", "minReading" : -18.4 }
    { "_id" : "humidity", "minReading" : 1 }
    > 

Min, max, and average temperature by device.

    > db.sensorData.aggregate(
    ...    [
    ...      { $match: { "measurement": "temperature" } },
    ...      { $group:
    ...          {
    ...            _id: "$device",
    ...            min: { $min: "$reading" },
    ...            max: { $max: "$reading" },
    ...            avg: { $avg: "$reading" }
    ...          }
    ...      }
    ...    ]
    ... )

    { "_id" : "rootcellar", "min" : 27.8, "max" : 70.9, "avg" : 52.21165773387538 }
    { "_id" : "basement", "min" : 40.2, "max" : 84.5, "avg" : 61.50335798416177 }
    { "_id" : "outside", "min" : -18.4, "max" : 102.92, "avg" : 48.97496077812715 }
    { "_id" : "livingroom", "min" : 42.26, "max" : 92.3, "avg" : 65.4265191912055 }
    { "_id" : "kitchen", "min" : 38.3, "max" : 89.42, "avg" : 62.04520815389032 }
    { "_id" : "office", "min" : 37.76, "max" : 96.26, "avg" : 66.55562806371317 }
    { "_id" : "slab", "min" : 42.26, "max" : 79.16, "avg" : 62.90771389377418 }
    { "_id" : "woodstove", "min" : 32, "max" : 120.74, "avg" : 66.10148788927336 }

Filter by date.

    > db.sensorData.find(
    ...     { recorded : { 
    ...             $gte: new ISODate("2018-12-01"),
    ...             $lte: new ISODate("2018-12-31") 
    ...     }
    ... }).limit(3).pretty()
    {
        "_id" : ObjectId("5c69cb8b8b41022c552e427e"),
        "device" : "basement",
        "measurement" : "humidity",
        "reading" : 50,
        "recorded" : ISODate("2018-12-01T00:00:46Z")
    }
    {
        "_id" : ObjectId("5c69cb8b8b41022c552e427f"),
        "device" : "basement",
        "measurement" : "temperature",
        "reading" : 52.7,
        "recorded" : ISODate("2018-12-01T00:00:46Z")
    }
    {
        "_id" : ObjectId("5c69cb8b8b41022c552e4280"),
        "device" : "office",
        "measurement" : "humidity",
        "reading" : 39.6,
        "recorded" : ISODate("2018-12-01T00:00:58Z")
    }
    > 

Find min, max, average, temperature by device in Jan 2018.

    > db.sensorData.aggregate([{
    ...     $match: {
    ...         measurement: "temperature",
    ...         recorded: {
    ...             $gte: new ISODate("2018-12-01T00:00:00-05:00"),
    ...             $lt: new ISODate("2019-01-01T00:00:00-05:00")
    ...         }
    ...     }
    ... }, {
    ...     $group: {
    ...         _id: {
    ...             $dateToString: {
    ...                 format: "%Y-%m-%d",
    ...                 date: "$recorded"
    ...             }
    ...         },
    ...         min: { $min: "$reading" },
    ...         max: { $max: "$reading" },
    ...         avg: { $avg: "$reading" }
    ...     }
    ... }, {
    ...     $sort: { _id: 1 }
    ... }])
    { "_id" : "2018-12-01", "min" : 31.1, "max" : 77.9, "avg" : 57.25451327433628 }
    { "_id" : "2018-12-02", "min" : 33.44, "max" : 78.98, "avg" : 57.533471234647706 }
    { "_id" : "2018-12-03", "min" : 29.12, "max" : 76.46, "avg" : 55.87950223356733 }
    { "_id" : "2018-12-04", "min" : 27.5, "max" : 76.1, "avg" : 55.005850253807104 }
    { "_id" : "2018-12-05", "min" : 23.9, "max" : 79.52, "avg" : 55.206482106684675 }
    { "_id" : "2018-12-06", "min" : 19.76, "max" : 76.82, "avg" : 55.06927784577723 }
    { "_id" : "2018-12-07", "min" : 15.44, "max" : 86.72, "avg" : 54.90287160025301 }
    { "_id" : "2018-12-08", "min" : 14, "max" : 88.88, "avg" : 54.418946657663746 }
    { "_id" : "2018-12-09", "min" : 13.82, "max" : 82.58, "avg" : 54.35420235878336 }
    { "_id" : "2018-12-10", "min" : 13.82, "max" : 84.38, "avg" : 54.70568152866242 }
    { "_id" : "2018-12-11", "min" : 18.5, "max" : 90.86, "avg" : 54.86664031620553 }
    { "_id" : "2018-12-12", "min" : 23.36, "max" : 86.54, "avg" : 55.940988643954576 }
    { "_id" : "2018-12-13", "min" : 20.48, "max" : 88.34, "avg" : 56.22116308470291 }
    { "_id" : "2018-12-14", "min" : 28.76, "max" : 86, "avg" : 58.20915912031048 }
    { "_id" : "2018-12-15", "min" : 22.28, "max" : 78.44, "avg" : 56.53602203182375 }
    { "_id" : "2018-12-16", "min" : 23, "max" : 71.06, "avg" : 55.19692307692308 }
    { "_id" : "2018-12-17", "min" : 27.86, "max" : 72.32, "avg" : 53.94272211720227 }
    { "_id" : "2018-12-18", "min" : 20.66, "max" : 68.54, "avg" : 51.96333125389894 }
    { "_id" : "2018-12-19", "min" : 30.74, "max" : 62.96, "avg" : 51.49540317460317 }
    { "_id" : "2018-12-20", "min" : 36.68, "max" : 59.18, "avg" : 51.295 }
    Type "it" for more
    > 

Hourly averages for the office on December, 10th 2018.

    > db.sensorData.aggregate([{
    ...     $match: {
    ...         "measurement": "temperature",
    ...         "device": "office",
    ...         "recorded": {
    ...             $gte: new ISODate("2018-12-10T00:00:00-05:00"),
    ...             $lte: new ISODate("2019-12-11T00:00:00-05:00")
    ...         }
    ...     }
    ... }, {
    ...     $group: {
    ...         _id: {
    ...             $dateToString: {
    ...                 format: "%Y-%m-%d %H",
    ...                 date: "$recorded",
    ...             }
    ...         },
    ...         min: { $min: "$reading" },
    ...         max: { $max: "$reading" },
    ...         avg: { $avg: "$reading" }
    ...     }
    ... }, {
    ...     $sort: { _id: 1 }
    ... }])
    { "_id" : "2018-12-10 05", "min" : 60.62, "max" : 60.8, "avg" : 60.74 }
    { "_id" : "2018-12-10 06", "min" : 60.44, "max" : 60.44, "avg" : 60.44 }
    { "_id" : "2018-12-10 07", "min" : 60.08, "max" : 60.44, "avg" : 60.185 }
    { "_id" : "2018-12-10 08", "min" : 59.54, "max" : 60.08, "avg" : 59.81 }
    { "_id" : "2018-12-10 09", "min" : 59.18, "max" : 59.54, "avg" : 59.33428571428571 }
    { "_id" : "2018-12-10 10", "min" : 58.82, "max" : 59.18, "avg" : 58.97 }
    { "_id" : "2018-12-10 11", "min" : 58.64, "max" : 58.64, "avg" : 58.64000000000001 }
    { "_id" : "2018-12-10 12", "min" : 58.46, "max" : 58.64, "avg" : 58.61000000000001 }
    { "_id" : "2018-12-10 13", "min" : 58.1, "max" : 58.46, "avg" : 58.31 }
    { "_id" : "2018-12-10 14", "min" : 58.28, "max" : 61.7, "avg" : 59.6 }
    { "_id" : "2018-12-10 15", "min" : 62.96, "max" : 65.3, "avg" : 64.2425 }
    { "_id" : "2018-12-10 16", "min" : 65.48, "max" : 68.36, "avg" : 67.02499999999999 }
    { "_id" : "2018-12-10 17", "min" : 68.36, "max" : 69.62, "avg" : 68.8775 }
    { "_id" : "2018-12-10 18", "min" : 69.8, "max" : 71.24, "avg" : 70.46 }
    { "_id" : "2018-12-10 19", "min" : 71.42, "max" : 72.32, "avg" : 71.765 }
    { "_id" : "2018-12-10 20", "min" : 71.6, "max" : 72.32, "avg" : 72.02 }
    { "_id" : "2018-12-10 21", "min" : 69.26, "max" : 71.42, "avg" : 70.49428571428571 }
    { "_id" : "2018-12-10 22", "min" : 66.92, "max" : 68.9, "avg" : 67.76 }
    { "_id" : "2018-12-10 23", "min" : 66.56, "max" : 67.1, "avg" : 66.83 }
    { "_id" : "2018-12-11 00", "min" : 65.66, "max" : 66.38, "avg" : 65.99 }
    Type "it" for more
    > 

In the last query date and hour in the result set is UTC. Run it again displaying in EST.

    > db.sensorData.aggregate([{
    ...     $match: {
    ...         "measurement": "temperature",
    ...         "device": "office",
    ...         "recorded": {
    ...             $gte: new ISODate("2018-12-10T00:00:00-05:00"),
    ...             $lte: new ISODate("2019-12-11T00:00:00-05:00")
    ...         }
    ...     }
    ... }, {
    ...     $group: {
    ...         _id: {
    ...             $dateToString: {
    ...                 format: "%Y-%m-%d %H",
    ...                 date: "$recorded",
    ...                 timezone: "America/New_York"
    ...             }
    ...         },
    ...         min: { $min: "$reading" },
    ...         max: { $max: "$reading" },
    ...         avg: { $avg: "$reading" }
    ...     }
    ... }, {
    ...     $sort: { _id: 1 }
    ... }])
    { "_id" : "2018-12-10 00", "min" : 60.62, "max" : 60.8, "avg" : 60.74 }
    { "_id" : "2018-12-10 01", "min" : 60.44, "max" : 60.44, "avg" : 60.44 }
    { "_id" : "2018-12-10 02", "min" : 60.08, "max" : 60.44, "avg" : 60.185 }
    { "_id" : "2018-12-10 03", "min" : 59.54, "max" : 60.08, "avg" : 59.81 }
    { "_id" : "2018-12-10 04", "min" : 59.18, "max" : 59.54, "avg" : 59.33428571428571 }
    { "_id" : "2018-12-10 05", "min" : 58.82, "max" : 59.18, "avg" : 58.97 }
    { "_id" : "2018-12-10 06", "min" : 58.64, "max" : 58.64, "avg" : 58.64000000000001 }
    { "_id" : "2018-12-10 07", "min" : 58.46, "max" : 58.64, "avg" : 58.61000000000001 }
    { "_id" : "2018-12-10 08", "min" : 58.1, "max" : 58.46, "avg" : 58.31 }
    { "_id" : "2018-12-10 09", "min" : 58.28, "max" : 61.7, "avg" : 59.6 }
    { "_id" : "2018-12-10 10", "min" : 62.96, "max" : 65.3, "avg" : 64.2425 }
    { "_id" : "2018-12-10 11", "min" : 65.48, "max" : 68.36, "avg" : 67.02499999999999 }
    { "_id" : "2018-12-10 12", "min" : 68.36, "max" : 69.62, "avg" : 68.8775 }
    { "_id" : "2018-12-10 13", "min" : 69.8, "max" : 71.24, "avg" : 70.46 }
    { "_id" : "2018-12-10 14", "min" : 71.42, "max" : 72.32, "avg" : 71.765 }
    { "_id" : "2018-12-10 15", "min" : 71.6, "max" : 72.32, "avg" : 72.02 }
    { "_id" : "2018-12-10 16", "min" : 69.26, "max" : 71.42, "avg" : 70.49428571428571 }
    { "_id" : "2018-12-10 17", "min" : 66.92, "max" : 68.9, "avg" : 67.76 }
    { "_id" : "2018-12-10 18", "min" : 66.56, "max" : 67.1, "avg" : 66.83 }
    { "_id" : "2018-12-10 19", "min" : 65.66, "max" : 66.38, "avg" : 65.99 }
    Type "it" for more
    > 

Adding date and hour to the _id might make this clearer that we're grouping by hour.

    > db.sensorData.aggregate([{
    ...     $match: {
    ...         "measurement": "temperature",
    ...         "device": "office",
    ...         "recorded": {
    ...             $gte: new ISODate("2018-12-10T00:00:00-05:00"),
    ...             $lte: new ISODate("2019-12-11T00:00:00-05:00")
    ...         }
    ...     }
    ... }, {
    ...     $group: {
    ...         _id: {
    ...             date: {
    ...                 $dateToString: {
    ...                     format: "%Y-%m-%d",
    ...                     date: "$recorded",
    ...                     timezone: "America/New_York"
    ...                 }
    ...             },
    ...             hour: {
    ...                 $hour: {
    ...                     date: "$recorded",
    ...                     timezone: "America/New_York"
    ...                 }
    ...             }
    ...         },
    ... 
    ...         min: { $min: "$reading" },
    ...         max: { $max: "$reading" },
    ...         avg: { $avg: "$reading" }
    ...     }
    ... }, {
    ...     $sort: { _id: 1 }
    ... }])
    { "_id" : { "date" : "2018-12-10", "hour" : 0 }, "min" : 60.62, "max" : 60.8, "avg" : 60.74 }
    { "_id" : { "date" : "2018-12-10", "hour" : 1 }, "min" : 60.44, "max" : 60.44, "avg" : 60.44 }
    { "_id" : { "date" : "2018-12-10", "hour" : 2 }, "min" : 60.08, "max" : 60.44, "avg" : 60.185 }
    { "_id" : { "date" : "2018-12-10", "hour" : 3 }, "min" : 59.54, "max" : 60.08, "avg" : 59.81 }
    { "_id" : { "date" : "2018-12-10", "hour" : 4 }, "min" : 59.18, "max" : 59.54, "avg" : 59.33428571428571 }
    { "_id" : { "date" : "2018-12-10", "hour" : 5 }, "min" : 58.82, "max" : 59.18, "avg" : 58.97 }
    { "_id" : { "date" : "2018-12-10", "hour" : 6 }, "min" : 58.64, "max" : 58.64, "avg" : 58.64000000000001 }
    { "_id" : { "date" : "2018-12-10", "hour" : 7 }, "min" : 58.46, "max" : 58.64, "avg" : 58.61000000000001 }
    { "_id" : { "date" : "2018-12-10", "hour" : 8 }, "min" : 58.1, "max" : 58.46, "avg" : 58.31 }
    { "_id" : { "date" : "2018-12-10", "hour" : 9 }, "min" : 58.28, "max" : 61.7, "avg" : 59.6 }
    { "_id" : { "date" : "2018-12-10", "hour" : 10 }, "min" : 62.96, "max" : 65.3, "avg" : 64.2425 }
    { "_id" : { "date" : "2018-12-10", "hour" : 11 }, "min" : 65.48, "max" : 68.36, "avg" : 67.02499999999999 }
    { "_id" : { "date" : "2018-12-10", "hour" : 12 }, "min" : 68.36, "max" : 69.62, "avg" : 68.8775 }
    { "_id" : { "date" : "2018-12-10", "hour" : 13 }, "min" : 69.8, "max" : 71.24, "avg" : 70.46 }
    { "_id" : { "date" : "2018-12-10", "hour" : 14 }, "min" : 71.42, "max" : 72.32, "avg" : 71.765 }
    { "_id" : { "date" : "2018-12-10", "hour" : 15 }, "min" : 71.6, "max" : 72.32, "avg" : 72.02 }
    { "_id" : { "date" : "2018-12-10", "hour" : 16 }, "min" : 69.26, "max" : 71.42, "avg" : 70.49428571428571 }
    { "_id" : { "date" : "2018-12-10", "hour" : 17 }, "min" : 66.92, "max" : 68.9, "avg" : 67.76 }
    { "_id" : { "date" : "2018-12-10", "hour" : 18 }, "min" : 66.56, "max" : 67.1, "avg" : 66.83 }
    { "_id" : { "date" : "2018-12-10", "hour" : 19 }, "min" : 65.66, "max" : 66.38, "avg" : 65.99 }
    Type "it" for more
    > 

We can also calculate values for a smaller time frame like 15 minutes. To bucket values into 15 minute increments, we convert the datetime to a long integer, which is milliseconds since epoch. Calculate the [mod](https://en.wikipedia.org/wiki/Modulo_operation) of this value to our interval and subtract from the datetime. 

We did this same operation in Postgres using integer division. Subtracting mod is a different way to get the same results.

    > db.sensorData.aggregate([
    ...   { $match: 
    ...      { 
    ...        "measurement": "temperature",
    ...        "recorded" : { 
    ...                $gte: new ISODate("2018-12-10T12:00:00-05:00"),
    ...                $lte: new ISODate("2018-12-10T14:00:00-05:00") 
    ...        }
    ...      } 
    ...   },
    ...   { "$group": {
    ...     "_id": { "date": {
    ...       "$toDate": {
    ...         "$subtract": [
    ...           { "$toLong": "$recorded" },
    ...           { "$mod": [ { "$toLong": "$recorded" }, 1000 * 60 * 15 ] }
    ...         ]
    ...       }},
    ...       device: "$device"
    ...     },
    ...     max: { $max: "$reading" },
    ...     min: { $min: "$reading" },
    ...     avg: { $avg: "$reading" },
    ...     count: { "$sum": 1 }
    ...   }}
    ... ])
    { "_id" : { "date" : ISODate("2018-12-10T18:45:00Z"), "device" : "woodstove" }, "max" : 74.84, "min" : 74.48, "avg" : 74.66000000000001, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T18:45:00Z"), "device" : "outside" }, "max" : 35.24, "min" : 34.88, "avg" : 35.06, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T18:45:00Z"), "device" : "basement" }, "max" : 52, "min" : 51.9, "avg" : 51.96666666666667, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T18:45:00Z"), "device" : "office" }, "max" : 71.24, "min" : 71.06, "avg" : 71.12, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T18:30:00Z"), "device" : "livingroom" }, "max" : 69.62, "min" : 69.62, "avg" : 69.62, "count" : 1 }
    { "_id" : { "date" : ISODate("2018-12-10T18:30:00Z"), "device" : "rootcellar" }, "max" : 37.9, "min" : 37.9, "avg" : 37.9, "count" : 1 }
    { "_id" : { "date" : ISODate("2018-12-10T17:30:00Z"), "device" : "woodstove" }, "max" : 75.2, "min" : 75.2, "avg" : 75.2, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T18:15:00Z"), "device" : "outside" }, "max" : 34.7, "min" : 34.34, "avg" : 34.46, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:30:00Z"), "device" : "outside" }, "max" : 33.44, "min" : 33.26, "avg" : 33.32, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:15:00Z"), "device" : "rootcellar" }, "max" : 37.9, "min" : 37.9, "avg" : 37.9, "count" : 1 }
    { "_id" : { "date" : ISODate("2018-12-10T17:15:00Z"), "device" : "office" }, "max" : 68.9, "min" : 68.72, "avg" : 68.81, "count" : 2 }
    { "_id" : { "date" : ISODate("2018-12-10T17:30:00Z"), "device" : "basement" }, "max" : 52.4, "min" : 52.3, "avg" : 52.333333333333336, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:30:00Z"), "device" : "slab" }, "max" : 61.7, "min" : 61.7, "avg" : 61.70000000000001, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:15:00Z"), "device" : "livingroom" }, "max" : 66.74, "min" : 66.2, "avg" : 66.5, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:00:00Z"), "device" : "basement" }, "max" : 52, "min" : 51.9, "avg" : 51.96666666666667, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:00:00Z"), "device" : "livingroom" }, "max" : 65.66, "min" : 64.94, "avg" : 65.3, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:00:00Z"), "device" : "office" }, "max" : 68.54, "min" : 68.36, "avg" : 68.48, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:00:00Z"), "device" : "slab" }, "max" : 61.88, "min" : 61.88, "avg" : 61.88, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T18:45:00Z"), "device" : "rootcellar" }, "max" : 38, "min" : 37.9, "avg" : 37.95, "count" : 2 }
    { "_id" : { "date" : ISODate("2018-12-10T17:15:00Z"), "device" : "outside" }, "max" : 33.08, "min" : 32.9, "avg" : 33.02, "count" : 3 }
    Type "it" for more
    > 

The `$in` function can be used to limited which devices match our query.

    > db.sensorData.aggregate([
    ...   { $match: 
    ...      { 
    ...        "measurement": "temperature",
    ...        "device": { $in: ["office", "livingroom", "slab"]},
    ...        "recorded" : { 
    ...                $gte: new ISODate("2018-12-10T12:00:00-05:00"),
    ...                $lte: new ISODate("2018-12-10T14:00:00-05:00") 
    ...        }
    ...      } 
    ...   },
    ...   { "$group": {
    ...     "_id": { "date": {
    ...       "$toDate": {
    ...         "$subtract": [
    ...           { "$toLong": "$recorded" },
    ...           { "$mod": [ { "$toLong": "$recorded" }, 1000 * 60 * 15 ] }
    ...         ]
    ...       }},
    ...       device: "$device"
    ...     },
    ...     max: { $max: "$reading" },
    ...     min: { $min: "$reading" },
    ...     avg: { $avg: "$reading" },
    ...     count: { "$sum": 1 }
    ...   }},
    ...   { "$sort": { "_id": 1 } }
    ... ])
    { "_id" : { "date" : ISODate("2018-12-10T17:00:00Z"), "device" : "livingroom" }, "max" : 65.66, "min" : 64.94, "avg" : 65.3, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:00:00Z"), "device" : "office" }, "max" : 68.54, "min" : 68.36, "avg" : 68.48, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:00:00Z"), "device" : "slab" }, "max" : 61.88, "min" : 61.88, "avg" : 61.88, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:15:00Z"), "device" : "livingroom" }, "max" : 66.74, "min" : 66.2, "avg" : 66.5, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:15:00Z"), "device" : "office" }, "max" : 68.9, "min" : 68.72, "avg" : 68.81, "count" : 2 }
    { "_id" : { "date" : ISODate("2018-12-10T17:15:00Z"), "device" : "slab" }, "max" : 61.7, "min" : 61.52, "avg" : 61.580000000000005, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:30:00Z"), "device" : "livingroom" }, "max" : 67.46, "min" : 66.92, "avg" : 67.22, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:30:00Z"), "device" : "office" }, "max" : 69.26, "min" : 69.08, "avg" : 69.17, "count" : 2 }
    { "_id" : { "date" : ISODate("2018-12-10T17:30:00Z"), "device" : "slab" }, "max" : 61.7, "min" : 61.7, "avg" : 61.70000000000001, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:45:00Z"), "device" : "livingroom" }, "max" : 68.18, "min" : 67.82, "avg" : 68, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T17:45:00Z"), "device" : "office" }, "max" : 69.62, "min" : 69.62, "avg" : 69.62, "count" : 1 }
    { "_id" : { "date" : ISODate("2018-12-10T17:45:00Z"), "device" : "slab" }, "max" : 61.88, "min" : 61.7, "avg" : 61.76, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T18:00:00Z"), "device" : "livingroom" }, "max" : 68.54, "min" : 68.36, "avg" : 68.45, "count" : 2 }
    { "_id" : { "date" : ISODate("2018-12-10T18:00:00Z"), "device" : "office" }, "max" : 69.98, "min" : 69.8, "avg" : 69.86, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T18:00:00Z"), "device" : "slab" }, "max" : 61.7, "min" : 61.7, "avg" : 61.7, "count" : 2 }
    { "_id" : { "date" : ISODate("2018-12-10T18:15:00Z"), "device" : "livingroom" }, "max" : 69.26, "min" : 68.9, "avg" : 69.08000000000001, "count" : 2 }
    { "_id" : { "date" : ISODate("2018-12-10T18:15:00Z"), "device" : "office" }, "max" : 70.34, "min" : 69.98, "avg" : 70.16000000000001, "count" : 3 }
    { "_id" : { "date" : ISODate("2018-12-10T18:15:00Z"), "device" : "slab" }, "max" : 61.7, "min" : 61.7, "avg" : 61.7, "count" : 1 }
    { "_id" : { "date" : ISODate("2018-12-10T18:30:00Z"), "device" : "livingroom" }, "max" : 69.62, "min" : 69.62, "avg" : 69.62, "count" : 1 }
    { "_id" : { "date" : ISODate("2018-12-10T18:30:00Z"), "device" : "office" }, "max" : 70.88, "min" : 70.52, "avg" : 70.7, "count" : 3 }
    Type "it" for more
    > 

A projection can be used to limit the fields that appear in the results.

    > db.sensorData.aggregate([
    ...   { $match: 
    ...      { 
    ...        "measurement": "temperature",
    ...        "device": { $in: ["office", "livingroom", "slab"]},
    ...        "recorded" : { 
    ...                $gte: new ISODate("2018-12-10T12:00:00-05:00"),
    ...                $lte: new ISODate("2018-12-10T14:00:00-05:00") 
    ...        }
    ...      } 
    ...   },
    ...   { "$group": {
    ...     "_id": { "date": {
    ...       "$toDate": {
    ...         "$subtract": [
    ...           { "$toLong": "$recorded" },
    ...           { "$mod": [ { "$toLong": "$recorded" }, 1000 * 60 * 15 ] }
    ...         ]
    ...       }},
    ...       device: "$device"
    ...     },
    ...     max: { $max: "$reading" },
    ...     min: { $min: "$reading" },
    ...     avg: { $avg: "$reading" },
    ...     count: { "$sum": 1 }
    ...   }},
    ...   { "$sort": { "_id": 1 } },
    ...   {
    ...     $project: {
    ...       device: 1, min: 1, max: 1
    ...     }
    ...   }
    ... ])
    { "_id" : { "date" : ISODate("2018-12-10T17:00:00Z"), "device" : "livingroom" }, "max" : 65.66, "min" : 64.94 }
    { "_id" : { "date" : ISODate("2018-12-10T17:00:00Z"), "device" : "office" }, "max" : 68.54, "min" : 68.36 }
    { "_id" : { "date" : ISODate("2018-12-10T17:00:00Z"), "device" : "slab" }, "max" : 61.88, "min" : 61.88 }
    { "_id" : { "date" : ISODate("2018-12-10T17:15:00Z"), "device" : "livingroom" }, "max" : 66.74, "min" : 66.2 }
    { "_id" : { "date" : ISODate("2018-12-10T17:15:00Z"), "device" : "office" }, "max" : 68.9, "min" : 68.72 }
    { "_id" : { "date" : ISODate("2018-12-10T17:15:00Z"), "device" : "slab" }, "max" : 61.7, "min" : 61.52 }
    { "_id" : { "date" : ISODate("2018-12-10T17:30:00Z"), "device" : "livingroom" }, "max" : 67.46, "min" : 66.92 }
    { "_id" : { "date" : ISODate("2018-12-10T17:30:00Z"), "device" : "office" }, "max" : 69.26, "min" : 69.08 }
    { "_id" : { "date" : ISODate("2018-12-10T17:30:00Z"), "device" : "slab" }, "max" : 61.7, "min" : 61.7 }
    { "_id" : { "date" : ISODate("2018-12-10T17:45:00Z"), "device" : "livingroom" }, "max" : 68.18, "min" : 67.82 }
    { "_id" : { "date" : ISODate("2018-12-10T17:45:00Z"), "device" : "office" }, "max" : 69.62, "min" : 69.62 }
    { "_id" : { "date" : ISODate("2018-12-10T17:45:00Z"), "device" : "slab" }, "max" : 61.88, "min" : 61.7 }
    { "_id" : { "date" : ISODate("2018-12-10T18:00:00Z"), "device" : "livingroom" }, "max" : 68.54, "min" : 68.36 }
    { "_id" : { "date" : ISODate("2018-12-10T18:00:00Z"), "device" : "office" }, "max" : 69.98, "min" : 69.8 }
    { "_id" : { "date" : ISODate("2018-12-10T18:00:00Z"), "device" : "slab" }, "max" : 61.7, "min" : 61.7 }
    { "_id" : { "date" : ISODate("2018-12-10T18:15:00Z"), "device" : "livingroom" }, "max" : 69.26, "min" : 68.9 }
    { "_id" : { "date" : ISODate("2018-12-10T18:15:00Z"), "device" : "office" }, "max" : 70.34, "min" : 69.98 }
    { "_id" : { "date" : ISODate("2018-12-10T18:15:00Z"), "device" : "slab" }, "max" : 61.7, "min" : 61.7 }
    { "_id" : { "date" : ISODate("2018-12-10T18:30:00Z"), "device" : "livingroom" }, "max" : 69.62, "min" : 69.62 }
    { "_id" : { "date" : ISODate("2018-12-10T18:30:00Z"), "device" : "office" }, "max" : 70.88, "min" : 70.52 }
    Type "it" for more
    > 

The _id can be hidden in the result set. Here _id.device and _id.date are moved to the top level of the results. The date string is formatted in EST.

    > db.sensorData.aggregate([
    ...   { $match: 
    ...      { 
    ...        "measurement": "temperature",
    ...        "device": { $in: ["office", "livingroom", "slab"]},
    ...        "recorded" : { 
    ...                $gte: new ISODate("2018-12-10T12:00:00-05:00"),
    ...                $lte: new ISODate("2018-12-10T14:00:00-05:00") 
    ...        }
    ...      } 
    ...   },
    ...   { "$group": {
    ...     "_id": { "date": {
    ...       "$toDate": {
    ...         "$subtract": [
    ...           { "$toLong": "$recorded" },
    ...           { "$mod": [ { "$toLong": "$recorded" }, 1000 * 60 * 15 ] }
    ...         ]
    ...       }},
    ...       device: "$device"
    ...     },
    ...     max: { $max: "$reading" },
    ...     min: { $min: "$reading" },
    ...     avg: { $avg: "$reading" },
    ...     count: { "$sum": 1 }
    ...   }},
    ...   { "$sort": { "_id": 1 } },
    ...   {
    ...     $project: {
    ...       _id: 0,
    ...       date: {$dateToString: { date: "$_id.date", format: "%Y-%m-%d %H:%M", timezone: "America/New_York" } },
    ...       device: "$_id.device", min: 1, max: 1
    ...     }
    ...   }
    ... ]) 
    { "max" : 65.66, "min" : 64.94, "date" : "2018-12-10 12:00", "device" : "livingroom" }
    { "max" : 68.54, "min" : 68.36, "date" : "2018-12-10 12:00", "device" : "office" }
    { "max" : 61.88, "min" : 61.88, "date" : "2018-12-10 12:00", "device" : "slab" }
    { "max" : 66.74, "min" : 66.2, "date" : "2018-12-10 12:15", "device" : "livingroom" }
    { "max" : 68.9, "min" : 68.72, "date" : "2018-12-10 12:15", "device" : "office" }
    { "max" : 61.7, "min" : 61.52, "date" : "2018-12-10 12:15", "device" : "slab" }
    { "max" : 67.46, "min" : 66.92, "date" : "2018-12-10 12:30", "device" : "livingroom" }
    { "max" : 69.26, "min" : 69.08, "date" : "2018-12-10 12:30", "device" : "office" }
    { "max" : 61.7, "min" : 61.7, "date" : "2018-12-10 12:30", "device" : "slab" }
    { "max" : 68.18, "min" : 67.82, "date" : "2018-12-10 12:45", "device" : "livingroom" }
    { "max" : 69.62, "min" : 69.62, "date" : "2018-12-10 12:45", "device" : "office" }
    { "max" : 61.88, "min" : 61.7, "date" : "2018-12-10 12:45", "device" : "slab" }
    { "max" : 68.54, "min" : 68.36, "date" : "2018-12-10 13:00", "device" : "livingroom" }
    { "max" : 69.98, "min" : 69.8, "date" : "2018-12-10 13:00", "device" : "office" }
    { "max" : 61.7, "min" : 61.7, "date" : "2018-12-10 13:00", "device" : "slab" }
    { "max" : 69.26, "min" : 68.9, "date" : "2018-12-10 13:15", "device" : "livingroom" }
    { "max" : 70.34, "min" : 69.98, "date" : "2018-12-10 13:15", "device" : "office" }
    { "max" : 61.7, "min" : 61.7, "date" : "2018-12-10 13:15", "device" : "slab" }
    { "max" : 69.62, "min" : 69.62, "date" : "2018-12-10 13:30", "device" : "livingroom" }
    { "max" : 70.88, "min" : 70.52, "date" : "2018-12-10 13:30", "device" : "office" }
    Type "it" for more
    > 

Same query as above, but sorted by device then date.

    > db.sensorData.aggregate([
    ...   { $match: 
    ...      { 
    ...        "measurement": "temperature",
    ...        "device": { $in: ["office", "livingroom", "slab"]},
    ...        "recorded" : { 
    ...                $gte: new ISODate("2018-12-10T12:00:00-05:00"),
    ...                $lte: new ISODate("2018-12-10T14:00:00-05:00") 
    ...        }
    ...      } 
    ...   },
    ...   { "$group": {
    ...     "_id": { "date": {
    ...       "$toDate": {
    ...         "$subtract": [
    ...           { "$toLong": "$recorded" },
    ...           { "$mod": [ { "$toLong": "$recorded" }, 1000 * 60 * 15 ] }
    ...         ]
    ...       }},
    ...       device: "$device"
    ...     },
    ...     max: { $max: "$reading" },
    ...     min: { $min: "$reading" },
    ...     avg: { $avg: "$reading" },
    ...     count: { "$sum": 1 }
    ...   }},
    ...   {
    ...     $project: {
    ...       _id: 0,
    ...       date: { $dateToString: { date: "$_id.date", format: "%Y-%m-%d %H:%M", timezone: "America/New_York" } },
    ...       device: "$_id.device", min: 1, max: 1
    ...     }
    ...   },
    ...   {
    ...     $sort: {
    ...       device: 1, date: 1
    ...     }
    ...   }
    ... ]) 
    { "max" : 65.66, "min" : 64.94, "date" : "2018-12-10 12:00", "device" : "livingroom" }
    { "max" : 66.74, "min" : 66.2, "date" : "2018-12-10 12:15", "device" : "livingroom" }
    { "max" : 67.46, "min" : 66.92, "date" : "2018-12-10 12:30", "device" : "livingroom" }
    { "max" : 68.18, "min" : 67.82, "date" : "2018-12-10 12:45", "device" : "livingroom" }
    { "max" : 68.54, "min" : 68.36, "date" : "2018-12-10 13:00", "device" : "livingroom" }
    { "max" : 69.26, "min" : 68.9, "date" : "2018-12-10 13:15", "device" : "livingroom" }
    { "max" : 69.62, "min" : 69.62, "date" : "2018-12-10 13:30", "device" : "livingroom" }
    { "max" : 69.98, "min" : 69.8, "date" : "2018-12-10 13:45", "device" : "livingroom" }
    { "max" : 68.54, "min" : 68.36, "date" : "2018-12-10 12:00", "device" : "office" }
    { "max" : 68.9, "min" : 68.72, "date" : "2018-12-10 12:15", "device" : "office" }
    { "max" : 69.26, "min" : 69.08, "date" : "2018-12-10 12:30", "device" : "office" }
    { "max" : 69.62, "min" : 69.62, "date" : "2018-12-10 12:45", "device" : "office" }
    { "max" : 69.98, "min" : 69.8, "date" : "2018-12-10 13:00", "device" : "office" }
    { "max" : 70.34, "min" : 69.98, "date" : "2018-12-10 13:15", "device" : "office" }
    { "max" : 70.88, "min" : 70.52, "date" : "2018-12-10 13:30", "device" : "office" }
    { "max" : 71.24, "min" : 71.06, "date" : "2018-12-10 13:45", "device" : "office" }
    { "max" : 61.88, "min" : 61.88, "date" : "2018-12-10 12:00", "device" : "slab" }
    { "max" : 61.7, "min" : 61.52, "date" : "2018-12-10 12:15", "device" : "slab" }
    { "max" : 61.7, "min" : 61.7, "date" : "2018-12-10 12:30", "device" : "slab" }
    { "max" : 61.88, "min" : 61.7, "date" : "2018-12-10 12:45", "device" : "slab" }
    Type "it" for more
    > 

MongoDB determines the order of the columns in the results. Normally this doesn't matter since it's JSON. You can use `$replaceRoot` to set the field order.

    > db.sensorData.aggregate([
    ...   { $match: 
    ...      { 
    ...        "measurement": "temperature",
    ...        "device": { $in: ["office", "livingroom", "slab"]},
    ...        "recorded" : { 
    ...                $gte: new ISODate("2018-12-10T12:00:00-05:00"),
    ...                $lte: new ISODate("2018-12-10T14:00:00-05:00") 
    ...        }
    ...      } 
    ...   },
    ...   { "$group": {
    ...     "_id": { "date": {
    ...       "$toDate": {
    ...         "$subtract": [
    ...           { "$toLong": "$recorded" },
    ...           { "$mod": [ { "$toLong": "$recorded" }, 1000 * 60 * 15 ] }
    ...         ]
    ...       }},
    ...       device: "$device"
    ...     },
    ...     max: { $max: "$reading" },
    ...     min: { $min: "$reading" },
    ...     avg: { $avg: "$reading" },
    ...     count: { "$sum": 1 }
    ...   }},
    ...   {
    ...     $replaceRoot: {
    ...       newRoot: {
    ...         date: { $dateToString: { date: "$_id.date", format: "%Y-%m-%d %H:%M %z", timezone: "America/New_York" } },
    ...         device: "$_id.device", max: "$max", min: "$min"
    ...       }
    ...     }
    ...   },
    ...   {
    ...     $sort: {
    ...       date: 1, device: 1
    ...     }
    ...   }
    ... ])
    { "date" : "2018-12-10 12:00 -0500", "device" : "livingroom", "max" : 65.66, "min" : 64.94 }
    { "date" : "2018-12-10 12:00 -0500", "device" : "office", "max" : 68.54, "min" : 68.36 }
    { "date" : "2018-12-10 12:00 -0500", "device" : "slab", "max" : 61.88, "min" : 61.88 }
    { "date" : "2018-12-10 12:15 -0500", "device" : "livingroom", "max" : 66.74, "min" : 66.2 }
    { "date" : "2018-12-10 12:15 -0500", "device" : "office", "max" : 68.9, "min" : 68.72 }
    { "date" : "2018-12-10 12:15 -0500", "device" : "slab", "max" : 61.7, "min" : 61.52 }
    { "date" : "2018-12-10 12:30 -0500", "device" : "livingroom", "max" : 67.46, "min" : 66.92 }
    { "date" : "2018-12-10 12:30 -0500", "device" : "office", "max" : 69.26, "min" : 69.08 }
    { "date" : "2018-12-10 12:30 -0500", "device" : "slab", "max" : 61.7, "min" : 61.7 }
    { "date" : "2018-12-10 12:45 -0500", "device" : "livingroom", "max" : 68.18, "min" : 67.82 }
    { "date" : "2018-12-10 12:45 -0500", "device" : "office", "max" : 69.62, "min" : 69.62 }
    { "date" : "2018-12-10 12:45 -0500", "device" : "slab", "max" : 61.88, "min" : 61.7 }
    { "date" : "2018-12-10 13:00 -0500", "device" : "livingroom", "max" : 68.54, "min" : 68.36 }
    { "date" : "2018-12-10 13:00 -0500", "device" : "office", "max" : 69.98, "min" : 69.8 }
    { "date" : "2018-12-10 13:00 -0500", "device" : "slab", "max" : 61.7, "min" : 61.7 }
    { "date" : "2018-12-10 13:15 -0500", "device" : "livingroom", "max" : 69.26, "min" : 68.9 }
    { "date" : "2018-12-10 13:15 -0500", "device" : "office", "max" : 70.34, "min" : 69.98 }
    { "date" : "2018-12-10 13:15 -0500", "device" : "slab", "max" : 61.7, "min" : 61.7 }
    { "date" : "2018-12-10 13:30 -0500", "device" : "livingroom", "max" : 69.62, "min" : 69.62 }
    { "date" : "2018-12-10 13:30 -0500", "device" : "office", "max" : 70.88, "min" : 70.52 }
    Type "it" for more
    > 

Consult the [MongoDB manual](https://docs.mongodb.com/manual/reference/operator/aggregation/) for more ways to build aggregation pipelines.

&copy; 2019 Don Coleman