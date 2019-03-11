# Processing MQTT data with Node.js

   * [Processing MQTT data with Node.js](#processing-mqtt-data-with-nodejs)
      * [Create a Node project](#create-a-node-project)
      * [Environment](#environment)
      * [Read messages from MQTT](#read-messages-from-mqtt)
      * [Log MQTT Messages to a File](#log-mqtt-messages-to-a-file)
   * [Write MQTT Messages to TimescaleDB](#write-mqtt-messages-to-timescaledb)
      * [Set up the database](#set-up-the-database)
      * [PostgreSQL driver and connection settings](#postgresql-driver-and-connection-settings)
      * [Inserting records with Node.js](#inserting-records-with-nodejs)
      * [Save MQTT messages to TimescaleDB](#save-mqtt-messages-to-timescaledb)
   * [High Temperature Alert](#high-temperature-alert)
      * [Sending SMS with AWS SNS](#sending-sms-with-aws-sns)
      * [Sending SMS without an AWS Account](#sending-sms-without-an-aws-account)
   * [PM2](#pm2)
   
## Create a Node project

We are going to create a Node.js project with code to read and process data from MQTT topics.

Open a `Terminal.app` and create new directory for this project

    mkdir week5
    cd week5

Use npm to initialize the project

    npm init -y

Install the mqtt library

    npm install mqtt

## Environment

Create a file name `.env` to store environment variables. Since this file contains usernames and passwords it should *not* be commited to version control. Add `.env` to your `.gitignore` file. Your file should look like this.

    export MQTT_SERVER=mqtts://user:password@iotwork.shop

Replace `user` and `password` with your username and password for the MQTT server.

The contents of `.env` are loaded into the environment by running `source .env`. In your terminal, source the file. You won't see any output unless there are errors. The environment is set for this current shell. If you open a different terminal, you'll need to source the file again.

    source .env

## Read messages from MQTT

Let's start by reading data from MQTT and printing it to standard output. Create a new file `mqtt-test.js` and enter the following contents

    const mqtt = require('mqtt');

    const mqttClient = mqtt.connect(process.env.MQTT_SERVER);

    mqttClient.on('connect', () => {
        console.log('MQTT Connected');
        mqttClient.subscribe('#');
    });

    mqttClient.on('message', (topic, message) => {
        console.log(topic, message.toString());
    });

Run the `mqtt-test` and ensure you see MQTT message printed to the console.

    node mqtt-test.js

## Log MQTT Messages to a File

Now that we can read MQTT messages, let's write them to a file instead of just printing them to the console. Copy `mqtt-test.js` to `mqtt-logger.js`.

    cp mqtt-test.js mqtt-logger.js

Open `mqtt-logger.js` in your editor. Near the top of the file, require `fs` and create a file name.

    const fs = require('fs');
    const fileName = './mqtt.log';

Replace the existing on messages handler with this new function

    mqttClient.on('message', function (topic, message) {        
        const timestamp = new Date().getTime();
        const data = `${timestamp}\t${topic}\t${message}\n`;
        fs.writeFile(fileName, data, { flag: 'a' }, () => {});
    });

The [getTime](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/getTime) method get the timestamp from the date in milliseconds since epoch. A tab-delimited string is created using the timestamp, topic, and message. The string is written to the log file with the append flag. Without the append flag, the new data would overwrite the old file.

Start the process

    node mqtt-logger.js

Open a 2nd terminal window and tail the MQTT log to ensure data is being written correctly.

    tail -f mqtt.log

Note that the logger will fail if your MQTT message contains tabs or linefeeds. (This might happen with JSON payloads.) You can add additional code to handle edge cases. Be sure to use payload instead of message when creating the data string.

    const payload = message.toString();
    // replace tabs with 2 spaces
    payload = payload.replace(/\t/g, '  ');
    // replace linefeeds with 1 space
    payload = payload.replace(/\n/g, ' ');

There are also [many TSV and CSV libraries](https://www.npmjs.com/search?q=tsv) available for node that can make reading and writing delimited files easier.

# Write MQTT Messages to TimescaleDB

We can use Node.js to save MQTT message into a database such as TimescaleDB.

## Set up the database

Use `psql` to connect to your database instance on the TimescaleDB server.

    psql -h timescale.iotwork.shop -U xx 

Create a new table for sensor data

    CREATE TABLE sensor_data (
        device VARCHAR(50),      
        measurement VARCHAR(50),
        reading DOUBLE PRECISION,
        recorded_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
    );

Ensure the TimescaleDB extensions are installed

    CREATE EXTENSION IF NOT EXISTS timescaledb CASCADE;

Convert the new table to a hypertable

    SELECT create_hypertable('sensor_data', 'recorded_at');

Ensure you can insert data into the table

    INSERT INTO sensor_data (device, measurement, reading)
        VALUES ('fake', 'test', 17.0);

Select data from the table

    SELECT * FROM sensor_data;

Delete your test row

    DELETE FROM sensor_data WHERE device = 'fake';

## PostgreSQL driver and connection settings

Install the PostgreSQL database driver

    npm install pg

Add another line to `.env` with the TimescaleDB connection information. Replace `user`, `password` with your TimescaleDB username and password. The `instance` should be the database with your initials.

    export TIMESCALE_SERVER=postgresql://user:password@timescale.iotwork.shop:5432/instance

Save the file and source it again to add the new values to your environment.

    source .env

## Inserting records with Node.js

Create a new file `insert-test.js`. This is a simple program to make sure we can insert data into the database from node.

    const { Pool } = require('pg');

    const pool = new Pool({
    connectionString: process.env.TIMESCALE_SERVER
    });

    async function insertRow(device, measurement, reading) {

        const query = 'INSERT INTO sensor_data (device, measurement, reading) VALUES ($1,$2,$3)';  
        const values = [device, measurement, reading];
        console.log(query, values);

        try {
            const result = await pool.query(query, values);
            console.log(`Inserted ${result.rowCount} record.`);  // should be 1
        } catch(err) {
            console.log(err.stack);
        }
    }

    insertRow('device_xx', 'acceleration', 9.82);

Run the program 

    node insert-test.js 

Expected output

    INSERT INTO sensor_data (device, measurement, reading) VALUES ($1,$2,$3) [ 'device_xx', 'acceleration', 9.82 ]
    Inserted 1 record.
    
Use psql to verify the records were inserted

    psql -h timescale.iotwork.shop -U xx
    select * from sensor_data;

## Save MQTT messages to TimescaleDB

Create a new file `mqtt-to-timescale.js`. This code will save MQTT messages in TimescaleDB using a combination of our code from `mqtt-test.js` and `insert-test.js`.

Include the MQTT and PostgreSQL libraries. Set up the connections.

    const mqtt = require('mqtt');
    const { Pool } = require('pg');

    const pool = new Pool({
        connectionString: process.env.TIMESCALE_SERVER
    });

    const mqttClient = mqtt.connect(process.env.MQTT_SERVER);

Set up the connect callback for MQTT. Adjust the subscription to match the devices you want to capture. Use specific topics or wildcards. You can add multiple subscriptions here.

    mqttClient.on('connect', () => {
        console.log('MQTT connected');
        mqttClient.subscribe('itp/device_xx/+');
    });

The add the message callback. The topic is split to separate the device and measurement information. The message payload contains the measurement reading value. Once we have all the data, call `insertRow`.

    mqttClient.on('message', (topic, message) => {
        console.log(topic, message.toString());

        const parts = topic.split('/');
        const root = parts[0];              // itp (not used)
        const device = parts[1];            // device_xx
        const measurement = parts[2];       // temperature, humidity, etc
        const reading = message.toString(); // convert buffer to a string 

        insertRow(device, measurement, reading);
    });

Copy the `insertRow` function from `insert-test.js` into this file.

    async function insertRow(device, measurement, reading) {

        const query = 'INSERT INTO sensor_data (device, measurement, reading) VALUES ($1,$2,$3)';  
        const values = [device, measurement, reading];
        console.log(query, values);

        try {
            const result = await pool.query(query, values);
            console.log(`Inserted ${result.rowCount} record.`);  // should be 1
        } catch(err) {
            console.log(err.stack);
        }
    }

Run the code

    node mqtt-to-timescale

Query the database to ensure new records are being inserted

    SELECT * FROM sensor_data ORDER BY recorded_at desc;

# High Temperature Alert

Create a new file `temperature-alert.js`. Require the mqtt library and create a connection to the MQTT server.

    const mqtt = require('mqtt');
    const mqttClient = mqtt.connect(process.env.MQTT_SERVER);

In the connect callback, subscribe to messages from a device.

    mqttClient.on('connect', () => {
        console.log('MQTT Connected');
        mqttClient.subscribe('itp/device_xx/temperature');
    });

When a message is received, check to see if the value is over the limit and post a new message to the alert topic.

    mqttClient.on('message', (topic, message) => {
        console.log(topic, message.toString());
        const temperature = Number(message.toString());

        if (temperature > 80) {
            const alertMessage = `Temperature ${temperature}°F exceeds the high temperature limit of 80°F`
            mqttClient.publish('itp/device_xx/alert', alertMessage);
        }
    });

Run this code

    node temperature-alert.js

While this code is running it will post messages to the alert topic for your device when the temprature is over 80F. Another application could process those alerts and do something.

Test this by sending some fake data for `device_xx`. Create a new file `publish.js` to publish some data to your topic. Note that you might want to create a fake device for this test so you don't pollute your real data.

    const mqtt = require('mqtt');
    const mqttClient = mqtt.connect(process.env.MQTT_SERVER);

    mqttClient.on('connect', () => {
        console.log('MQTT Connected');
        mqttClient.publish('itp/device_xx/temperature', "120");
        console.log('Sent message');
        mqttClient.end();
    });    

## Sending SMS with AWS SNS

It would be better if our rule could send us an email or text message when the temperature was exceeded. Using Amazon Web Service (AWS) Simple Notifcation Service (SNS) we can send SMS messages.

If you don't have an AWS account, there'll be an alternate way to do this below. This example assumes you have an AWS account, the [AWS CLI](https://aws.amazon.com/cli/) installed and you're logged in. Install the AWS Javascript SDK.

    npm install aws-sdk

Copy `temperature-alert.js` to `temperature-alert-sms.js`

    cp temperature-alert.js temperature-alert-sms.js

Open `temperature-alert-sms.js` in your editor. Import the aws-sdk and set the region. Note that the aws-sdk gets your credentials from the file system. Run `aws configure` to set up the access key and secret.

    var AWS = require('aws-sdk');
    AWS.config.update({ region: 'us-east-1' });

Create a new function `sendSms` that takes a phone number and a message.

    async function sendSms(number, message) {

        var params = {
            Message: message,
            PhoneNumber: number
        };

        try {
            // Send the message
            let response = await new AWS.SNS({ apiVersion: '2010-03-31' }).publish(params).promise();
            console.log(`MessageID is ${response.MessageId}`);
        } catch (e) {
            console.error(e, e.stack)
        }
    }

Edit the on message function. Remove the mqtt publish line.

    // mqttClient.publish('itp/device_xx/alert', alertMessage);

Add a new line to send the SMS message. Assuming your number is (201) 555-9999 format it like +12015559999.

    sendSms('+12015559999', alertMessage);

Save the file and run

    node high-temperature-sms.js

Run `publish.js` to test this

    node publish

![SMS Notification: "Temperature 120F exceeds the high temperature limit of 80F"](img/sms-notification.jpg)

## Sending SMS without an AWS Account

For students without an AWS account, I created a process to listen to the MQTT topic `itp/sms/+` and send SMS messages. Publish a MQTT message to `itp/sms/$PHONE_NUMBER` and the code will you a SMS message. Add another `mqttClient.publish` to send a SMS message.

    mqttClient.publish('itp/device_xx/alert', alertMessage);
    mqttClient.publish('itp/sms/+12015559999', alertMessage);

Note that this only works while I'm running my `mqtt-to-sms` code.

# PM2

The mqtt-logger and mqtt-to-timescale processes will keep running as long as we keep the terminal window open, which works great for development but isn't very convenient for long runnning processes. [PM2](https://pm2.io/runtime/) is a process manager that run the processes as daemons for us.

Install PM2 globally

    npm install -g pm2

Ensure the environment file is up to date, then start the processes with pm2.

    source .env
    pm2 start mqtt-logger.js
    pm2 start mqtt-to-timescale.js

 Starting processes keeps them running and will restart them if they crash. Use `pm2 list` to show process information.

![screenshot of pm2 list output](img/pm2-list.png)

Use `pm2 stop <id|name>` and `pm2 start <id|name>` to stop and start processes. PM2 saves the environment when the process was added. You can see the saved values with `pm2 env <id|name>`. `pm2 info` shows details about the process. Use `pm2 log` to see the console output from your scripts. Use the `--watch` flag with `pm2 start` to have PM2 restart the process when it detects the source files have been changed.

PM2 can restart processes when the machine boots. Install the startup hooks with `pm2 startup`. Save the process list with `pm2 save`. Remember to run `pm2 save` again when you add or change processes.

See the [PM2 documenation](https://pm2.io/doc/en/runtime/overview/) for more details.


