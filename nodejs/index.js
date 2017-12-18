var express     = require('express');
var app         = express();
var bodyParser  = require('body-parser');

app.set('port', 8000)
app.set('view engine', 'pug')
app.use(express.static(__dirname + '/views'));
app.use('/scripts', express.static(__dirname + '/node_modules/vis/dist/'));

app.use(bodyParser.urlencoded({
    extended: true
}));
app.use(bodyParser.json());


// Visualize
app.get('/', function(req, res) {
    
        res.render('index');
	// res.render('index', { data: results });
    });

app.listen(app.get('port'));
console.info("Server started");
