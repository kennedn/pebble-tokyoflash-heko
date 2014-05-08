Pebble.addEventListener("ready", onPebbleReady);
Pebble.addEventListener("showConfiguration", onPebbleStartConfigure);
Pebble.addEventListener("webviewclosed", onPebbleEndConfigure);

function onPebbleReady() {
  
}

function onPebbleStartConfigure() {
  Pebble.openURL("http://pebble.ruirize.co.uk/heko/");
}
function onPebbleEndConfigure(data) {
  return;
}