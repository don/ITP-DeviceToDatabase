const mqtt = require('mqtt')

const device = 'device_01';
const client = mqtt.connect('mqtts://itpdtd.com');

client.on('connect', on);

function on() {
  client.publish(`itp/${device}/led`, 'on');
  setTimeout(off, 1000);
}

function off() {
  client.publish(`itp/${device}/led`, 'off');
  setTimeout(on, 1000);
}