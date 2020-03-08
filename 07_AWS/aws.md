# AWS IoT

Use AWS to collect and store data from Arduino.

## IAM User

Log into the [AWS Console](http://aws.amazon.com). Open the Identity and Access Managment Service (IAM). Click on the [Users tab](https://console.aws.amazon.com/iam/home#/users) and press the *Add user* button.

![Screenshot of AWS IAM dashboard](img/iam-users.png)

Create a new user. I chose the name `cli` since we're using this user with the Command Line Interface (CLI). Be sure to check the *Programmatic access* option before pressing the *Next* button.

![Screenshot of AWS IAM create new user](img/iam-new-user.png)

Attach the *Administrative Access* policy to this user.

![Screenshot of AWS IAM new user permissions](img/iam-permissions.png)

Review the settings and create the user.

![Screenshot of AWS IAM screen to review new user settings](img/iam-review.png)

Download the credentials CSV file with the access key ID and and secret access key.

![Screenshot of AWS IAM credentials download for new user](img/iam-credentials.png)


## AWS CLI

Install the AWS Command Line Interface (CLI) using [Homebrew](https://brew.sh) or refer to [Amazon's instructions](https://docs.aws.amazon.com/cli/latest/userguide/install-macos.html) for alternate installation methods.

    brew install awscli

Once you install the `awscli`, it needs to be configured with your Access Key ID and Secret Access Key that were downloaded in the previous step. Be sure to choose `us-east-1` as the default region. See the [Amazon documentation](https://docs.aws.amazon.com/cli/latest/userguide/cli-chap-configure.html) for more details about access key ids and secrets.

    $ aws configure
    AWS Access Key ID [None]: AKIAIOSFODNN7EXAMPLE
    AWS Secret Access Key [None]: wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY
    Default region name [None]: us-east-1
    Default output format [None]: table

## Policy

AWS uses IoT policies to give things permission to access AWS IoT resources. Policies can be created using the [AWS IoT website](https://console.aws.amazon.com/iot/home?region=us-east-1#/policyhub), but it's rather cumbersome. We'll use a CloudFormation template to create the policy for us. Download [thing-policy.yaml](thing-policy.yaml). Open a terminal and run this aws command from the same directory as thing-policy.yml.

    aws cloudformation create-stack --template-body file://thing-policy.yaml --stack-name thing-policy

Open the [AWS IoT Core Policy screen](https://console.aws.amazon.com/iot/home?region=us-east-1#/policyhub) in your web browser. You should see the ThingPolicy that was created by AWS Cloud Formation.

![Screenshot of AWS IoT Policy screen](img/cloudformation-thing-policy.png)

This new policy ensures that when a device connects, the client id of the device matches the Common Name in the certificate. The policy also restricts the topics that a device can use. The device can only publish or subscribe to the topic that begin with `things/${clientId}/`. Clicking on the policy will show you details of the policy document. Your policy should look something like this, but with different arns.

    {
        "Version": "2012-10-17",
        "Statement": [
            {
                "Effect": "Allow",
                "Action": "iot:Connect",
                "Resource": "arn:aws:iot:us-east-1:661516571298:client/${iot:Certificate.Subject.CommonName}"
            },
            {
                "Effect": "Allow",
                "Action": [
                    "iot:Publish", 
                    iot:Receive"
                ],
                "Resource": "arn:aws:iot:us-east-1:661516571298:topic/things/${iot:ClientId}/*"
            },
            {
                "Effect": "Allow",
                "Action": "iot:Subscribe",
                "Resource": "arn:aws:iot:us-east-1:661516571298:topicfilter/things/${iot:ClientId}/*"
            }
        ]
    }

### Manually creating a policy

This section is for information only, if you used Cloud Formation to create the policy, skip ahead to [the next section](#arduino).

To manually create a policy, open the [AWS IoT Core](https://console.aws.amazon.com/iot/home?region=us-east-1) console in your web browser. 

![Screenshot of AWS IoT console](img/aws-iot-console.png)

AWS uses IoT policies to give things permission to access AWS IoT resources. Create a new policy. On the left menu, choose Secure -> Policies and press the `Create a Policy` button.

![Screenshot of AWS IoT console showing the policy menu on the left](img/aws-iot-policy-menu.png)

Enter `ThingPolicy` for the name and allow the following policy actions. Let the web page pre-populate the beginning of the ARN including the ACCOUNT_ID.

    Action: iot:Connect 
    Resource: arn:aws:iot:us-east-1:ACCOUNT_ID:client/${iot:Certificate.Subject.CommonName}

    Action: iot:Publish, iot:Receive
    Resource: arn:aws:iot:us-east-1:ACCOUNT_ID:topic/things/${iot:ClientId}/*

    Action:iot:Subscribe
    Resource: arn:aws:iot:us-east-1:ACCOUNT_ID:topicfilter/things/${iot:ClientId}/*

![](img/aws-iot-thing-policy-1.png)
![](img/aws-iot-thing-policy-2.png)

## Arduino

In order to connect the Arduino to AWS IoT Core, our device needs a client certificate instead of a username and password. Use the ECCX08CSR.ino sketch from [week 2](https://github.com/don/ITP-DeviceToDatabase/tree/master/02_Arduino_MQTT) to generate a new private key and certificate signing request (CSR).

### Create CSR

Load [ECCX08CSR.ino](https://github.com/don/ITP-DeviceToDatabase/blob/master/02_Arduino_MQTT/arduino/ECCX08CSR/ECCX08CSR.ino) on your MRK 1000 or MRK WiFi 1010.

Open the Serial Monitor and generate a CSR. You can use all the default values. The common name is pre-populated with the serial number of the ECCX08. You can use this default or create your own name. Refer to the [week 2 instructions](https://github.com/don/ITP-DeviceToDatabase/blob/master/02_Arduino_MQTT/exercises/exercise7.md) if you need more details.

![Screenshot of Arduino sketch generating CSR](../02_Arduino_MQTT/exercises/images/eccx08-csr.png)

Copy the CSR from the serial monitor and save to a new text file named `csr.txt`. Save the device serial number since you'll need that in the next step.

## Create Thing

Open AWS IoT console in your browser and choose `Manage -> Things` from the left side menu.

https://console.aws.amazon.com/iot/home?region=us-east-1#/thinghub

Press the `Register a Thing` button.
Choose `Create a Single Thing`.

 * Use the Serial Number for your device name. Scroll down and choose Next.
 * Choose `Create with CSR`
 * Choose `csr.txt` in the Finder window
 * Choose `Upload`
 * Attach the `ThingPolicy` to your device
 * Choose `Register Thing`

![Screenshot of successful device registration](img/aws-iot-thing-registered.png)

### Download the Certificate

From the [Manage -> Things](https://console.aws.amazon.com/iot/home?region=us-east-1#/thinghub) screen:
 * Click on the newly created device.
 * Click the Security menu on the left
 * Click the Certificate to show the details
 * Use the Action menu to Download the certificate
 * You will need to paste this certificate into config.h in the next step

![Screenshot of page for downloading the device certificate](img/aws-iot-download-certificate.png)

## AWS.ino

Use the `aws` command line tool to get the AWS IoT MQTT broker URL.

    aws iot describe-endpoint --endpoint-type "iot:Data-ATS"

Using the MQTT broker URL and device certificate you downloaded from AWS, configure [AWS.ino](../02_Arduino_MQTT/arduino/AWS/AWS.ino) sketch and load it onto your Arduino. Make sure you use the certificate and not the CSR. Review the [instructions from week 2](/02_Arduino_MQTT/exercises/exercise7.md#awsino) if necessary.

Open the Arduino Serial Monitor and verify that your device has connected to the wireless network and to AWS.

## Test

Open the AWS IoT console and click on the [Test](https://console.aws.amazon.com/iot/home?region=us-east-1#/test) menu on the left hand side. Subscribe to the wildcard topic `#` and ensure you see data arriving from your device.

![Screenshot of AWS IoT MQTT test page](img/aws-iot-test-subscribe.png)
![Screenshot of AWS IoT MQTT test page with messages from the device](img/aws-iot-test-data.png)

AWS is now successfully receiving data from your device!

Ensure that you can send data back to the Arduino. In the test tab, click `Publish to a topic`. The topic should be `things/${clientId}/led`. The payload should be a value between 0 and 100. Sending different values should adjust the brightness of the LED on your Arduino.

![Screenshot of AWS IoT MQTT test sending data to the device](img/aws-iot-test-send.png)

## Database

Now that we have the device sending and receiving data with AWS using MQTT, we need a database to persist the readings. Click the Services menu at the top of the screen and enter `RDS` intro the search box to navigate to the Amazon Relational Database Service page. Use the [hamburger button](https://en.wikipedia.org/wiki/Hamburger_button) on the top left to expand the menu and choose Databases. 

Click the "Create database" button.

![](img/rds-start.png)

Choose the PostgreSQL engine and select Next

![](img/rds-postgres.png)

Choose Dev/Test and select Next

![](img/rds-dev-test.png)

Check the checkbox in the Free tier info box.

![](img/rds-free.png)

Name the DB instance `itp-d2d`. Enter `postgres` for the master username. Choose a secure password. Click Next.

![](img/rds-settings.png)

Choose `Yes` under Public accessibility

![](img/rds-public.png)

Use you initials for the Database name

![](img/rds-options.png)

Scroll down and choose `Create database`

![](img/rds-create-database.png)

![](img/rds-creating.png)

## Test Connection

Click on the RDS home screen, choose Databases, then click the itp-d2d database instance.

![](img/rds-instance.png)

Scroll down to Connectivity &amp; security to get the Endpoint. This is the hostname of your RDS server.

![](img/rds-endpoint.png)

### Security group rules

The security group rules restrict inbound database connections to your current IP address. To connect from other addresses, you need to modify the `rds-launch-wizard` security group rule. Click on `rds-launch-wizard` and edit the inbound rule to allow PostgreSQL traffic from any host, 0.0.0.0/0. Note that we are making the database public so the AWS Lambda function will be able to connect. A better solutions for a real system is to configure the Lambda function so it can [access resources in the VPC](https://docs.aws.amazon.com/lambda/latest/dg/vpc.html).

![](img/rds-security.png)
![](img/rds-inbound.png)


### psql

Use the endpoint URL and connect to your database using `psql`.

![](img/rds-test-connection.png)

## Schema

Now that you're connected to PostgreSQL, let's create set up the new database and users for our data.

    CREATE DATABASE sensors;
    CREATE USER aws WITH ENCRYPTED PASSWORD 'secret';
    GRANT ALL PRIVILEGES ON DATABASE sensors to aws;

Disconnect from psql using `\q`

Reconnect to the sensors database as user aws

    psql -h itp-d2d.cdgh6zvblo40.us-east-1.rds.amazonaws.com -U aws sensors

Create the environment table to hold data from our sensors

    CREATE TABLE environment (
        id SERIAL,
        device TEXT NOT NULL,
        temperature FLOAT,
        humidity FLOAT,
        recorded_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP);

## Lambda

### Node.js

We will use an AWS Lambda function to insert MQTT data into the database. Create a new Node.js project for the Lambda function.

    mkdir store-sensor-data
    cd store-sensor-data
    npm init -y
    touch index.js

Install the PostgreSQL npm driver.

    npm install pg

Open the `store-sensor-data` directory in a text editor. Replace the contents of index.js with:

    const { Pool } = require('pg');

    const pool = new Pool({
        connectionString: process.env.CONNECTION_STRING
    });

    async function insertRow(event) {
        const query = `INSERT INTO environment 
            (device, temperature, humidity) 
            VALUES ($1,$2,$3)`;  
        const values = [event.device, event.temperature, event.humidity];
        console.log(query, values);

        const result = await pool.query(query, values);
        return result.rowCount;
    };

    exports.handler = async (event) => {    
        await insertRow(event);
        const response = {
            statusCode: 200,
            body: 'OK'
        };
        return response;
    };

Edit `package.json` and add a script to zip the contents of the directory. `"zip": "zip -r lambda.zip * -x *.zip *.log"`. Your package.json file should look like

    {
        "name": "save-sensor-data",
        "version": "1.0.0",
        "description": "",
        "main": "index.js",
        "scripts": {
            "test": "echo \"Error: no test specified\" && exit 1",
            "zip": "zip -r lambda.zip * -x *.zip *.log"
        },
        "author": "",
        "license": "ISC",
        "dependencies": {
            "pg": "^7.9.0"
        }
    }

Save the file and run `npm run zip` to generate a zip file for AWS Lambda.

### Create the Lambda function

Use the Services menu at the top of the AWS console to navigate to the AWS Lambda page. 

![](img/lambda-home.png)

Create a new function named `storeSensorData`. Let AWS create a new role with basic Lambda permissions.

![](img/lambda-create.png)

Change the *code entry type* from *edit code inline* to *Upload a .zip file*. 

![](img/lambda-create-success.png)

Click the upload button and choose lambda.zip. Hit Save on the top right.

![](img/lambda-upload-zip.png)

Scroll down to the Environment Variables section. Create a new variable CONNECTION_STRING with your PostgreSQL connection information. Make sure you adjust the URL for your RDS connection.

    postgres://aws:secret@itp-d2d.cdgh6zvbol40.us-east-1.rds.amazonaws.com/sensors

![](img/lambda-connection-string.png)

Press the save button in the top right.

#### Testing

Use the *Select a test event* combo box and select *Configure test events*. Create an event named `TestSensorData`. The event object should have device, temperature, and humidity. Click create.

![](img/lambda-test-event.png)

Click test.

![](img/lambda-test-success.png)

Assuming the test works, use `psql` to view the data in the database. `SELECT * FROM environment;`

![](img/lambda-select-test-data.png)

## Rules

The Lambda function inserts event data into the database. Next, we need to create a Rule that calls our Lambda function when MQTT data arrives. Use the Services menu and navigate back to IoT Core. Click the Act menu on the left side and create a rule.

![](img/rule-no-rules.png)

Name the rule `environment`

![](img/rule-create.png)

Add the *rule query statement*.

    SELECT topic(2) as device, temperature, humidity FROM 'things/+/environment'
    
The `topic(2)` field gets the 2nd element from the MQTT topic, which in this case, is the device name. See the [AWS IoT SQL Reference](https://docs.aws.amazon.com/iot/latest/developerguide/iot-sql-reference.html) for more details.

![](img/rule-query.png)

Press the add action button. Select "Send a message to a Lambda function" and choose configure action.

![](img/rule-send-to-lambda.png)

Select the "storeSensorData" Lambda function and choose add action.

![](img/rule-store-sensor-data.png)

Choose create rule

![](img/rule-create-success.png)

Now that the rule is deployed, query the environment table again. You should see new data being inserted into the table.

![](img/rule-select-real-data.png)

## Grafana

Log into your local Grafana server. Add a connection to your RDS database. Create a graph to display your sensor data.

![](img/grafana-query.png)
![](img/grafana-graph.png)

Next [Notifications](notifications.md) 

