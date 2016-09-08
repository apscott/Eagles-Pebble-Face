#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;

// Layer/bmp for the stripe behind the time
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

// Layer/bmp for the Eagles helm
static BitmapLayer *s_helm_layer;
static GBitmap *s_helm_bitmap;

// Layer/bmp for the opponent team in gameday view
static BitmapLayer *s_helm_opp_layer;
static GBitmap *s_helm_opp_bitmap;

// Are the Eagles playing right now?
static bool is_gameday = false;

static void gameday_window_load(Window *window);
static void gameday_window_unload(Window *window);
static void main_window_load(Window *window);
static void main_window_unload(Window *window);

// Are the Eagles playing right now?
static bool check_gameday() {
  // Just stubbed out right now. This should either be:
  // A) A hard coded scheule (the lazy way) or
  // B) periodically check the web to update the schedule because flex games exist (the proper way)
  // Need to check the game schedule against the current time/date. Return true/false as appropriate.
  return !is_gameday; // Watch is switch every minute.
}

// Callback for the time event (every minute)
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

  // Is has gameday status changed? If so, change the view
  if(is_gameday != check_gameday()) {
    is_gameday = !is_gameday;
    // Is loading/unloading the main window the pebble proper way to do this?
    // Don't know...let's find out!
    if(is_gameday) {
      main_window_unload(s_main_window);
      gameday_window_load(s_main_window);
    } else {
      gameday_window_unload(s_main_window);
      main_window_load(s_main_window);
    }
  }
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}

static void gameday_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create GBitmap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_helm_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HELM_PHI_GAMEDAY);
  s_helm_opp_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HELM_SHT); //TODO this should be dynamic based on who they're playing
  
  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(GRect(0,PBL_IF_ROUND_ELSE(75, 80),bounds.size.w,80));
  s_helm_layer = bitmap_layer_create(GRect(5,2,57,60));
  s_helm_opp_layer = bitmap_layer_create(GRect((bounds.size.w-62),2,57,60));
  
  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  
  bitmap_layer_set_bitmap(s_helm_layer, s_helm_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_helm_layer));
  
  bitmap_layer_set_bitmap(s_helm_opp_layer, s_helm_opp_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_helm_opp_layer));
  
  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(95, 95), bounds.size.w, 50));

  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LECO_40));
  
  // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void gameday_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  
  // Unload GFont
  fonts_unload_custom_font(s_time_font);
  
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);
  gbitmap_destroy(s_helm_bitmap);
  gbitmap_destroy(s_helm_opp_bitmap);
  
  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  bitmap_layer_destroy(s_helm_layer);
  bitmap_layer_destroy(s_helm_opp_layer);
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create GBitmap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_helm_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HELM_PHI);
  
  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(GRect(0,PBL_IF_ROUND_ELSE(75, 80),bounds.size.w,80));
  s_helm_layer = bitmap_layer_create(GRect(8,8,72,75));
  
  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  
  bitmap_layer_set_bitmap(s_helm_layer, s_helm_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_helm_layer));
  
  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(95, 95), bounds.size.w, 50));

  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LECO_40));
  
  // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  
  // Unload GFont
  fonts_unload_custom_font(s_time_font);
  
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);
  gbitmap_destroy(s_helm_bitmap);
  
  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  bitmap_layer_destroy(s_helm_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Black background, yo
  window_set_background_color(s_main_window, GColorBlack);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Make sure the time is displayed from the start
  update_time();
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}