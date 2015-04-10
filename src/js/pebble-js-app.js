Pebble.addEventListener("ready", onPebbleReady);
Pebble.addEventListener("showConfiguration", onPebbleStartConfigure);
Pebble.addEventListener("webviewclosed", onPebbleEndConfigure);

function onPebbleReady() {
  
}

function onPebbleStartConfigure() {
  Pebble.openURL("http://pebble.ruirize.co.uk/heko/");
}
function onPebbleEndConfigure(e) {
  if (e.response === "CANCELLED" || e.response === "") return;

  var configuration = JSON.parse(decodeURIComponent(e.response));
  
  var pebbleConfig = {};
  
  pebbleConfig["to-outline"] = parseInt(configuration.to.outline);
  pebbleConfig["to-fill"] = parseInt(configuration.to.fill);
  pebbleConfig["to-accumulate"] = parseInt(configuration.to.accumulate);
  pebbleConfig["to-invert"] = parseInt(configuration.to.invert);
  pebbleConfig["ti-outline"] = parseInt(configuration.ti.outline);
  pebbleConfig["ti-fill"] = parseInt(configuration.ti.fill);
  pebbleConfig["ti-accumulate"] = parseInt(configuration.ti.accumulate);
  pebbleConfig["ti-invert"] = parseInt(configuration.ti.invert);
  pebbleConfig["c-outline"] = parseInt(configuration.c.outline);
  pebbleConfig["c-fill"] = parseInt(configuration.c.fill);
  pebbleConfig["c-accumulate"] = parseInt(configuration.c.accumulate);
  pebbleConfig["c-invert"] = parseInt(configuration.c.invert);
  pebbleConfig["bo-outline"] = parseInt(configuration.bo.outline);
  pebbleConfig["bo-fill"] = parseInt(configuration.bo.fill);
  pebbleConfig["bo-accumulate"] = parseInt(configuration.bo.accumulate);
  pebbleConfig["bo-invert"] = parseInt(configuration.bo.invert);
  pebbleConfig["bi-outline"] = parseInt(configuration.bi.outline);
  pebbleConfig["bi-fill"] = parseInt(configuration.bi.fill);
  pebbleConfig["bi-accumulate"] = parseInt(configuration.bi.accumulate);
  pebbleConfig["bi-invert"] = parseInt(configuration.bi.invert);
  pebbleConfig["top-bg"] = parseInt(configuration.top_bg);
  pebbleConfig["center-bg"] = parseInt(configuration.center_bg);
  pebbleConfig["bottom-bg"] = parseInt(configuration.bottom_bg);
  pebbleConfig["display-dividers"] = parseInt(configuration.display_dividers);
  pebbleConfig["vibe-disconnect"] = parseInt(configuration.vibe_disconnect);
  pebbleConfig["vibe-connect"] = parseInt(configuration.vibe_connect);
  pebbleConfig["vibe-hour"] = parseInt(configuration.vibe_hour);
  
  Pebble.sendAppMessage(pebbleConfig, function(e){console.log("Received fine!");}, function(e){console.log("failed! " + e.error.message);});
}