## Grafana

Use Grafana to visualize your sensor data. Log into your local Grafana server or https://grafana.itpdtd.com. Add a connection to your PostgreSQL RDS database and your Influx database. Create a graph to display your sensor data.

![](img/grafana-query.png)
![](img/grafana-graph.png)

# Glitch & Chart.js

Grafana doesn't natively support DynamoDB. However you can modify the Glitch application from earlier in the semester to visualize data from Dynamo. Open https://glitch.com/edit/#!/itp-arduino-workshop. Use the button on the top right to "Remix to Edit" which makes a copy of the project. In your copy of the project open the `.env` file.

Add a username and password for that is used to access the website

	USERNAME="itp"
	PASSWORD="secret"
	
Enter your AWS information.

	THING_POLICY="ThingPolicy"
	AWS_REGION="us-east-1"
	AWS_ACCESS_KEY_ID="AKIAIOSFODNN7EXAMPLE"
	AWS_SECRET_ACCESS_KEY="wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY"

In order to get the AWS_ACCESS_KEY_ID and AWS_SECRET_ACCESS_KEY, open AWS IAM and navigate to your user. On the Security Credientials tab, create a new Access Key.

Once the properties are set, choose "Show -> In a New Window" from the menu at the top.

![](img/glitch-ui.png)