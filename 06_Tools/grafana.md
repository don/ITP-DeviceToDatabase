# Grafana

## Install

### macOS 

Install [grafana](https://grafana.com) using brew on macOS

    brew install grafana

Start the service with 

    brew services start grafana

Shut the service down when you're done with 

    brew services stop grafana

### Windows

Download the Windows installer from https://grafana.com/grafana/download?platform=windows&edition=oss. I used the OSS version. The Enterprise version will work too. Run the installer and follow the prompts.

### Cloud

If you'd rather not install Grafana, you can try and create a cloud account at https://grafana.com/products/cloud/

## Login

Once the server has started, open a web browser to http://localhost:3000

Log into Grafana using admin/admin. You will be prompted to set up a new password.

![screenshot grafana login screen](img/grafana-login.png)

Home dashboard. Choose `Add data source`.

![screenshot grafana home screen](img/grafana-home.png)

## TimescaleDB 

We are going to add a TimescaleDB DB connection so select PostgreSQL.

![screenshot grafana add data source](img/grafana-add-datasource.png)

![screenshot grafana add data source timescale](img/grafana-add-datasource-timescale.png)

Add the configuration information for TimescaleDB. 

 * Host: timescale.dev2db.com:5432
 * Database: tsitp
 * Use your username and password
 * Change SSL mode to require
 * Under PostgreSQL details
    * Change version to 12+
    * Enable the switch for TimescaleDB 

![screenshot grafana with timescaledb data source configuration](img/grafana-timescale.png)

Click Save & Test

From the home screen choose `Create your first dashboard`.

![screenshot grafana home dashboard](img/grafana-before-dashboard.png)

Push the `Add new panel` button

![screenshot grafana empty new dashboard](img/grafana-new-dashboard.png)

Use the UI to edit the query to show temperature grouped by device. Your query should match the screenshot below.

![screenshot grafana configuring temperature query grouped by device](img/grafana-timescale-query.png)

Use the Query Inspector button to see more details about how Grafana is getting data from Timescale.

![screenshot grafana query inspector](img/grafana-query-inspector.png)

Use the arrow on the top left to navigate back to the dashboard.

![screenshot of dashboard with the new query](img/grafana-dashboard.png)

Click the `Add Panel` button near the top, and push the blue `Add new Panel` button.

![screenshot of the gauge query](img/grafana-gauge-query.png)

Expand the visualization section on the right to switch from a bar graph to a gauge. Edit the graphical query editor so it matches the screenshot above. Hit apply to save. Resize the charts so they fit nicely on the page.

![screenshot of the dashboard with the graph and a gauge](img/grafana-dashboard-with-gauge.png)

## InfluxDB

Add another database source for InfluxDB. From the left side toolbar, choose Setting -> Data Sources -> Add Data Source. Choose InfluxDB.

![screenshot of choosing the influx datasource](img/grafana-add-datasource-influx.png)

HTTP
* URL https://influx.dev2db.com:8086
 
![screenshot of influx datasource configuration](img/grafana-influx-config.png)

InfluxDB Details
* Database: itp
* Enter your username and password
 
![2nd screenshot of influx datasource configuration](img/grafana-influx-config2.png)

Add another panel to your dashboard and create a temperature query using InfluxDB as a data source. The `time($_interval)` the GROUP BY section buckets the data into the interval specified in the datasource, 10 seconds. Experiment with a larger interval like `time(5m)` or `time(4h)` to see how it changes your graph.

![screenshot of influx query configuration](img/grafana-influx-query.png)

![screenshot of influx panel on dashboard](img/grafana-influx.png)

Sometimes there is too much data in a chart to clearly see what's happening. Clicking on a label in the legend will display only that series.

![screenshot of dashboard and one series selected in the temperature graph](img/grafana-select-one.png)


The dashboard can mix queries from multiple data sources.

![screenshot of dashboard with multiple charts](img/grafana-timescale-influx.png)

Use the time selection drop down on the top right to choose different time frames, such as `Last 2 days` or  `This month so far`. 

Now create some additional dashboards and queries for your data.

## Cleanup

### macOS

    brew services stop grafana

### Windows

The Grafana installer installed a Windows service to start Grafana whenever you boot your computer. You probably don't want that. In the Windows search box, type "service" and open the Services app. Scroll down to Grafana. Right click and select Properties. Change the startup type from Automatic to Manual.

![grafana service in windows services app](img/services-grafana.png)
![grafana service in windows services app](img/services-grafana-properties-menu.png)
![grafana service in windows services app](img/services-grafana-automatic.png)
![grafana service in windows services app](img/services-grafana-startup-menu.png)
![grafana service in windows services app](img/services-grafana-manual-startup.png)

Now that the service is set to manual, you'll need to start Grafana from the Services app when you reboot your computer.

Alternately you can use an Administrator cmd prompt to start and stop the Grafana service

    sc start Grafana

    sc stop Grafana