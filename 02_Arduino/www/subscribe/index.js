let client;

function connect() {
  console.log('Connecting');

  if (username.value.match(/device/)) {
    alert(`Only the Arduino should use the '${username.value}' account.\nTry your user account instead.`);
    return;
  }

  client = mqtt.connect(broker.value, {
    username: username.value,
    password: password.value 
  });

  client.on('connect', function () { 
    pre.innerText = '';   
    console.log('connected');
    connectButton.hidden = true;
    disconnectButton.hidden = false;
    document.querySelectorAll('input[type=text],input[type=password]').forEach(e => e.disabled = true);
    client.subscribe(topic.value);
  })

  client.on('message', function (topic, message) {
    // message is Buffer so call toString before logging
    console.log(topic, message.toString()); 
    pre.innerText += '\n' + new Date() + '\t' + topic + '\t' + message;
  })

  client.on('error', function (message) {
      alert(message);
      client.end();
  })
}

function disconnect() {
  if (client) {
    client.end();
  }
  connectButton.hidden = false;
  disconnectButton.hidden = true;
  document.querySelectorAll('input[type=text],input[type=password]').forEach(e => e.disabled = false);
}

