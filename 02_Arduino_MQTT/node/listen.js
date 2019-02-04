var mqtt = require('mqtt');
var config = require('./config');

var client = mqtt.connect(config.server, {
    username: config.username,
    password: config.password
});

client.on('connect', function () {
  console.log("connected");
  // # is the wildcard to match any devices
  client.subscribe("\#");
  // + is the wildcard to match part of a topic
  // get temperature for workshop devices
  //client.subscribe("itp/+/temperature");
  // get all messages for device1
  //client.subscribe("itp/device_xx/+");
});

client.on('message', function (topic, message) {
  // message is Buffer
  console.log(topic, message.toString());
});