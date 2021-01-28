# Mosquitto Command Line Examples

MQTT command line examples from class

## Subscribing

Subscribe to all messages

	mosquitto_sub -h dev2db.cloud.shiftr.io -u dev2db -P public -t '#' -v

Subscribe to the temperature topic for nano33 device

	mosquitto_sub -h dev2db.cloud.shiftr.io -u dev2db -P public -t itp/nano33/temperature -v

Subscribe to the humidity topic for nano33 device

	mosquitto_sub -h dev2db.cloud.shiftr.io -u dev2db -P public -t itp/nano33/humidity -v
	
Subscribe to all the topics for nano33 device

	mosquitto_sub -h dev2db.cloud.shiftr.io -u dev2db -P public -t itp/nano33/+ -v
	
Subscribe to the temperature topic for all devices

	mosquitto_sub -h dev2db.cloud.shiftr.io -u dev2db -P public -t itp/+/temperature -v

The `-h` flag specifies the hostname of the broker. The '-u' flag specifies the user and the '-P' flag specifies the password. The `-t` flag specifies the topic to subscribe to. The `-v` flag is verbose mode which prints the topic along with the payload. 

### Wildcards

Wildcards only work for subscribing, not publishing

The single level wildcard is **+**.

	itp/nano33/+
	itp/+/temperature
	
The multi level wildcard is **#**.

	#
	itp/#
	itp/nano33/#

## Publishing 

Write a message to the message topic

	mosquitto_pub -h itpdtd.com -t message -m 'hello, world'

Turn the LED on device_01 on
	
	mosquitto_pub -h itpdtd.com -t itp/device_01/led -m 100       
	
Turn the LED on device_01 off

	mosquitto_pub -h itpdtd.com -t itp/device_01/led -m 0       

Write a message to a long topic

	mosquitto_pub -h itpdtd.com -t itp/foo/bar/baz/ack -m yeet

The `-h` flag specifies the hostname of the broker. The `-t` flag specifies the topic to publish to. The `-m` flag specifies the message to publish.

## Documentation

 * [mosquitto_sub](https://mosquitto.org/man/mosquitto_sub-1.html) 
 * [mosquitto_pub](https://mosquitto.org/man/mosquitto_pub-1.html) 


	
