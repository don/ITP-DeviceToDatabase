var mqtt = require('mqtt')
var config = require('./config');

var client  = mqtt.connect(config.server, {
    username: config.username,
    password: config.password
});

client.on('connect', function () {
  on();
});

function on() {
  client.publish(`itp/${config.device}/led`, 'ON');
  setTimeout(off, 1000);
}

function off() {
  client.publish(`itp/${config.device}/led`, 'OFF');
  setTimeout(on, 1000);
}