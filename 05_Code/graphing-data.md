# Graphing TimescaleDB data with Google Charts

We are going to create an Express app to query the database and use Google Charts to crate a graph to display information for the user.

## Create a project

Open `Terminal.app` and create a new project.

    mkdir dev2db-charts
    cd dev2db-charts
    npm init -y

Install the Express framework

    npm install express

Create `server.js` with a minimal express app.

    const express = require('express');
    const app = express();
    const port = 4000;

    app.get('/', (req, res) => res.send('Hello World!'));

    app.listen(port, () => console.log(`Sensor data app listening on port ${port}`));

Start the server to make sure everything is working

    node server.js

Open a web browser to http://localhost:4000

![screenshot of browser loading hello world express app](img/hello-express.png)

## Environment

Install the [dotenv](https://www.npmjs.com/package/dotenv) library

    npm install dotenv

Dotenv loads application configuration data from a .env file so we can keep passwords and other configuration out of the code. Create a file name `.env` to store the TimescaleDB connection information. Replace `user`, `password` with your TimescaleDB username and password. The database `instance` is the timescale version of itp, `tsitp`.

    TIMESCALE_SERVER=postgresql://user:password@timescale.dev2db.com:5432/tsitp

On the first line of `server.js` add the following code
    
    require('dotenv').config();

## PostgreSQL

Install the PostgreSQL driver

    npm install pg

Add the postgres configuration near the top of `server.js` after `require('dotenv').config();` 

    const { Pool } = require('pg');

    const pool = new Pool({
        connectionString: process.env.TIMESCALE_SERVER
    });

## Get devices

Add a new endpoint to return a list of devices that have temperature data.

    app.get('/device', async (req, res) => {
        const query = `SELECT distinct device 
            FROM sensor_data 
            WHERE measurement = 'temperature'
            ORDER BY device`; 
        console.log(query);

        try {
            const results = await pool.query(query);
            console.log(`returning ${results.rowCount} rows`);
            const devices = results.rows.map(d => d.device);
            res.send(devices);
        } catch(err) {
            console.log(err.stack);
            res.status(400).send('server error');
        }
    });

Use Control + C to stop the server. Restart the server so it runs the new code.

    node server.js

Open http://localhost:4000/device in a web browser to view the devices

![](img/devices-browser.png)

Alternately you could open new Terminal.app and use curl to test the new endpoint

    curl http://localhost:4000/device

You should get back a list of devices

![](img/devices-curl.png)


## Get temperature data

Add another endpoint to get temperature data for a specific device.

    app.get('/device/:device/temperature', async (req, res) => {
        const device = req.params.device;
        const query = `SELECT recorded_at, reading::float as temperature 
            FROM sensor_data 
            WHERE measurement = 'temperature' 
            AND device = $1`;
        const params = [device];
        console.log(query, params);

        try {
            const results = await pool.query(query, params);
            console.log(`returning ${results.rowCount} rows`);
            res.send(results.rows);
        } catch(err) {
            console.log(err.stack);
            res.status(400).send('server error');
        }
    });

Restart the sever and open http://localhost:4000/device/device_01/temperature to test the new endpoint.

![](img/device_01-temperature-json.png)


## Static files

Now that we have APIs to serve data we need some web pages to use the APIs. Express can be configured to serve static files from a directory. We will use this to send HTML and Javascript pages to the web browser. Add the express middleware to `server.js` before the first `app.get` line.

    app.use(express.static('public'));

Optionally **Remove** the old default route

    // app.get('/', (req, res) => res.send('Hello World!'));

Create a new directory named `public` for the html and javascript files. Create a file named index.html in the public directory.

    <!-- index.html -->
    Hello from index.html

Restart your server and test that your index.html file is being served.

![screenshot of safari loading index.html page](img/hello-index.png)

## pm2-dev

Restarting the server after each change can be tedious. We can use `pm2-dev` to watch for changes to our project and automatically restart the server. Use Control + C to stop the server. Install pm2, then restart the server. 

    sudo npm install -g pm2
    pm2-dev server.js

## index.html and index.js

Now that our server is working, we can write the client side of our app. Replace the existing contents of index.html with the following.

    <!DOCTYPE html>
    <html>
    <head>
        <title>Device to Database</title>
        <script src="https://www.gstatic.com/charts/loader.js"></script>
        <script src='index.js'></script>
    </head>

    <body>
        <div>
            <div style="padding:10px">Device <select id="deviceSelect"></select></div>
            <div id='chart_div' style='width: 100%; height: 800px'></div>
        </div>
    </body>
    </html>

Create a new file `public/index.js`. When index.js loads in the web browser, it gets a list of devices from the server and builds an HTML select control with the data.

    window.addEventListener('load', getDeviceList);

    // Get a list of devices from the server
    async function getDeviceList() {
        const response = await fetch('device');
        const devices = await response.json();
        buildSelectControl(devices);
    }

    // Build HTML for <select> control with devices
    function buildSelectControl(devices) {
        const select = document.querySelector('select');
        devices.forEach(device => {
            const option = document.createElement('option');
            option.innerText = device;
            select.appendChild(option);
        });
        select.addEventListener('change', onDeviceSelected, false);
    }

Test in your browser to ensure you see a list of devices.

![screenshot of select control in web browser](img/device-list.png)

When the selected item in the device dropdown changes, we need to ask the server for temperature data from that device. Create the `onDeviceSelected` event handler.

    // onChange event handler for device <select> control
    function onDeviceSelected(e) {
        const device = e.target.value;
        getChartData(device);
    }

The `getChartData` function that calls the `device/:device/temperature` endpoint on the server, formats the JSON data into an array of arrays, and calls `drawChart`.

    // Get temperature data from the server for the selected device
    async function getChartData(device) {
        const response = await fetch(`device/${device}/temperature`);
        const json = await response.json();
        const rows = json.map(row => [new Date(row.recorded_at), row.temperature]);
        drawChart(rows);
    }

The `drawChart` function sets up and renders the chart using Google Chart APIs.

    // Draw the chart
    function drawChart(rows) {
        var data = new google.visualization.DataTable();
        data.addColumn('datetime', 'Date');
        data.addColumn('number', 'Temperature');
        data.addRows(rows);

        var chart = new google.visualization.AnnotationChart(document.getElementById('chart_div'));

        var options = {
            displayAnnotations: true
        };

        chart.draw(data, options);
    }

The last line of the file needs to load the Google Chart package.

    google.charts.load('current', {'packages':['annotationchart']});

Reload the http://localhost:4000, choose a device

![screenshot of graph in browser](img/device_ef3.png)

To have the chart render when the page loads, add this line to the end of the `getDeviceList` function.

    getChartData(deviceSelect.value);

## Slow response

Some of the devices return a lot of data, and the screens render slowly. We can take advantage of Timescale and use time_bucket to roll up the data. Try updating the the query to use 15 minute increments.

    const query = `SELECT time_bucket('15 minute', recorded_at) as recorded_at, avg(reading) as temperature
        FROM sensor_data
        WHERE measurement = 'temperature'
        AND device = $1
        GROUP BY 1`;


# More Charts

Create a new http endpoint in `server.js` that returns the message count by device.

    app.get('/messages-per-device', async (req, res) => {

        const query = `SELECT device, count(*) 
            FROM sensor_data 
            GROUP BY device
            ORDER BY device`; 
        console.log(query);

        try {
            const result = await pool.query(query);
            res.send(result.rows);
        } catch(err) {
            console.log(err.stack);
            res.status(400).send('server error');
        }
    });

Create a new http endpoint in `server.js` that returns the message count by day.

    app.get('/messages-per-day', async (req, res) => {

        const query = `SELECT recorded_at::date as date, count(*) 
            FROM sensor_data 
            GROUP BY date
            ORDER BY count`; 
        console.log(query);

        try {
            const result = await pool.query(query);
            res.send(result.rows);
        } catch(err) {
            console.log(err.stack);
            res.status(400).send('server error');
        }
    });

Test functions in the brower.

![](img/by-day.png)    

To make the JSON easier read, set `json spaces` on the server. Pretty JSON is less efficient, but better for learning.

    // 'json spaces' pretty prints JSON which is nice for debugging
    app.set('json spaces', 2);

![](img/by-day-pretty.png)


Create new a new file `public/by-device.html`

    <html>
    <head>
        <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    </head>

    <body>
        <div id="chart_div"></div>
        <script src="by-device.js"></script>
    </body>
    </html>

Create new a new file `public/by-device.js`

    // Load the Visualization API and the corechart package.
    google.charts.load('current', {'packages':['corechart']});

    // Set a callback to run when the Google Visualization API is loaded.
    google.charts.setOnLoadCallback(drawChart);

    async function getMessageCountByDevice() {
        const response = await fetch('messages-per-device');
        const json = await response.json();
        const rows = json.map(row => [row.device, Number(row.count)]);
        return rows;
    }

    // Callback that creates and populates a data table, instantiates
    // the pie chart, passes in the data and draws it.
    async function drawChart() {

        // Create the data table.
        const data = new google.visualization.DataTable();
        data.addColumn('string', 'Device');
        data.addColumn('number', 'Message Count');
        const rows = await getMessageCountByDevice();
        data.addRows(rows);

        // Set chart options
        const options = {
            title:'Message Count per Device',
            is3D: true,
            //sliceVisibilityThreshold: .01,
            width: 2048,
            height: 1000
        };

        // Instantiate and draw our chart, passing in some options.
        const chart = new google.visualization.PieChart(document.getElementById('chart_div'));
        chart.draw(data, options);
    }

![screenshot of messages by device pie chart](img/message-count-by-device.png)

Create new a new file `public/by-day.html`

    <html>
    <head>
        <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    </head>
    <body>
        <div id="barchart_material" style="width: 100%; height: 100%;"></div>
        <script src="by-day.js"></script>
    </body>
    </html>

Create new a new file `public/by-day.js`

    google.charts.load('current', {'packages':['bar']});
    google.charts.setOnLoadCallback(drawChart);

    async function getMessageCountByDay() {
        const response = await fetch('messages-per-day');
        const json = await response.json();
        const rows = json.map(row => [new Date(row.date), Number(row.count)]);
        return rows;
    }

    async function drawChart() {
        // Create the data table.
        const data = new google.visualization.DataTable();
        data.addColumn('date', 'Date');
        data.addColumn('number', 'Message Count');
        const rows = await getMessageCountByDay();
        data.addRows(rows);

        var options = {
            chart: {
                title: 'Message Count by Day'
            },
            bars: 'vertical' // optionally try horizontal
        };

        var chart = new google.charts.Bar(document.getElementById('barchart_material'));

        chart.draw(data, google.charts.Bar.convertOptions(options));
    }

![screenshot of messages by day bar chart](img/message-count-by-day.png)

For more information about how you can use Google Charts see https://developers.google.com/chart/.

