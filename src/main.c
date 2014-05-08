/*
 WATCHFACE ANATOMY:
 
 


*/

#include "pebble.h"
#include "constants.h"
#include "structs.h"

static Window *window;

static Layer *display_layer;

static GBitmap *led_outline;
static GBitmap *led_halftone;
static GBitmap *led_solid;

static void display_layer_update_callback(Layer *me, GContext* ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  
  GRect bounds = led_solid->bounds;
  
  // Minutes Outer
  int minutes = t->tm_min / 15;
  
  if (t->tm_min == 0) {
    graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={66,102},.size=bounds.size});
  }
  
  switch(minutes) {
    case 1: graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={124,126},.size=bounds.size}); break;
    case 2: graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={66,150},.size=bounds.size}); break;
    case 3: graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={18,126},.size=bounds.size}); break;
  }
  
  // Minutes Inner
  int normal_minutes = t->tm_min - (minutes * 15);
  if (normal_minutes >= 5) {
    graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={48,126},.size=bounds.size});
  }
  if (normal_minutes >= 10) {
    graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={84,126},.size=bounds.size});
  }
  
  // Center
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, (GRect){.origin={0,72},.size={144,24}}, 0, GCornerNone);
  
  graphics_context_set_compositing_mode(ctx, GCompOpClear);
  
  int minutes_to_display = t->tm_min % 5;
  if (minutes_to_display > 0) {
    graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={30,78},.size=bounds.size});
  }
  if (minutes_to_display > 1) {
    graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={54,78},.size=bounds.size});
  }
  if (minutes_to_display > 2) {
    graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={78,78},.size=bounds.size});
  }
  if (minutes_to_display > 3) {
    graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={102,78},.size=bounds.size});
  }
  
  graphics_context_set_compositing_mode(ctx, GCompOpAssign);

  // Hours Outer
  int hours = t->tm_hour;
  if (hours > 12) hours = hours / 2;
  hours = hours / 3;
  
  if (t->tm_hour == 0) {
    graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={66,6},.size=bounds.size});
  }
  
  switch(hours) {
    case 1: graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={124,30},.size=bounds.size}); break;
    case 2: graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={66,54},.size=bounds.size}); break;
    case 3: graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={18,30},.size=bounds.size}); break;
  }

  // Hours Inner
  hours = t->tm_hour;
  if (hours > 12) hours = hours / 2;
  int hours_to_display = hours % 3;
  if (hours_to_display > 0) {
    graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={48,30},.size=bounds.size});
  }
  if (hours_to_display > 1) {
    graphics_draw_bitmap_in_rect(ctx, led_solid, (GRect){.origin={84,30},.size=bounds.size});
  }
}

static void draw_face()

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
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
  
  // Init the layer for the display
  display_layer = layer_create(frame);
  layer_set_update_proc(display_layer, &display_layer_update_callback);
  layer_add_child(root_layer, display_layer);

  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);
}

static void do_deinit(void) {
  gbitmap_destroy(led_outline);
  gbitmap_destroy(led_halftone);
  gbitmap_destroy(led_solid);

  layer_destroy(display_layer);
  window_destroy(window);
}

int main(void) {
  do_init();
  app_event_loop();
  do_deinit();
}