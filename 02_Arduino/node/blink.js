var mqtt = require('mqtt')
var config = require('./config');

// pass in device_id or get from config
var device = process.argv[2] || config.device;
console.log(`blinking ${device}`);

var client  = mqtt.connect(config.server, {
    username: config.username,
    password: config.password
});

client.on('connect', function () {
  on();
});

function on() {
  client.publish(`itp/${device}/led`, 'ON');
  setTimeout(off, 1000);
}

function off() {
  client.publish(`itp/${device}/led`, 'OFF');
  setTimeout(on, 1000);
}