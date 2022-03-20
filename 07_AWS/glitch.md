# Glitch

Grafana doesn't natively support DynamoDB. However you can modify the Glitch application from earlier in the semester to visualize data from Dynamo. 

## IAM

Before we get to Glitch, we need to create an new AWS user. Open the [Users](https://us-east-1.console.aws.amazon.com/iamv2/home?region=us-east-1#/users) page in AWS Identity and Access Management (IAM). 

![](img/iam-users.png)

Create a new user named `glitch`. Choose `Access key` for the AWS credential type.

![](img/iam-glitch-user.png)

AWS uses permissions to determine which resources a user can access. Choose `Attach policies directly`. Type `IoTFull | Dynamo` into the filter box to limit which policies are shown. Check the box next to `AWSIoTFullAccess` and `DynamoReader-environment`.

![](img/iam-permissions.png)

Click `Next: Tags`. Skip the tags page and choose `Next: Review`. Click `Create User`.

![](img/iam-review-user.png)

The final page shows the `Access key ID` and `Secret access key` for the glitch user. These values are like a username and password. They must be protected and should never be shared. The secret access key can not be retrieved once you leave this page.

![](img/iam-download-key.png)

## Glitch

Open https://glitch.com/edit/#!/itp-arduino-workshop. Use the button on the top right to "Remix to Edit" to make a copy of the project. Open the `.env` file to configure the server.

Add a username and password that users must enter to access the website

	USERNAME="itp"
	PASSWORD="secret"
	
Enter your AWS information.

	THING_POLICY="ThingPolicy"
	AWS_REGION="us-east-1"
	AWS_ACCESS_KEY_ID="AKIAIOSFODNN7EXAMPLE"
	AWS_SECRET_ACCESS_KEY="wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY"

![](img/glitch-env.png)

Once the properties are set, choose "Show -> In a New Window" from the menu at the top.

![](img/glitch-ui.png)

Next [Grafana](grafana.md)