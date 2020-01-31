const mqtt = require('mqtt');
const client = mqtt.connect('mqtts://itpdtd.com');

client.on('connect', () => {
  client.publish('itp/device_01/led', 'on');
  client.end();
});
