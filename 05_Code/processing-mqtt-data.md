# Processing MQTT data with Node.js

   * [Processing MQTT data with Node.js](#processing-mqtt-data-with-nodejs)
      * [Create a Node project](#create-a-node-project)
      * [Environment](#environment)
      * [Read messages from MQTT](#read-messages-from-mqtt)
      * [Log MQTT messages to a File](#log-mqtt-messages-to-a-file)
   * [Write MQTT messages to SQLite](#write-mqtt-messages-to-a-sqlite-database)
      * [SQLite driver](#sqlite-driver)
      * [Database Tables](#database-tables)
      * [Inserting records with Node.js](#inserting-records-with-nodejs)
      * [Save MQTT messages to SQLite](#save-mqtt-messages-to-sqlite)
   * [High Temperature Alert](#high-temperature-alert)
      * [Test data](#test-data)
      * [SMS](#sms)
   * [PM2](#pm2)
   
## Create a Node project

We are going to create a Node.js project with code to read and process data from MQTT topics.

Open `Terminal.app` on Mac, or PowerShell on Windows and create new directory for this project

    mkdir process-mqtt-data
    cd process-mqtt-data

Use `npm` to initialize the project

    npm init -y

Install the mqtt and dotenv libraries

    npm install mqtt dotenv

Open the `process-mqtt-data` directory in [Visual Studio Code](https://code.visualstudio.com), Sublime, or your preferred text editor.

## Environment

We store the configuration for the application in a hidden file named `.env`. That is a dot (or period) followed by env, meaning environment. Files that begin with '.' are sometimes tricky to deal since they are hidden in Windows Exporer and in Finder on macOS.

Create a file name `.env` in your text editor. Since this file contains usernames and passwords it should *not* be commited to version control. Add `.env` to your `.gitignore` file. The contents of your file should look like this.

    MQTT_SERVER=mqtts://user:password@dev2db.com

Replace `user` and `password` with your username and password for the MQTT server.

The contents of `.env` are loaded into the environment by the [dotenv](https://www.npmjs.com/package/dotenv) package.

## Read messages from MQTT

Let's start by reading data from MQTT and printing it to standard output. Create a new file `mqtt-test.js` and enter the following contents

    require('dotenv').config();
    const mqtt = require('mqtt');

    const mqttClient = mqtt.connect(process.env.MQTT_SERVER);

    mqttClient.on('connect', () => {
        console.log('MQTT Connected');
        mqttClient.subscribe('#');
    });

    mqttClient.on('message', (topic, message) => {
        console.log(topic, message.toString());
    });

    mqttClient.on('error', (error) => {
        console.log(error);
    });

Run the `mqtt-test` and ensure you see MQTT messages printed to the console.

    node mqtt-test.js

Sample output

    itp/device_12/temperature 65.30
    itp/device_12/humidity 40.30
    itp/device_14/temperature 80.42
    itp/device_14/humidity 19.00
    itp/device_06/temperature 44.60
    itp/device_06/humidity 81.40

## Log MQTT messages to a File

Now that we can read MQTT messages, let's write these messages to a file instead of just printing data to the console. Create a new file `mqtt-logger.js`. Copy the code from `mqtt-test.js` and paste it into the `mqtt-logger.js` file.

Add the following code to line 2 of `mqtt-logger.js`, after the `require('dotenv').config();` line. Ths code imports the [Node.js File System library](https://nodejs.org/dist/latest-v17.x/docs/api/fs.html) `fs` and creates a variable to hold the file name of the log file.

    const fs = require('fs');
    const fileName = './mqtt.log';

Replace the existing on message handler code with this new function

    mqttClient.on('message', function (topic, message) {        
        const timestamp = new Date().getTime();
        const data = `${timestamp}\t${topic}\t${message}\n`;
        fs.writeFileSync(fileName, data, { flag: 'a' });
    });

The [getTime](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/getTime) method of Date gets the timestamp from the date in milliseconds since epoch. A tab-delimited string is created using the timestamp, topic, and message. The string is written to the log file with the append flag. Without the append flag, the new data would overwrite the old file.

Your `mqtt-logger.js` file should look like this

    require('dotenv').config();
    const fs = require('fs');
    const fileName = './mqtt.log';
    const mqtt = require('mqtt');
    const mqttClient = mqtt.connect(process.env.MQTT_SERVER);

    mqttClient.on('connect', () => {
        console.log('MQTT Connected');
        mqttClient.subscribe('#');
    });

    mqttClient.on('message', function (topic, message) {        
        const timestamp = new Date().getTime();
        const data = `${timestamp}\t${topic}\t${message}\n`;
        fs.writeFileSync(fileName, data, { flag: 'a' });
    });

    mqttClient.on('error', (error) => {
        console.log(error);
    });

Start the process

    node mqtt-logger.js

Open a 2nd terminal window and tail the MQTT log to ensure data is being written correctly.

    tail -f mqtt.log

Windows users should use PowerShell to tail mqtt.log

    Get-Content mqtt.log -Wait

Sample output

    1614407069930   itp/device_06/temperature       44.60
    1614407069947   itp/device_06/humidity  81.70
    1614407069947   itp/device_16/temperature       73.22
    1614407069947   itp/device_16/humidity  27.10
    1614407073207   itp/device_23/temperature       80.42
    1614407073225   itp/device_23/humidity  33.00
    1614407073248   itp/device_12/temperature       65.30
    1614407073264   itp/device_12/humidity  40.50

### Enhancement

Note that the logger will fail if your MQTT message contains tabs or linefeeds. (This might happen with JSON payloads.) You can add additional code to handle edge cases. Be sure to use payload instead of message when creating the data string.

    mqttClient.on('message', (topic, message) => {
        console.log(topic, message.toString());
        let payload = message.toString();
        // replace tabs with 2 spaces
        payload = payload.replace(/\t/g, ' ');
        // remove carriage returns
        payload = payload.replace(/\r/g, '');
        // replace line feeds with one space
        payload = payload.replace(/\n/g, ' ');
        const timestamp = new Date().getTime();
        const data = `${timestamp}\t${topic}\t${payload}\n`;
        fs.writeFileSync(fileName, data, { flag: 'a' });
    });

There are also [many TSV and CSV libraries](https://www.npmjs.com/search?q=tsv) available for Node.js that can make reading and writing delimited files easier.

# Write MQTT messages to a SQLite Database

We can use Node.js to save MQTT message into a database. This example uses SQLite, but you could modify it to write to PostgreSQL, TimescaleDB, or InfluxDB.

## SQLite driver

Install the [better-sqlite3](https://www.npmjs.com/package/better-sqlite3) driver with npm.

    npm install better-sqlite3

## Database Tables

We need two database tables to hold the data from MQTT. One for numeric data and one for text data. Most of the data like temperature and humidity will be numeric, but some data like turning the LED on and off and hex values from the color sensor will text. 

Create a new file `schema.sql` with the following contents:

    -- table for numeric mqtt data
    CREATE TABLE IF NOT EXISTS sensor_data (
        device TEXT,
        measurement TEXT,
        reading NUMERIC(9, 2),
        recorded_at TIMESTAMP NOT NULL default current_timestamp
    );

    -- table for text mqtt data
    CREATE TABLE IF NOT EXISTS sensor_data_text (
        device TEXT,
        measurement TEXT,
        reading TEXT,
        recorded_at TIMESTAMP NOT NULL default current_timestamp
    );

Notice that the create table statements use `CREATE TABLE IF NOT EXISTS`. This allows us to run the SQL script many time and only create the tables if they are missing.

## Inserting records with Node.js

Create a new file `insert-test.js`. This is a simple program to make sure we can insert data into the database from Node.js. Require the libraries, then open the database `example.db` with verbose logging. The first time this is run the database doesn't exist, so it will be created.

    const fs = require('fs');
    const Database = require('better-sqlite3');
    const db = new Database('example.db', { verbose: console.log });

Read the schema.sql file, and run it to create the tables in the database.

    // read the database schema from schema.sql and and execute it
    const migration = fs.readFileSync('schema.sql', 'utf8');
    db.exec(migration);

Insert some test data using a prepared statement. The query in the prepared statement uses question marks as placeholders. The parameters are passed to the prepared statement when the query is run using `stmt.run`. Always use placeholders when builing queries in code. This allows the database driver to sanitize the input. Never build queries by concatenating strings because it puts you at risk for SQL Injection attacks.

    // insert some test data using a prepared statement
    const stmt = db.prepare('INSERT INTO sensor_data (device, measurement, reading) VALUES (?, ?, ?)');
    stmt.run('device_test', 'acceleration', 9.82);

Select the data from sensor_data and print it out to verify that our data was inserted.

    // select all the records from the sensor table
    const sensor_data = db.prepare('SELECT * FROM sensor_data').all();
    console.log(sensor_data);

Run the program 

    node insert-test.js 

Expected output

    -- table for numeric mqtt data
    CREATE TABLE IF NOT EXISTS sensor_data (
        device TEXT,
        measurement TEXT,
        reading NUMERIC(9, 2),
        recorded_at TIMESTAMP NOT NULL default current_timestamp
    );
    -- table for text mqtt data
    CREATE TABLE IF NOT EXISTS sensor_data_text (
        device TEXT,
        measurement TEXT,
        reading TEXT,
        recorded_at TIMESTAMP NOT NULL default current_timestamp
    );
    INSERT INTO sensor_data (device, measurement, reading) VALUES ('device_test', 'acceleration', 9.82)
    SELECT * FROM sensor_data
    [
        {
            device: 'device_test',
            measurement: 'acceleration',
            reading: 9.82,
            recorded_at: '2021-03-01 00:49:31'
        }
    ]

Use the sqlite3 command to verify that data was inserted into the database.

Open the database

    sqlite3 example.db

Run the query

    SELECT rowid, * FROM sensor_data;

Expected output 

    1|device_test|acceleration|9.82|2021-03-01 00:49:31

## Save MQTT messages to SQLite

Create a new file `mqtt-to-sqlite.js`. This code will save MQTT messages in SQLite using a combination of our code from `mqtt-test.js` and `insert-test.js`.

Include the MQTT and SQLite libraries,  Set up the database connection, schema, and prepared statements.  Set up the MQTT client connection.

    require('dotenv').config();
    const fs = require('fs');
    const mqtt = require('mqtt');
    const Database = require('better-sqlite3');
    const db = new Database('example.db');

    // read the database schema from schema.sql and and execute it
    const migration = fs.readFileSync('schema.sql', 'utf8');
    db.exec(migration);

    // SQL prepared statements
    const stmt_numeric_data = db.prepare('INSERT INTO sensor_data (device, measurement, reading) VALUES (?, ?, ?)');
    const stmt_text_data = db.prepare('INSERT INTO sensor_data_text (device, measurement, reading) VALUES (?, ?, ?)');

    const mqttClient = mqtt.connect(process.env.MQTT_SERVER);

Create the connect callback for MQTT. This function is called after the program connects to the MQTT broker. Adjust the subscription to match the devices you want to capture. Use specific topics or wildcards. You can also multiple subscriptions here. If you just want to capture your device, subscribe to `itp/device_XX/+`. (Replace `XX` with your device number.) To get everyone's data, subscribe to `itp/+/+`. 

    mqttClient.on('connect', async () => {
        console.log('MQTT connected');
        mqttClient.subscribe('itp/+/+');
        mqttClient.publish(`presence/connected/${process.env.USER}`, 'mqtt-to-sqlite')
    });

The add the MQTT message callback. This function is called for every MQTT message that matches your subscription. The topic is split to separate the device and measurement information. The message payload contains the measurement reading value. The message parameter passed to the callback is a Buffer that contains the payload. In our case, it's a string. That string could be a number, like 72.3 or a string like '#615c3c'. We parse the reading to a floating point number. If this fails and returns not a number (NaN), we insert the data into the `sensor_data_text` table. Otherwise when we insert the data into the `sensor_data` table.

    mqttClient.on('message', (topic, message) => {
        console.log(topic, message.toString());

        // split the mqtt topic to get the device and measurement
        const parts = topic.split('/');
        const root = parts[0];          // root is always 'itp' and ignored
        const device = parts[1];
        const measurement = parts[2];
        const reading = message;

        if (isNaN(parseFloat(reading))) {   // the reading is text data
            stmt_text_data.run(device, measurement, reading);
        } else {  // the reading is numeric data
            stmt_numeric_data.run(device, measurement, reading);
        }

    });

Run the code

    node mqtt-to-sqlite

Open the database

    sqlite3 example.db

Optionally configure the output

    .mode columns
    .headers on

Query the database to ensure new records are being inserted

    SELECT * FROM sensor_data ORDER BY recorded_at desc;

    SELECT device, count(*) FROM sensor_data GROUP BY device;

Since records are constantly arriving you can write a query to show you records that are less than 30 seconds old

    SELECT rowid, * 
      FROM sensor_data 
      WHERE recorded_at > datetime('now', '-30 seconds') 
      ORDER BY recorded_at DESC;

Use datetime to show recorded_at in the local time zome

    SELECT rowid, device, measurement, reading, recorded_at, datetime(recorded_at, 'localtime') as local_time
      FROM sensor_data 
      WHERE recorded_at > datetime('now', '-30 seconds') 
      ORDER BY recorded_at DESC;


# High Temperature Alert

Code running on the server can be used to check MQTT data and send alerts when certain conditions are met. Create a new file `temperature-alert.js`. Require the mqtt library and create a connection to the MQTT server.

    require('dotenv').config();
    const mqtt = require('mqtt');
    const mqttClient = mqtt.connect(process.env.MQTT_SERVER);

Create a variable for the high temperature limit

    const limit = 80;

Create the phone number that should receive the SMS message when this limit is exceeded. For the US, you phone number must be 10 digits prefixed by the country code, 1. This *should* work for international numbers, if you include the country code. Replace 12155559999 with your number.

    const phoneNumber = '12125559999';

In the connect callback, subscribe to messages from a device. Since we're experimenting, subscribe to `test/device_XX/temperature`. Replace device_XX with your device name. We're using the test topic to keep test data out of the itp database.

    mqttClient.on('connect', () => {
        console.log('MQTT Connected');
        mqttClient.subscribe('test/device_XX/temperature');
    });

When a message is received, check to see if the value is over the limit and post a new message to the SMS topic.

    mqttClient.on('message', (topic, message) => {
        console.log(topic, message.toString());
        const device = topic.split('/')[1];
        const temperature = Number(message.toString());

        if (temperature > limit) {
            const alertMessage = `Temperature ${temperature}°F for device '${device}' exceeds the high limit of ${limit}°F`
            mqttClient.publish(`sms/send/${phoneNumber}`, alertMessage);
        }
    });

Run this code

    node temperature-alert.js

While this code is running it will post messages to the alert topic for your device when the temprature is over 80F. Another application could process those alerts and do something.

## Test Data

We can test this by sending some fake data to `test/device_XX/temperature`. Create a new file `publish.js` to publish some data to your topic. 

    require('dotenv').config();
    const mqtt = require('mqtt');
    const mqttClient = mqtt.connect(process.env.MQTT_SERVER);
    const temperature = process.argv[2] || '120';

    mqttClient.on('connect', () => {
        console.log('MQTT Connected');
        mqttClient.publish('test/device_XX/temperature', temperature);
        console.log('Sent message');
        mqttClient.end();
    });    

You should receive an SMS message with the alert.

![SMS Notification: "Temperature 120F exceeds the high temperature limit of 80F"](img/sms-notification.jpg)

## Listen for data from your device

Once your test is successful, you can change `temperature-alert.js` to listen for messages from your device.

        mqttClient.subscribe('test/device_XX/temperature');

Notice that the topic starts with **itp** instead of test. Be sure to update device_XX with your device number.

        mqttClient.subscribe('itp/device_XX/temperature');

## SMS

How did the SMS message get sent? There is a node script running on dev2db.com listening for messages on topic `sms/send/+`. It uses [Twilio](http://twilio.com/) to send the text messages. **You do NOT need to create a file with this code.** It is just included for reference.

    require('dotenv').config();
    const accountSid = process.env.TWILIO_ACCOUNT_SID;
    const authToken = process.env.TWILIO_AUTH_TOKEN;
    const client = require('twilio')(accountSid, authToken);

    const mqtt = require('mqtt');
    const mqttClient = mqtt.connect(process.env.MQTT_SERVER);

    mqttClient.on('connect', () => {
        console.log('MQTT Connected');
        mqttClient.subscribe('sms/send/+');
    });

    mqttClient.on('message', (topic, message)=> {
        console.log(topic, message.toString());
        const number = topic.split('/')[2];
        sendSMS(number, message);
    });

    function sendSMS(number, message) {
        client.messages
            .create({
                body: message,
                from: '+1718682XXXX',
                to: `+${number}`
            })
            .then(message => {
                console.log('success', number, message.sid)
                mqttClient.publish(`sms/success/${number}`, message.sid);
            })
            .catch(err => {
                console.log('error', number, err.message)
                mqttClient.publish(`sms/error/${number}`, err.message);
            });
    }

# PM2

The mqtt-logger and mqtt-to-sqlite processes will keep running as long as we keep the terminal window open, which works great for development but isn't very convenient for long runnning processes. [PM2](https://pm2.keymetrics.io/docs/usage/quick-start/) is a process manager that run the processes as daemons for us.

Install PM2 globally

    npm install -g pm2

NOTE: on macOS you might need to use

    sudo npm install -g pm2

Ensure the environment file is up to date, then start the processes with pm2. (For Windows cmd works better than PowerShell.)

    pm2 start mqtt-logger.js
    pm2 start mqtt-to-sqlite.js

 Starting processes keeps them running and will restart them if they crash. Use `pm2 list` to show process information.

![screenshot of pm2 list output](img/pm2-list.png)

Use `pm2 stop <id|name>` and `pm2 start <id|name>` to stop and start processes. PM2 saves the environment when the process was added. You can see the saved values with `pm2 env <id|name>`. `pm2 info` shows details about the process. Use `pm2 log` to see the console output from your scripts. Use the `--watch` flag with `pm2 start` to have PM2 restart the process when it detects the source files have been changed.

PM2 can restart processes when the machine boots. Install the startup hooks with `pm2 startup`. Save the process list with `pm2 save`. Remember to run `pm2 save` again when you add or change processes.

See the [PM2 documentation](https://pm2.keymetrics.io/docs/usage/pm2-doc-single-page/) for more details.
