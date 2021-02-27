# Processing MQTT data with Node.js

   * [Processing MQTT data with Node.js](#processing-mqtt-data-with-nodejs)
      * [Create a Node project](#create-a-node-project)
      * [Environment](#environment)
      * [Read messages from MQTT](#read-messages-from-mqtt)
      * [Log MQTT messages to a File](#log-mqtt-messages-to-a-file)
   * [Write MQTT messages to SQLite](#write-mqtt-messages-to-a-sqlite-database)
      * [SQLite driver](#sqlite-driver)
      * [Migrations](#migrations)
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

Open the `process-mqtt-data` directory in Visual Studio Code, Sublime, or your preferred text editor.

## Environment

Create a file name `.env` to store environment variables. Since this file contains usernames and passwords it should *not* be commited to version control. Add `.env` to your `.gitignore` file. Your file should look like this.

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

Now that we can read MQTT messages, let's write them to a file instead of just printing them to the console. Create a new file `mqtt-logger.js`. Copy the code from `mqtt-test.js` and paste it into the `mqtt-logger.js` file.

After the `require('dotenv').config();` line add the following code to, require `fs` and create a variable to hold the file name of the log file.

    const fs = require('fs');
    const fileName = './mqtt.log';

Replace the existing on messages handler with this new function

    mqttClient.on('message', function (topic, message) {        
        const timestamp = new Date().getTime();
        const data = `${timestamp}\t${topic}\t${message}\n`;
        fs.writeFileSync(fileName, data, { flag: 'a' });
    });

The [getTime](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/getTime) method get the timestamp from the date in milliseconds since epoch. A tab-delimited string is created using the timestamp, topic, and message. The string is written to the log file with the append flag. Without the append flag, the new data would overwrite the old file.

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

Sample output

    1614407069930   itp/device_06/temperature       44.60
    1614407069947   itp/device_06/humidity  81.70
    1614407069947   itp/device_16/temperature       73.22
    1614407069947   itp/device_16/humidity  27.10
    1614407073207   itp/device_23/temperature       80.42
    1614407073225   itp/device_23/humidity  33.00
    1614407073248   itp/device_12/temperature       65.30
    1614407073264   itp/device_12/humidity  40.50

Windows users should use PowerShell to tail mqtt.log

    Get-Content mqtt.log -Wait

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

Install the SQLite database driver

    npm install sqlite

## Migrations

The sqlite driver supports database [migrations](https://www.npmjs.com/package/sqlite#migrations). Migrations are SQL scripts that are used to automatically create database tables.

In your project directory, create a new folder named `migrations`. In the migration folder, create a new file named `001-initial-schema.sql`. The 001-initial-schema.sql file needs to contain the following data.

    --------------------------------------------------------------------------------
    -- Up
    --------------------------------------------------------------------------------

    CREATE TABLE sensor_data (
        device TEXT,
        measurement TEXT,
        reading NUMERIC(9, 2),
        recorded_at TIMESTAMP NOT NULL default current_timestamp
    );

    --------------------------------------------------------------------------------
    -- Down
    --------------------------------------------------------------------------------

    DROP TABLE sensor_data;

Your directory structure should look like this:

![screenshot of the directory structure](img/process-mqtt-directory-structure-migrations.png)

Use `CMD + SHIFT + .` to show hidden files in Finder.  Follow these instuctions to [view hidden files on Windows 10](https://support.microsoft.com/en-us/help/4028316/windows-view-hidden-files-and-folders-in-windows-10).

## Inserting records with Node.js

Create a new file `insert-test.js`. This is a simple program to make sure we can insert data into the database from node.

    const sqlite = require('sqlite');
    const database = './test.db'

Create a function insertRow, that inserts the data into the database. The query uses question marks as paramters. When we call `db.run`, the query and the values are passed to the database engine. You should use placeholders in queries because the input is santized. If you build SQL by concatenating stirngs, it puts you at risk for [SQL Injection attacks](https://github.com/mapbox/node-sqlite3/issues/57).

    async function insertRow(device, measurement, reading) {
        try {
            const query = 'INSERT INTO sensor_data (device, measurement, reading) VALUES (?, ?, ?)';
            const db = await sqlite.open(database, { cached: true });
            console.log(query);
            console.log(device, measurement, reading);    
            const result = await db.run(query, [device, measurement, reading]);
            console.log('Inserted ROWID', result.lastID);
        } catch(err) {
            console.error(err.stack)
        }
    }

Create a main function. The main functions opens the database, and then runs the migrations to create tables if necessary. The first time you run the code the test.db will be created. After the database is open, it will insert one row in to the database.

    async function main() {
        const db = await sqlite.open(database, { cached: true });
        await db.migrate();
        insertRow('device_test', 'acceleration', 9.82)
    }
    main();

Run the program 

    node insert-test.js 

Expected output

    INSERT INTO sensor_data (device, measurement, reading) VALUES (?, ?, ?)
    device_test acceleration 9.82
    Inserted ROWID 1

Use the sqlite3 command to verify that data was inserted into the database.

Open the database

    sqlite3 test.db

Run the query

    SELECT rowid, * FROM sensor_data;

Expected output 

    1|device_test|acceleration|9.82|2020-02-23 22:48:15

## Save MQTT messages to SQLite

Create a new file `mqtt-to-sqlite.js`. This code will save MQTT messages in SQLite using a combination of our code from `mqtt-test.js` and `insert-test.js`.

Include the MQTT and SQLite libraries. Set up the connections.

    require('dotenv').config();
    const mqtt = require('mqtt');
    const sqlite = require('sqlite');

    const database = './example.db'
    const query = 'INSERT INTO sensor_data (device, measurement, reading) VALUES (?, ?, ?)';

    const mqttClient = mqtt.connect(process.env.MQTT_SERVER);

Set up the connect callback for MQTT. Adjust the subscription to match the devices you want to capture. Use specific topics or wildcards. You can also multiple subscriptions here. If you just want to capture your devices, subscribe to `itp/device_XX/+`. Replace `XX` with your device number. To get everyone's data subscribe to `itp/+/+`. 

    mqttClient.on('connect', async () => {
        console.log('MQTT connected');
        await initializeDatabase();
        mqttClient.subscribe('itp/+/+');
    });

The add the message callback. The topic is split to separate the device and measurement information. The message payload contains the measurement reading value. Once we have all the data, call `insertRow`.

    mqttClient.on('message', (topic, message) => {
        console.log(topic, message.toString());

        const parts = topic.split('/');
        const root = parts[0];
        const device = parts[1];
        const measurement = parts[2];
        const reading = message;

        insertRow(device, measurement, reading);
    });

The `insertRow` function writes data to the database. The query is defined as a constant at the top of the file.

    async function insertRow(device, measurement, reading) {
        try {
            const db = await sqlite.open(database, { cached: true });
            const result = await db.run(query, [device, measurement, reading]);
        } catch(err) {
            console.error(err.stack)
        }
    }

Finally we need to create the `initializeDatabase` function which opens the database and creates the tables if necessary using the schema file.

    async function initializeDatabase() {
        const db = await sqlite.open(database, { cached: true });
        await db.migrate();
    }

Run the code

    node mqtt-to-sqlite

Open the database

    sqlite3 example.db

Query the database to ensure new records are being inserted

    SELECT * FROM sensor_data ORDER BY recorded_at desc;

# High Temperature Alert

Code running on the server can be used to check data and send alerts. Create a new file `temperature-alert.js`. Require the mqtt library and create a connection to the MQTT server.

    require('dotenv').config();
    const mqtt = require('mqtt');
    const mqttClient = mqtt.connect(process.env.MQTT_SERVER);

Create a variable for the high temperature limit

    const limit = 80;

Create the phone number that should receive the SMS message when this limit is exceeded. The phone number must be 10 digits and begin with a 1. Replace 215-555-9999 with your number.

    const phoneNumber = '12125559999';

In the connect callback, subscribe to messages from a device. Since we're experimenting, subscribe to `test/name/temperature`. Replace name with your name. We're using the test topic to keep test data out of the itp database.

    mqttClient.on('connect', () => {
        console.log('MQTT Connected');
        mqttClient.subscribe('test/name/temperature');
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

Ensure the environment file is up to date, then start the processes with pm2.

    pm2 start mqtt-logger.js
    pm2 start mqtt-to-sqlite.js

 Starting processes keeps them running and will restart them if they crash. Use `pm2 list` to show process information.

![screenshot of pm2 list output](img/pm2-list.png)

Use `pm2 stop <id|name>` and `pm2 start <id|name>` to stop and start processes. PM2 saves the environment when the process was added. You can see the saved values with `pm2 env <id|name>`. `pm2 info` shows details about the process. Use `pm2 log` to see the console output from your scripts. Use the `--watch` flag with `pm2 start` to have PM2 restart the process when it detects the source files have been changed.

PM2 can restart processes when the machine boots. Install the startup hooks with `pm2 startup`. Save the process list with `pm2 save`. Remember to run `pm2 save` again when you add or change processes.

See the [PM2 documenation](https://pm2.keymetrics.io/docs/usage/pm2-doc-single-page/) for more details.


