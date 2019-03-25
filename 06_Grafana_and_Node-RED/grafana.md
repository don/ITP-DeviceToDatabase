# Grafana

Install [grafana](https://grafana.com) using brew on macOS

    brew install grafana

Start the service with 

    brew services start grafana

Shut the service down when you're done with 

    brew services stop grafana

See https://grafana.com/grafana/download for other platforms

Once the server has started, open a web browser to http://localhost:3000

Log into Grafana using admin/admin. You will be prompted to set up a new password.

![screenshot grafana login screen](img/grafana-login.png)

Home dashboard. Choose `Add data source`.

![screenshot grafana home screen](img/grafana-home.png)

We are going to add a TimescaleDB DB connection so select PostgreSQL.

![screenshot grafana add data source](img/grafana-add-datasource.png)

Add the configuration information for TimescaleDB. Be sure to enable the switch for TimescaleDB under PostgreSQL details.

![screenshot grafana with timescaledb data source configuration](img/grafana-timescale.png)

From the home dashboard choose `New dashboard`.

![screenshot grafana home dashboard](img/grafana-before-dashboard.png)

On the new dashboard. Click the `Add Query` button.

![screenshot grafana empty new dashboard](img/grafana-new-dashboard.png)

Use the UI to create a query for temperature grouped by device.

![screenshot grafana configuring temperature query grouped by device](img/grafana-add-query.png)

Use the arrow on the top left to navigate back to the dashboard.

![screenshot of dashboard with the new query](img/grafana-dashboard.png)

Add an InfluxDB data source.

![screenshot of grafana configuration for influxdb connection](img/grafana-influx.png)

Add some gauges using the queries from InfluxDB. Change the time range to be `This month so far`. 

![screenshot of dashboard with gauges and lots of series in the temperature graph](img/grafana-too-many-series.png)

Increasing the time range shows many more devices. The graph can get very confusing. Click on one device in the legend to hide the other series.

![screenshot of dashboard with gauges from influx and one series selected in the temperature graph](img/grafana-select-one.png)

