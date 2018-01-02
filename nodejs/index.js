var express     = require('express');
var app         = express();
var bodyParser  = require('body-parser');
var server = require('http').createServer(app);
var io = require('socket.io')(server);
var port = 8000;
var mqtt = require('mqtt');
/*var options = {
  port: 8883,
  host: '192.168.178.56'
};*/
var client  = mqtt.connect('mqtt://192.168.1.31');

app.use(express.static(__dirname + '/views'));
app.use('/scripts', express.static(__dirname + '/node_modules/vis/dist/'));

app.use(bodyParser.urlencoded({
    extended: true
}));
app.use(bodyParser.json());


client.on('connect', function () {
	console.log("Connected to mqtt broker");
  client.subscribe('sensor/humidity');
  client.subscribe('sensor/distance');
  client.subscribe('sensor/temperature');
})

client.on('message', function (topic, message) {
  // message is Buffer
  console.log(String(topic) + ": " + String(message));
  io.emit('mqtt', {'topic': String(topic), data: String(message)});
})

// Visualize
app.get('/', function(req, res) {
	res.render('index', function(err, html) {
  		res.send(html);
	});
	// res.render('index', { data: results });
    });


server.listen(port);
//app.listen(app.get('port'));
console.info("Server started");
