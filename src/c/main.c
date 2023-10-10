/*
 WATCHFACE ANATOMY
         ╔═════════╗
         ║    0    ║ 0-3: Cardinal Hours / Months (TO)
         ║ 3 4 5 1 ║ 4-5: +1 Hours / Months (TI)
 Top_____║    2    ║
 Center__║ 6 7 8 9 ║ 6-9: +1 Minutes / Days (C)
 Bottom  ║    A    ║ A-D: Cardinal Minutes / Days (BO)
         ║ D E F B ║ E-F: +5 Minutes / Days (BI)
         ║    C    ║   D: Date Indicator
         ╚═════════╝
*/

#include "pebble.h"
#include "constants.h"
#include "structs.h"

static Window *window;

static Layer *display_layer;

static GBitmap *led_outline;
static GBitmap *led_halftone;
static GBitmap *led_solid;
static GBitmap *led_none;

static LEDStyle cfg_leds_top_outer;
static LEDStyle cfg_leds_top_inner;
static LEDStyle cfg_leds_center;
static LEDStyle cfg_leds_bottom_outer;
static LEDStyle cfg_leds_bottom_inner;
static bool cfg_top_bg;
static bool cfg_center_bg;
static bool cfg_bottom_bg;
static bool cfg_display_dividers;
static uint8_t cfg_vibe_disconnect;
static uint8_t cfg_vibe_connect;
static uint8_t cfg_vibe_hour;

static bool cfg_needs_save;

static GBitmap * get_led_bitmap(uint8_t fill) {
  switch(fill) {
    case LED_HALFTONE: return led_halftone;
    case LED_SOLID: return led_solid;
    default: return led_none;
  }
}

static void draw_led(GContext * ctx, char led, bool on, bool bg, LEDStyle style) {
  GPoint led_pos;
  switch(led) {
    case '0': led_pos = POS_0; break;
    case '1': led_pos = POS_1; break;
    case '2': led_pos = POS_2; break;
    case '3': led_pos = POS_3; break;
    case '4': led_pos = POS_4; break;
    case '5': led_pos = POS_5; break;
    case '6': led_pos = POS_6; break;
    case '7': led_pos = POS_7; break;
    case '8': led_pos = POS_8; break;
    case '9': led_pos = POS_9; break;
    case 'a': led_pos = POS_A; break;
    case 'b': led_pos = POS_B; break;
    case 'c': led_pos = POS_C; break;
    case 'd': led_pos = POS_D; break;
    case 'e': led_pos = POS_E; break;
    case 'f': led_pos = POS_F; break;
  }
  
  graphics_context_set_compositing_mode(ctx,bg?GCompOpClear:GCompOpAssign);
  graphics_draw_bitmap_in_rect(ctx,on?led_solid:get_led_bitmap(style.fill),(GRect){.origin=led_pos,.size=SIZE_LED});
  if (style.outline == OL_NONE) return;
  
  graphics_context_set_compositing_mode(ctx,style.outline==OL_ADD?(bg?GCompOpClear:GCompOpOr):(bg?GCompOpOr:GCompOpClear));
  graphics_draw_bitmap_in_rect(ctx,led_outline,(GRect){.origin=led_pos,.size=SIZE_LED});
}

static void draw_face(GContext *ctx, bool top_bg,bool center_bg,bool bottom_bg,
  LEDStyle s_to,LEDStyle s_ti,LEDStyle s_c,LEDStyle s_bo,LEDStyle s_bi,
  int leds_to,int leds_ti,int leds_c,int leds_bo,int leds_bi,
  bool ignore_accumulate) {
  bool led_0=false,led_1=false,led_2=false,led_3=false,
        led_4=false,led_5=false,
       led_6=false,led_7=false,led_8=false,led_9=false,
       led_a=false,led_b=false,led_c=false,led_d=false,
        led_e=false,led_f=false;
  
  switch(leds_to) {
    case 0: led_0 = true; break;
    case 1: 
    if (!ignore_accumulate && s_to.accumulative) {
      led_0 = true;
    }
    led_1 = true; break;
    case 2: 
    if (!ignore_accumulate && s_to.accumulative) {
      led_0 = true;
      led_1 = true;
    }
    led_2 = true; break;
    case 3:
    if (!ignore_accumulate && s_to.accumulative) {
      led_0 = true;
      led_1 = true;
      led_2 = true;
    }
    led_3 = true; break;
  }
  switch(leds_ti) {
    case 1: led_4 = true; break;
    case 2:
    if (!ignore_accumulate && s_ti.accumulative) {
      led_4 = true;
    }
    led_5 = true;
    break;
  }
  switch(leds_c) {
    case 0: led_6 = true; break;
    case 1: 
    if (!ignore_accumulate && s_c.accumulative) {
      led_6 = true;
    }
    led_7 = true; break;
    case 2: 
    if (!ignore_accumulate && s_c.accumulative) {
      led_6 = true;
      led_7 = true;
    }
    led_8 = true; break;
    case 3:
    if (!ignore_accumulate && s_c.accumulative) {
      led_6 = true;
      led_7 = true;
      led_8 = true;
    }
    led_9 = true; break;
  }
  switch(leds_bo) {
    case 0: led_a = true; break;
    case 1: 
    if (!ignore_accumulate && s_bo.accumulative) {
      led_a = true;
    }
    led_b = true; break;
    case 2: 
    if (!ignore_accumulate && s_bo.accumulative) {
      led_a = true;
      led_b = true;
    }
    led_c = true; break;
    case 3:
    if (!ignore_accumulate && s_bo.accumulative) {
      led_a = true;
      led_b = true;
      led_c = true;
    }
    led_d = true; break;
  }
  switch(leds_bi) {
    case 1: led_e = true; break;
    case 2:
    if (!ignore_accumulate && s_bi.accumulative) {
      led_e = true;
    }
    led_f = true;
    break;
  }

  if (s_to.invert) {
    led_0 = !led_0;
    led_1 = !led_1;
    led_2 = !led_2;
    led_3 = !led_3;
  }
  if (s_ti.invert) {
    led_4 = !led_4;
    led_5 = !led_5;
  }
  if (s_c.invert) {
    led_6 = !led_6;
    led_7 = !led_7;
    led_8 = !led_8;
    led_9 = !led_9;
  }
  if (s_bo.invert) {
    led_a = !led_a;
    led_b = !led_b;
    led_c = !led_c;
    led_d = !led_d;
  }
  if (s_bi.invert) {
    led_e = !led_e;
    led_f = !led_f;
  }
  
  // Top
  graphics_context_set_fill_color(ctx,top_bg?GColorWhite:GColorBlack);
  graphics_fill_rect(ctx,(GRect){.origin={0,0},.size={144,72}},0,GCornerNone);
  // Center
  graphics_context_set_fill_color(ctx,center_bg?GColorWhite:GColorBlack);
  graphics_fill_rect(ctx,(GRect){.origin={0,72},.size={144,24}},0,GCornerNone);
  // Bottom
  graphics_context_set_fill_color(ctx,bottom_bg?GColorWhite:GColorBlack);
  graphics_fill_rect(ctx,(GRect){.origin={0,96},.size={144,72}},0,GCornerNone);
  
  draw_led(ctx,'0',led_0,top_bg,s_to);
  draw_led(ctx,'1',led_1,top_bg,s_to);
  draw_led(ctx,'2',led_2,top_bg,s_to);
  draw_led(ctx,'3',led_3,top_bg,s_to);
  draw_led(ctx,'4',led_4,top_bg,s_ti);
  draw_led(ctx,'5',led_5,top_bg,s_ti);
  draw_led(ctx,'6',led_6,center_bg,s_c);
  draw_led(ctx,'7',led_7,center_bg,s_c);
  draw_led(ctx,'8',led_8,center_bg,s_c);
  draw_led(ctx,'9',led_9,center_bg,s_c);
  draw_led(ctx,'a',led_a,bottom_bg,s_bo);
  draw_led(ctx,'b',led_b,bottom_bg,s_bo);
  draw_led(ctx,'c',led_c,bottom_bg,s_bo);
  draw_led(ctx,'d',led_d,bottom_bg,s_bo);
  draw_led(ctx,'e',led_e,bottom_bg,s_bi);
  draw_led(ctx,'f',led_f,bottom_bg,s_bi);
  graphics_context_set_compositing_mode(ctx,GCompOpAssign);
  
  // Dividers
  if (cfg_display_dividers && top_bg == center_bg) {
    graphics_context_set_fill_color(ctx,top_bg?GColorBlack:GColorWhite);
    graphics_fill_rect(ctx,(GRect){.origin={0,71},.size={144,1}},0,GCornerNone);
  }
  if (cfg_display_dividers && bottom_bg == center_bg) {
    graphics_context_set_fill_color(ctx,bottom_bg?GColorBlack:GColorWhite);
    graphics_fill_rect(ctx,(GRect){.origin={0,96},.size={144,1}},0,GCornerNone);
  }
}

static void display_layer_update_callback(Layer *me, GContext* ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  int minutes = t->tm_min;
  int hours = t->tm_hour;
  if (hours > 12) hours -= 12;
  
  int card_minutes = minutes / 15;
  int inner_minutes = (minutes - (card_minutes * 15)) / 5;
  int center_minutes = (minutes % 5) - 1;
  
  int card_hours = hours / 3;
  int inner_hours = hours - (card_hours * 3);
  
  draw_face(ctx,cfg_top_bg,cfg_center_bg,cfg_bottom_bg,
  cfg_leds_top_outer,cfg_leds_top_inner,cfg_leds_center,cfg_leds_bottom_outer,cfg_leds_bottom_inner,
  card_hours,inner_hours,center_minutes,card_minutes,inner_minutes,false);
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(display_layer);
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
  // Sadly nothing we can do. We only accept messages from the config window.
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *to_outline = dict_find(iter, JS_TO_OUTLINE);
  Tuple *to_fill = dict_find(iter, JS_TO_FILL);
  Tuple *to_accumulate = dict_find(iter, JS_TO_ACCUMULATE);
  Tuple *to_invert = dict_find(iter, JS_TO_INVERT);
  Tuple *ti_outline = dict_find(iter, JS_TI_OUTLINE);
  Tuple *ti_fill = dict_find(iter, JS_TI_FILL);
  Tuple *ti_accumulate = dict_find(iter, JS_TI_ACCUMULATE);
  Tuple *ti_invert = dict_find(iter, JS_TI_INVERT);
  Tuple *c_outline = dict_find(iter, JS_C_OUTLINE);
  Tuple *c_fill = dict_find(iter, JS_C_FILL);
  Tuple *c_accumulate = dict_find(iter, JS_C_ACCUMULATE);
  Tuple *c_invert = dict_find(iter, JS_C_INVERT);
  Tuple *bo_outline = dict_find(iter, JS_BO_OUTLINE);
  Tuple *bo_fill = dict_find(iter, JS_BO_FILL);
  Tuple *bo_accumulate = dict_find(iter, JS_BO_ACCUMULATE);
  Tuple *bo_invert = dict_find(iter, JS_BO_INVERT);
  Tuple *bi_outline = dict_find(iter, JS_BI_OUTLINE);
  Tuple *bi_fill = dict_find(iter, JS_BI_FILL);
  Tuple *bi_accumulate = dict_find(iter, JS_BI_ACCUMULATE);
  Tuple *bi_invert = dict_find(iter, JS_BI_INVERT);
  Tuple *top_bg = dict_find(iter, JS_TOP_BG);
  Tuple *center_bg = dict_find(iter, JS_CENTER_BG);
  Tuple *bottom_bg = dict_find(iter, JS_BOTTOM_BG);
  Tuple *display_dividers = dict_find(iter, JS_DISPLAY_DIVIDERS);
  Tuple *vibe_disconnect = dict_find(iter, JS_VIBE_DISCONNECT);
  Tuple *vibe_connect = dict_find(iter, JS_VIBE_CONNECT);
  Tuple *vibe_hour = dict_find(iter, JS_VIBE_HOUR);
  
  if (to_outline) { cfg_leds_top_outer.outline = to_outline->value->uint8; }
  if (ti_outline) { cfg_leds_top_inner.outline = ti_outline->value->uint8; }
  if (c_outline) { cfg_leds_center.outline = c_outline->value->uint8; }
  if (bo_outline) { cfg_leds_bottom_outer.outline = bo_outline->value->uint8; }
  if (bi_outline) { cfg_leds_bottom_inner.outline = bi_outline->value->uint8; }
  
  if (to_fill) { cfg_leds_top_outer.fill = to_fill->value->uint8; }
  if (ti_fill) { cfg_leds_top_inner.fill = ti_fill->value->uint8; }
  if (c_fill) { cfg_leds_center.fill = c_fill->value->uint8; }
  if (bo_fill) { cfg_leds_bottom_outer.fill = bo_fill->value->uint8; }
  if (bi_fill) { cfg_leds_bottom_inner.fill = bi_fill->value->uint8; }
  
  if (to_accumulate) { cfg_leds_top_outer.accumulative = to_accumulate->value->int8 == 1 ? true : false; }
  if (ti_accumulate) { cfg_leds_top_inner.accumulative = ti_accumulate->value->int8 == 1 ? true : false; }
  if (c_accumulate) { cfg_leds_center.accumulative = c_accumulate->value->int8 == 1 ? true : false; }
  if (bo_accumulate) { cfg_leds_bottom_outer.accumulative = bo_accumulate->value->int8 == 1 ? true : false; }
  if (bi_accumulate) { cfg_leds_bottom_inner.accumulative = bi_accumulate->value->int8 == 1 ? true : false; }
  
  if (to_invert) { cfg_leds_top_outer.invert = to_invert->value->int8 == 1 ? true : false; }
  if (ti_invert) { cfg_leds_top_inner.invert = ti_invert->value->int8 == 1 ? true : false; }
  if (c_invert) { cfg_leds_center.invert = c_invert->value->int8 == 1 ? true : false; }
  if (bo_invert) { cfg_leds_bottom_outer.invert = bo_invert->value->int8 == 1 ? true : false; }
  if (bi_invert) { cfg_leds_bottom_inner.invert = bi_invert->value->int8 == 1 ? true : false; }
  
  if (top_bg) { cfg_top_bg = top_bg->value->int8 == 1 ? true : false; }
  if (center_bg) { cfg_center_bg = center_bg->value->int8 == 1 ? true : false; }
  if (bottom_bg) { cfg_bottom_bg = bottom_bg->value->int8 == 1 ? true : false; }
  if (display_dividers) { cfg_display_dividers = display_dividers->value->int8 == 1 ? true : false; }
  
  if (vibe_disconnect) { cfg_vibe_disconnect = vibe_disconnect->value->uint8; }
  if (vibe_connect) { cfg_vibe_connect = vibe_connect->value->uint8; }
  if (vibe_hour) { cfg_vibe_hour = vibe_hour->value->uint8; }
  
  cfg_needs_save = true;
  layer_mark_dirty(display_layer);
}

static void do_init(void) {
  window = window_create();
  window_stack_push(window, true);

  window_set_background_color(window, GColorBlack);

  Layer *root_layer = window_get_root_layer(window);
  GRect frame = layer_get_frame(root_layer);

  led_outline = gbitmap_create_with_resource(RESOURCE_ID_LED_OUTLINE);
  led_halftone = gbitmap_create_with_resource(RESOURCE_ID_LED_HALFTONE);
  led_solid = gbitmap_create_with_resource(RESOURCE_ID_LED_SOLID);
  led_none = gbitmap_create_with_resource(RESOURCE_ID_LED_NONE);
  
  if (persist_exists(ST_TOP_OUTER_LEDS)) {
    persist_read_data(ST_TOP_OUTER_LEDS, &cfg_leds_top_outer, sizeof(cfg_leds_top_outer));
  } else {
    cfg_leds_top_outer = (LEDStyle){
      .outline = OL_NONE,
      .fill = LED_HALFTONE,
      .accumulative = false,
      .invert = false
    };
  }
  if (persist_exists(ST_TOP_INNER_LEDS)) {
    persist_read_data(ST_TOP_INNER_LEDS, &cfg_leds_top_inner, sizeof(cfg_leds_top_inner));
  } else {
    cfg_leds_top_inner = (LEDStyle){
      .outline = OL_REMOVE,
      .fill = LED_HALFTONE,
      .accumulative = true,
      .invert = false
    };
  }
  
  if (persist_exists(ST_CENTER_LEDS)) {
    persist_read_data(ST_CENTER_LEDS, &cfg_leds_center, sizeof(cfg_leds_center));
  } else {
    cfg_leds_center = (LEDStyle){
      .outline = OL_NONE,
      .fill = LED_HALFTONE,
      .accumulative = true,
      .invert = false
    };
  }
  
  if (persist_exists(ST_BOTTOM_OUTER_LEDS)) {
    persist_read_data(ST_BOTTOM_OUTER_LEDS, &cfg_leds_bottom_outer, sizeof(cfg_leds_bottom_outer));
  } else {
    cfg_leds_bottom_outer = (LEDStyle){
      .outline = OL_NONE,
      .fill = LED_HALFTONE,
      .accumulative = false,
      .invert = false
    };
  }
  if (persist_exists(ST_BOTTOM_INNER_LEDS)) {
    persist_read_data(ST_BOTTOM_INNER_LEDS, &cfg_leds_bottom_inner, sizeof(cfg_leds_bottom_inner));
  } else {
    cfg_leds_bottom_inner = (LEDStyle){
      .outline = OL_REMOVE,
      .fill = LED_HALFTONE,
      .accumulative = true,
      .invert = false
    };
  }
  cfg_top_bg = persist_exists(ST_TOP_BG) ? persist_read_bool(ST_TOP_BG) : false;
  cfg_center_bg = persist_exists(ST_CENTER_BG) ? persist_read_bool(ST_CENTER_BG) : true;
  cfg_bottom_bg = persist_exists(ST_BOTTOM_BG) ? persist_read_bool(ST_BOTTOM_BG) : false;
  cfg_display_dividers = persist_exists(ST_DISPLAY_DIVIDERS) ? persist_read_bool(ST_DISPLAY_DIVIDERS) : true;
  cfg_vibe_disconnect = persist_exists(ST_VIBE_DISCONNECT) ? persist_read_int(ST_VIBE_DISCONNECT) : VIBE_NONE;
  cfg_vibe_connect = persist_exists(ST_VIBE_CONNECT) ? persist_read_int(ST_VIBE_CONNECT) : VIBE_NONE;
  cfg_vibe_hour = persist_exists(ST_VIBE_HOUR) ? persist_read_int(ST_VIBE_HOUR) : VIBE_NONE;
  
  // Init the layer for the display
  display_layer = layer_create(frame);
  layer_set_update_proc(display_layer, &display_layer_update_callback);
  layer_add_child(root_layer, display_layer);

  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
	app_message_open(app_message_inbox_size_maximum(), app_message_inbox_size_maximum());
}

static void do_deinit(void) {
  gbitmap_destroy(led_outline);
  gbitmap_destroy(led_halftone);
  gbitmap_destroy(led_solid);
  gbitmap_destroy(led_none);

  layer_destroy(display_layer);
  window_destroy(window);
  
  if (cfg_needs_save) {
    persist_write_data(ST_TOP_OUTER_LEDS, &cfg_leds_top_outer, sizeof(cfg_leds_top_outer));
    persist_write_data(ST_TOP_INNER_LEDS, &cfg_leds_top_inner, sizeof(cfg_leds_top_inner));
    persist_write_data(ST_CENTER_LEDS, &cfg_leds_center, sizeof(cfg_leds_center));
    persist_write_data(ST_BOTTOM_OUTER_LEDS, &cfg_leds_bottom_outer, sizeof(cfg_leds_bottom_outer));
    persist_write_data(ST_BOTTOM_INNER_LEDS, &cfg_leds_bottom_inner, sizeof(cfg_leds_bottom_inner));
      
    persist_write_bool(ST_TOP_BG,cfg_top_bg);
    persist_write_bool(ST_CENTER_BG,cfg_center_bg);
    persist_write_bool(ST_BOTTOM_BG,cfg_bottom_bg);
    persist_write_bool(ST_DISPLAY_DIVIDERS,cfg_display_dividers);
    persist_write_int(ST_VIBE_DISCONNECT, cfg_vibe_disconnect);
    persist_write_int(ST_VIBE_CONNECT, cfg_vibe_connect);
    persist_write_int(ST_VIBE_HOUR, cfg_vibe_hour);
  }
}

int main(void) {
  do_init();
  app_event_loop();
  do_deinit();
}