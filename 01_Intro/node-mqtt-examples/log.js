const mqtt = require('mqtt');
const fs = require('fs');

const client  = mqtt.connect('mqtts://itpdtd.com');
const fileName = "./mqtt.log";

client.on('connect', () => {
  client.subscribe('#');
  client.publish('presence', 'MQTT Logger');
});

client.on('message', function (topic, message) {
	// remove linefeeds
	let payload = message.toString().replace(/\n/g, '');
	// tab separated values
  const row = `${new Date().getTime()}\t${topic}\t${payload}\n`;
  fs.writeFileSync(fileName, row, { flag: 'a' });
});