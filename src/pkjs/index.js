var Clay = require('pebble-clay');
var clayConfig = require('./config.json');
var customClay = require('./custom-clay');
var clay = new Clay(clayConfig, customClay);

Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});
