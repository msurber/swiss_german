#include <pebble.h>

static Window *window;
static TextLayer *minuteLayer; // The Minutes
static TextLayer *hourLayer; // The hours

static void init_text_layers(int linecount) {
  (void)linecount;
  int skippLines = 3 - linecount;

  minuteLayer = text_layer_create((GRect) { .origin = {10, 10 - (skippLines * 21)}, .size = {144-10 /* width */, 168-14 /* height */}});
  text_layer_set_text_color(minuteLayer, GColorWhite);
  text_layer_set_background_color(minuteLayer, GColorClear);
  GFont bitham = fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT);
  text_layer_set_font(minuteLayer, bitham);

  hourLayer = text_layer_create((GRect) { .origin = {10, 94 - (skippLines * 21)}, .size = {144-10 /* width */, 168-14 /* height */}});
  text_layer_set_text_color(hourLayer, GColorWhite);
  text_layer_set_background_color(hourLayer, GColorClear);
  GFont bithamBold = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
  text_layer_set_font(hourLayer, bithamBold);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  init_text_layers(3);
  layer_add_child(window_layer, text_layer_get_layer(minuteLayer));
  layer_add_child(window_layer, text_layer_get_layer(hourLayer));
}

static void window_unload(Window *window) {
  text_layer_destroy(minuteLayer);
  text_layer_destroy(hourLayer);
}

static void display_time(struct tm *time) {

const char *hour_string[25] = { "zwölfi", "eis","zwei", "drü", "viäri", "föifi", "sächsi",
		 "sibni", "achti", "nüni", "zäni", "elfi", "zwölfi", "eis", "zwei", "drü", "viäri",
		 "föifi", "sächsi", "sibni", "achti", "nüni", "zäni", "elfi" , "zwölfi"};

  int hour = time->tm_hour;
  int min = time->tm_min;
  char minute_text[50];
  char hour_text[50];
  if (min < 25) {
    strcpy(hour_text , hour_string[hour]);
  } else {
  	strcpy(hour_text , hour_string[hour + 1]);
  }
  if (-1 < min && min < 5) {
  	 strcpy(minute_text , "");
  }
  if (4 < min && min < 10) {
  	 strcpy(minute_text , "\nföif ab");
  }
  if (9 < min && min < 15) {
  	 strcpy(minute_text , "\nzäh ab");
  }
  if (14 < min && min < 20) {
  	 strcpy(minute_text , "viertel ab");
  }
  if (19 < min && min < 25) {
  	 strcpy(minute_text , "zwänzg ab");
  }
  if (24 < min && min < 30) {
  	 strcpy(minute_text , "föif vor halbi");
  }
  if (29 < min && min < 35) {
  	 strcpy(minute_text , "\nhalbi");
  }
  if (34 < min && min < 40) {
  	 strcpy(minute_text , "föif ab halbi");
  }
  if (39 < min && min < 45) {
  	 strcpy(minute_text , "zwänzg vor");
  }
  if (44 < min && min < 50) {
  	 strcpy(minute_text , "viertel vor");
  }
  if (49 < min && min < 55) {
  	 strcpy(minute_text , "\nzäh vor");
  }
  if (54 < min && min < 60) {
  	 strcpy(minute_text , "\nföif vor");
  }
/*
  if (linecount > 4) {
	  init_text_layers(linecount);
  }
*/
  
  static char staticTimeText[50] = ""; // Needs to be static because it's used by the system later.
  strcpy(staticTimeText , "");
  strcat(staticTimeText , minute_text);
  text_layer_set_text(minuteLayer, staticTimeText);

  static char staticHourText[50] = ""; // Needs to be static because it's used by the system later.
  strcpy(staticHourText , "");
  strcat(staticHourText , hour_text);
  text_layer_set_text(hourLayer, staticHourText);
}


static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  display_time(tick_time);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
  window_set_background_color(window, GColorBlack);
  
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  display_time(tick_time);
  
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}


static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
