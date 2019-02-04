let client;
let broker = 'wss://mqtt.iotwork.shop:8083';

function connect() {
  console.log('Connecting');

  if (username.value.match(/device/)) {
    alert(`Only the Arduino should use the '${username.value}' account.\nTry your user account instead.`);
    return;
  }

  client = mqtt.connect(broker, {
    username: username.value,
    password: password.value 
  });

  client.on('connect', function () {    
    console.log('connected');
    connectButton.hidden = true;
    disconnectButton.hidden = false;
    radioButtons.disabled = false;
    document.querySelectorAll('input[type=text],input[type=password]').forEach(e => e.disabled = true);
  });

  client.on('error', function (message) {
    alert(message);
    client.end();
  });

  client.on('message', function (topic, message) {
    // message is Buffer
    console.log(topic, message.toString());
  })
}

function on() {
  client.publish(topic.value, 'ON');
  console.log('on');
}

function off() {
  client.publish(topic.value, 'OFF');
  console.log('off');
}

function disconnect() {
  if (client) {
    client.end();
  }
  connectButton.hidden = false;
  disconnectButton.hidden = true;
  radioButtons.disabled = true;
  document.querySelectorAll('input[type=text],input[type=password]').forEach(e => e.disabled = false);
}



