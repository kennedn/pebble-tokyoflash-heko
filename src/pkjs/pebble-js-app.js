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
  
  pebbleConfig["to_outline"] = parseInt(configuration.to.outline);
  pebbleConfig["to_fill"] = parseInt(configuration.to.fill);
  pebbleConfig["to_accumulate"] = parseInt(configuration.to.accumulate);
  pebbleConfig["to_invert"] = parseInt(configuration.to.invert);
  pebbleConfig["ti_outline"] = parseInt(configuration.ti.outline);
  pebbleConfig["ti_fill"] = parseInt(configuration.ti.fill);
  pebbleConfig["ti_accumulate"] = parseInt(configuration.ti.accumulate);
  pebbleConfig["ti_invert"] = parseInt(configuration.ti.invert);
  pebbleConfig["c_outline"] = parseInt(configuration.c.outline);
  pebbleConfig["c_fill"] = parseInt(configuration.c.fill);
  pebbleConfig["c_accumulate"] = parseInt(configuration.c.accumulate);
  pebbleConfig["c_invert"] = parseInt(configuration.c.invert);
  pebbleConfig["bo_outline"] = parseInt(configuration.bo.outline);
  pebbleConfig["bo_fill"] = parseInt(configuration.bo.fill);
  pebbleConfig["bo_accumulate"] = parseInt(configuration.bo.accumulate);
  pebbleConfig["bo_invert"] = parseInt(configuration.bo.invert);
  pebbleConfig["bi_outline"] = parseInt(configuration.bi.outline);
  pebbleConfig["bi_fill"] = parseInt(configuration.bi.fill);
  pebbleConfig["bi_accumulate"] = parseInt(configuration.bi.accumulate);
  pebbleConfig["bi_invert"] = parseInt(configuration.bi.invert);
  pebbleConfig["top_bg"] = parseInt(configuration.top_bg);
  pebbleConfig["center_bg"] = parseInt(configuration.center_bg);
  pebbleConfig["bottom_bg"] = parseInt(configuration.bottom_bg);
  pebbleConfig["display_dividers"] = parseInt(configuration.display_dividers);
  pebbleConfig["vibe_disconnect"] = parseInt(configuration.vibe_disconnect);
  pebbleConfig["vibe_connect"] = parseInt(configuration.vibe_connect);
  pebbleConfig["vibe_hour"] = parseInt(configuration.vibe_hour);
  
  Pebble.sendAppMessage(pebbleConfig, function(e){console.log("Received fine!");}, function(e){console.log("failed! " + e.error.message);});
}