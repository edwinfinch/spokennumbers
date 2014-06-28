/*
Spoken Numbers
Written by Edwin Finch
Idea suggested by /u/SlothLyfeYo
*/

#include <pebble.h>
#include "elements.h"

static TextLayer* text_layer_init(GRect location, GColor background, GTextAlignment alignment, int font)
{
	TextLayer *layer = text_layer_create(location);
	text_layer_set_text_color(layer, GColorBlack);
	text_layer_set_background_color(layer, background);
	text_layer_set_text_alignment(layer, alignment);
	if(font == 1){
		text_layer_set_font(layer, gotham_bold_c);
	}
	else if(font == 2){
		text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	}
	return layer;
}

void update_hour(){
	//APP_LOG(APP_LOG_LEVEL_INFO, "Hour");
	if(hour < 11){
		snprintf(word1_buffer, sizeof(word1_buffer), "%s", ones[hour]);
	}
	else if(hour > 10 && hour < 20){
		snprintf(word1_buffer, sizeof(word1_buffer), "%s", teens[hour-11]);
	}
	else{
		snprintf(word1_buffer, sizeof(word1_buffer), "%s", extra_hour_words[hour-21]);
	}
	text_layer_set_text(word_1, word1_buffer);
	//APP_LOG(APP_LOG_LEVEL_INFO, "Wrote hour");
}

void update_minute_1(){
	//APP_LOG(APP_LOG_LEVEL_INFO, "Minute");
	snprintf(word2_buffer, sizeof(word2_buffer), "%s", tens[(minute/10)]);
	text_layer_set_text(word_2, word2_buffer);
	//APP_LOG(APP_LOG_LEVEL_INFO, "Wrote minute");
}

void update_minute_2(){
	//APP_LOG(APP_LOG_LEVEL_INFO, "Minute 2");
	snprintf(word3_buffer, sizeof(word3_buffer), "%s", ones[(minute%10)]);
	text_layer_set_text(word_3, word3_buffer);
	//APP_LOG(APP_LOG_LEVEL_INFO, "Wrote minute 2");
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed){
	minute = tick_time->tm_min;
	hour = tick_time->tm_hour;
	update_hour();
	update_minute_1();
	update_minute_2();
}

void window_load(Window *window){
	Layer *window_layer = window_get_root_layer(window);
	gotham_bold_c = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GOTHAM_BOLD_31));
	word_1 = text_layer_init(GRect(0, 0, 144, 168), GColorClear, GTextAlignmentLeft, 1);
	word_2 = text_layer_init(GRect(0, 60, 144, 168), GColorClear, GTextAlignmentLeft, 1);
	word_3 = text_layer_init(GRect(0, 90, 144, 168), GColorClear, GTextAlignmentLeft, 1);
	word_4 = text_layer_init(GRect(0, 130, 144, 168), GColorClear, GTextAlignmentLeft, 2);
	theme = inverter_layer_create(GRect(0, 0, 144, 168));
	
	text_layer_set_text(word_4, "HOURS");
	
	layer_add_child(window_layer, text_layer_get_layer(word_1));
	layer_add_child(window_layer, text_layer_get_layer(word_2));
	layer_add_child(window_layer, text_layer_get_layer(word_3));
	layer_add_child(window_layer, text_layer_get_layer(word_4));
	layer_add_child(window_layer, inverter_layer_get_layer(theme));
}

void window_unload(Window *window){
	text_layer_destroy(word_1);
	text_layer_destroy(word_2);
	text_layer_destroy(word_3);
	text_layer_destroy(word_4);
}

void init(void) {
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers){
		.load = window_load,
		.unload = window_unload,
	});
	tick_timer_service_subscribe(MINUTE_UNIT, &tick_handler);
	window_stack_push(window, true);
}

void deinit(void) {
	window_destroy(window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
