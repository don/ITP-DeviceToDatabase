# DynamoDB

Save the environment data from the sensor in DynamoDB. 

Create a new DynamoDB table named `environment`. Add String partition key named `device` and Number sort key named `timestamp`. 

![](img/dynamo-create-db.png)

Go to the AWS IoT dashboard. Edit the `environment` rule. This rule is currently using a Lambda function to store data in PostgreSQL. We are going to update the rule so data is also sent to DynamoDB.

![](img/dynamo-before-modify.png)

Update the query to include the timestamp.

    SELECT topic(2) as device, timestamp() as timestamp, temperature, humidity FROM 'things/+/environment'

Add an action to split the message into multiple columns in a DynamoDB table.

![](img/dynamo-split.png)

Choose the `environment` table. Create a new role `MqttToDynamo`.

![](img/dynamo-configure-action.png)

Your updated rule should look like this.

![](img/dynamo-updated-rule.png)

Once the new rule is in place, the DynamoDB table will start being populated. Use the AWS console to [view data in the environment table](https://console.aws.amazon.com/dynamodb/home?region=us-east-1#tables:selected=environment;tab=items).

![](img/dynamo-data.png)

## Glitch UI

Now that Dynamo is running, you can modify Glitch UI from Week 2. Open https://glitch.com/edit/#!/itp-arduino-workshop. Use the menu on the top left to "Remix Project" which makes a copy. In your copy of the project open the `.env` file.

Add a username and password for that is used to access the website

	USERNAME="itp"
	PASSWORD="secret"
	
Enter your AWS information.

	AWS_REGION="us-east-1"
	AWS_ACCESS_KEY_ID="AKIAIOSFODNN7EXAMPLE"
	AWS_SECRET_ACCESS_KEY="wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY"

Our Dynamo table is different than the original version, so we also need to edit `server.js`. The code expects a column name `deviceId` in the environment table. Our column is named `device`. Edit lines 115 to 121 to fix this.

Original code

	params.KeyConditionExpression = "#deviceId = :device_id";
	params.ExpressionAttributeNames = {
	  "#deviceId": "deviceId"
	};
	params.ExpressionAttributeValues = {
	  ":device_id": id
	};

Suggested Modification

	params.KeyConditionExpression = "#device = :device";
	params.ExpressionAttributeNames = {
	  "#device": "device"
	};
	params.ExpressionAttributeValues = {
	  ":device": id
	};

![](img/glitch-ui.png)