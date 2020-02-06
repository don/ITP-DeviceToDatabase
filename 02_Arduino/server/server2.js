const express = require('express');
const app = express();

const bodyParser = require("body-parser");
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

const fs = require('fs');
const logFileName = __dirname + '/data.json';

// GET returns the data file
app.get('/', function(request, response) {
  response.sendFile(logFileName);
});

// POST write data to the log
app.post("/", function (request, response) {
  // post data
  let data = request.body; 
  // add a timestamp
  let dt = new Date();
  data.timestamp = dt.getTime();
  // add the remote address
  let remoteAddress = request.headers['x-forwarded-for'] || request.connection.remoteAddress;
  data.remoteAddress = remoteAddress;
  
  fs.appendFile(logFileName, JSON.stringify(data) + '\n', err => {
    if(err) { throw err };
    console.log(data);
    response.send("OK");
  });
});

// GET /clear - clears the log file
app.get('/clear', function(request, response) {
  fs.writeFile(logFileName, '', err => {
    if(err) { throw err };
    response.send('ok\n');
  });
});

// listen for requests :)
const listener = app.listen(process.env.PORT || '3000', function() {
  fs.closeSync(fs.openSync(logFileName, 'w'));
  console.log('Server is listening on port ' + listener.address().port);
});
