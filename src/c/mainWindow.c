//  This file mainly deals with the visuals of the watchface and setting the time and health
//  data.

#include <pebble.h>
#include "mainWindow.h"
#include "mgs_health.h"
#include "Images.h"
#include "Texts.h"

// Layer pointers and GBitmap pointers to stick in thin Bitmap Layers
static Window *s_window;
static Layer *canvas;
//static TextLayer * ptt_layer;
//static TextLayer * max_layer;
static GBitmap * snake_pic;
static BitmapLayer * snake_layer;
static GBitmap * mei_ling_pic;
static BitmapLayer * mei_ling_layer;
static GBitmap * STEP_pic;
static BitmapLayer * STEP_layer;
static GBitmap * DISTANCE_pic;
static BitmapLayer * DISTANCE_layer;
static GBitmap * CALL_pic;
static BitmapLayer * CALL_layer;

#define NUM_LARGE_TEXT 1
#define NUM_SMALL_TEXT 3
struct Images *Image_holder;
struct Texts *Text_holder_large;
struct Texts *Text_holder_small;

// temporary Indices for accessing the text_structs
#define STEPS 0
#define DISTANCE 1

// Font variables
static GFont digital_font_30;
static GFont digital_font_15;

/* Data for GRects:
top_panel
bottom_panel
memory_panel
signal_rect_left
signal_bar_short
signal_bar_long
signal_bar_medium
*/
uint8_t panel_data[7][4] =
{
  {5,  5,   134, 80},
  {5,  100, 134, 60},
  {52, 87,  40,  11},
  {15, 22,  2,   6},
  {19, 0,   30,  6},
  {19, 22,  108, 6},
  {19, 30,  60,  6}
};

// Draws the background elements onto canvas_layer.
static void canvas_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_color(ctx, GColorBrightGreen);
  graphics_context_set_fill_color(ctx, GColorBrightGreen);
  graphics_context_set_stroke_width(ctx, 1);

  // text layer rectangle visual.
  //graphics_draw_rect(ctx, GRect(52, 120, 45, 20));

  GRect top_panel = GRect(panel_data[0][0], panel_data[0][1], panel_data[0][2], panel_data[0][3]);
  graphics_draw_rect(ctx, top_panel);

  GRect bottom_panel = GRect(panel_data[1][0], panel_data[1][1], panel_data[1][2], panel_data[1][3]);
  graphics_draw_rect(ctx, bottom_panel);

  GRect memory_panel = GRect(panel_data[2][0], panel_data[2][1], panel_data[2][2], panel_data[2][3]);
  graphics_fill_rect(ctx, memory_panel, 0, GCornerNone);

  // Draw the small rectangles to the left of the signal bars.
  // The stored y coordinate for signal_rect_left is just for the uppermost unit. This y-coordinate value will be incremented by 8 for each unit.
  uint8_t signal_rect_left_y_coord = panel_data[3][1];
  for (int i = 0; i < 7; i++) {
    GRect signal_rect_left = GRect(panel_data[3][0], signal_rect_left_y_coord, panel_data[3][2], panel_data[3][3]);
    graphics_fill_rect(ctx, signal_rect_left, 0, GCornerNone);

    // Draw the short bars while we're here at the correct y-coordinates.
    if (i >= 2) {
      GRect signal_bar_short = GRect(panel_data[4][0], signal_rect_left_y_coord, panel_data[4][2], panel_data[4][3]);
      graphics_draw_rect(ctx, signal_bar_short);
    }

    signal_rect_left_y_coord += 8;
  }

  GRect signal_bar_long = GRect(panel_data[5][0], panel_data[5][1], panel_data[5][2], panel_data[5][3]);
  graphics_draw_rect(ctx, signal_bar_long);

  GRect signal_bar_medium = GRect(panel_data[6][0], panel_data[6][1], panel_data[6][2], panel_data[6][3]);
  graphics_draw_rect(ctx, signal_bar_medium);

  //TODO: check battery percentage. Each bar is 15 percent except the bottom which is 10 percent
  int percentage_per_bar = 100 / 7;
  BatteryChargeState state = battery_state_service_peek();
  if ((int)state.charge_percent >= percentage_per_bar * 7)
    graphics_fill_rect(ctx, signal_bar_long, 0, GCornerNone);
  if ((int)state.charge_percent >= percentage_per_bar * 6)
    graphics_fill_rect(ctx, signal_bar_medium, 0, GCornerNone);

  uint8_t signal_bar_short_y_coord = 39;
  for (int i = 5; i > 0; i--)
  {
    if ((int)state.charge_percent >= percentage_per_bar * i)
    {
      GRect signal_bar_short = GRect(panel_data[4][0], signal_bar_short_y_coord, panel_data[4][2], panel_data[4][3]);
      graphics_fill_rect(ctx, signal_bar_short, 0, GCornerNone);
    }

    signal_bar_short_y_coord += 8;
  }
}


// Change the step text layer to show the new steps.
void set_health_info_text() {
  int steps = 0;
  steps = get_steps();
  static char steps_buffer[7];
  snprintf(steps_buffer, sizeof(steps_buffer), "%d", steps);
  //update_text(Text_holder_small->text_array[STEPS], steps_buffer);

  int distance_walked = 0;
  distance_walked = get_distance_walked();
  static char distance_buffer[8];
  snprintf(distance_buffer, sizeof(distance_buffer), "%d M", distance_walked);
  //update_text(Text_holder_small->text_array[DISTANCE], distance_buffer);
}

static void update_time() {
  // Get a tm structure.
  time_t temp = time(NULL);
  struct tm * tick_time = localtime(&temp);

  // format the time from the tm structure and stick it into a buffer
  static char buffer[8];
  strftime(buffer, sizeof(buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

  // Change the text layer to reflect the current time.
  //update_text(Text_holder_large->text_array[0], buffer);
}

// Update the time and health text whenever the time changes (each minute)
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  set_health_info_text();
}

static void battery_state_handler(BatteryChargeState charge) {
  // Report the current charge percentage
  //APP_LOG(APP_LOG_LEVEL_INFO, "Battery charge is %d%%", (int)charge.charge_percent);

}

static void window_load(Window *window) {
  // window info. I'll use this for both the canvas and text.
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer); //This canvas will cover the whole window

  

  // Drawing layer init.
  canvas = layer_create(window_bounds);
  layer_set_update_proc(canvas, canvas_update_proc);
  layer_add_child(window_layer, canvas);

  // Setup time layer
  Text_holder_large = init_texts_struct(NUM_LARGE_TEXT, GColorBrightGreen, GColorClear, RESOURCE_ID_DS_DIGII_35, window_layer);
  add_text(Text_holder_large, GRect(60, 40, window_bounds.size.w, 50), "00:00", window_layer);

  // Text_holder_small = init_texts_struct(1, GColorBrightGreen, GColorClear, RESOURCE_ID_DS_DIGII_35, window_layer);
  // add_text(Text_holder_large, GRect(60, 40, window_bounds.size.w, 50), "00:00", window_layer);
  // add_text(Text_holder_small, GRect(50, 134, 45, 20), "0", window_layer);

  // digital_font_30 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DS_DIGII_35));
  // time_layer = text_layer_create(GRect(60, 40, window_bounds.size.w, 50));
  // text_layer_set_background_color(time_layer,GColorClear);
  // text_layer_set_text_color(time_layer, GColorBrightGreen);
  // text_layer_set_font(time_layer, digital_font_30);
  // text_layer_set_text_alignment(time_layer, GTextAlignmentLeft);
  // text_layer_set_text(time_layer, "00:00");
  // layer_add_child(window_layer, text_layer_get_layer(time_layer));

  // // steps layer
  // digital_font_15 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DS_DIGII_15));
  // steps_layer = text_layer_create(GRect(50, 112, 45, 20));
  // text_layer_set_background_color(steps_layer,GColorClear);
  // text_layer_set_text_color(steps_layer, GColorBrightGreen);
  // text_layer_set_font(steps_layer, digital_font_15);
  // text_layer_set_text_alignment(steps_layer, GTextAlignmentLeft);
  // text_layer_set_text(steps_layer, "0");
  // layer_add_child(window_layer, text_layer_get_layer(steps_layer));

  // // distance walked layer
  // distance_layer = text_layer_create(GRect(50, 134, 45, 20));
  // text_layer_set_background_color(distance_layer,GColorClear);
  // text_layer_set_text_color(distance_layer, GColorBrightGreen);
  // text_layer_set_font(distance_layer, digital_font_15);
  // text_layer_set_text_alignment(distance_layer, GTextAlignmentLeft);
  // text_layer_set_text(distance_layer, "0");
  // layer_add_child(window_layer, text_layer_get_layer(distance_layer));

  // //STEP
  // STEP_pic = gbitmap_create_with_resource(RESOURCE_ID_STEP);
  // STEP_layer = bitmap_layer_create(GRect(50,110,15,5));
  // bitmap_layer_set_compositing_mode(STEP_layer, GCompOpSet);
  // bitmap_layer_set_bitmap(STEP_layer, STEP_pic);
  // layer_add_child(window_layer, bitmap_layer_get_layer(STEP_layer));

  // //DISTANCE
  // DISTANCE_pic = gbitmap_create_with_resource(RESOURCE_ID_DISTANCE);
  // DISTANCE_layer = bitmap_layer_create(GRect(50,132,17,5));
  // bitmap_layer_set_compositing_mode(DISTANCE_layer, GCompOpSet);
  // bitmap_layer_set_bitmap(DISTANCE_layer, DISTANCE_pic);
  // layer_add_child(window_layer, bitmap_layer_get_layer(DISTANCE_layer));

  // //CALL
  // CALL_pic = gbitmap_create_with_resource(RESOURCE_ID_CALL);
  // CALL_layer = bitmap_layer_create(GRect(61,89,22,7));
  // bitmap_layer_set_compositing_mode(CALL_layer, GCompOpSet);
  // bitmap_layer_set_bitmap(CALL_layer, CALL_pic);
  // layer_add_child(window_layer, bitmap_layer_get_layer(CALL_layer));

  // // snake
  // snake_pic = gbitmap_create_with_resource(RESOURCE_ID_SNAKE_PROFILE);
  // snake_layer = bitmap_layer_create(GRect(10,106,32,48));
  // bitmap_layer_set_compositing_mode(snake_layer,GCompOpSet);
  // bitmap_layer_set_bitmap(snake_layer, snake_pic);
  // layer_add_child(window_layer, bitmap_layer_get_layer(snake_layer));

  // // mei ling
  // mei_ling_pic = gbitmap_create_with_resource(RESOURCE_ID_MEI_LING_PROFILE);
  // mei_ling_layer = bitmap_layer_create(GRect(101,106,32,48));
  // bitmap_layer_set_compositing_mode(mei_ling_layer,GCompOpSet);
  // bitmap_layer_set_bitmap(mei_ling_layer, mei_ling_pic);
  // layer_add_child(window_layer, bitmap_layer_get_layer(mei_ling_layer));

  Image_holder = init_images_struct(1);
  add_image(Image_holder, GRect(0, 0, 32, 48), RESOURCE_ID_SNAKE_PROFILE, window_layer);
  // test_image = init_image(GRect(0, 0, 32, 48), RESOURCE_ID_SNAKE_PROFILE, window_layer);
}

static void window_unload(Window *window) {
}

void main_window_create() {
    s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_set_background_color(s_window, GColorBlack);

  const bool animated = true;
  window_stack_push(s_window, animated);

  // Get the time every minute.
  // Can be placed before the window is created too. I think this first update_time() call could be placed in window_load() if you do.
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  update_time();

  /*
  if(!health_service_events_subscribe(health_handler, NULL)) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Health not available!");
  }
  else {
    set_health_info_text();
    APP_LOG(APP_LOG_LEVEL_INFO, "Health GET!");
  }
  */

  battery_state_service_subscribe(battery_state_handler);
  //BatteryChargeState state = battery_state_service_peek();
  //APP_LOG(APP_LOG_LEVEL_INFO, "Battery charge is %d%%", (int)state.charge_percent);

}

void main_window_destroy() {
  //destroy_image_struct(test_image);
  de_init_images_struct(Image_holder);
  destroy_texts_struct(Text_holder_large);
  // destroy_texts_struct(Text_holder_small);

  // gbitmap_destroy(snake_pic);
  // gbitmap_destroy(mei_ling_pic);
  // bitmap_layer_destroy(snake_layer);
  // bitmap_layer_destroy(mei_ling_layer);

  // gbitmap_destroy(STEP_pic);
  // gbitmap_destroy(DISTANCE_pic);
  // gbitmap_destroy(CALL_pic);
  // bitmap_layer_destroy(STEP_layer);
  // bitmap_layer_destroy(DISTANCE_layer);
  // bitmap_layer_destroy(CALL_layer);
  window_destroy(s_window);
}

/*Window * get_main_window() {
    return s_window;
}*/
