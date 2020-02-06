const express = require('express');
const app = express();
const bodyParser = require("body-parser");

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

app.post("/", function (request, response) {
  // posted data is in the body
  let data = request.body; 
  // add a timestamp
  let dt = new Date();
  data.timestamp = dt.getTime();
  // add the remote address
  let remoteAddress = request.headers['x-forwarded-for'] || request.connection.remoteAddress;
  data.remoteAddress = remoteAddress;
  
  console.log(data);
  response.send();
});

// listen for requests :)
const listener = app.listen(process.env.PORT || '3000', function() {
  console.log('Server is listening on port ' + listener.address().port);
});
