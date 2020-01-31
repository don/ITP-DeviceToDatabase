const mqtt = require('mqtt');
const client = mqtt.connect('mqtts://itpdtd.com');

client.on('connect', () => {
  console.log("connected");
  client.subscribe("itp/device_01/temperature");
});

client.on('message', (topic, message) => {
  console.log(topic, message.toString());
});