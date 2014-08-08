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

bool get_teen_status(){
	bool teen = 0;
	if(minute > 10 && minute < 20){
		teen = true;
	}
	return teen;
}

bool get_fresh_hour(){
	bool fresh = 0;
	if(minute == 0){
		fresh = 1;
	}
	return fresh;
}

bool get_hour_over_10(){
	bool isTrue = 0;
	if(hour > 10){
		isTrue = 1;
	}
	return isTrue;
}

void fix_fonts(){
	if(hour > 12){
		text_layer_set_font(word_1, gotham_bold_c1);
	}
	else{
		text_layer_set_font(word_1, gotham_bold_c);
	}
	
	if(minuteIsTeens){
		text_layer_set_font(word_2, gotham_bold_c1);
	}
	else{
		text_layer_set_font(word_2, gotham_bold_c);
	}
	
	if(isFreshHour){
		text_layer_set_font(word_3, gotham_bold_c1);
		layer_set_hidden(text_layer_get_layer(word_2), true);
	}
	else{
		text_layer_set_font(word_3, gotham_bold_c);
		layer_set_hidden(text_layer_get_layer(word_2), false);
	}
}

void try_override(){
	//I know I fucked up the buffers here but shut up and deal with it
	//Sorry
	if(isFreshHour){
		text_layer_set_text(word_1, "ZERO");
		text_layer_set_text(word_3, "HUNDRED");
		if(hourIsOver10){
			if(hour < 20){
				snprintf(word1_buffer, sizeof(word1_buffer), "%s", teens[hour-11]);
			}
			else{
				snprintf(word1_buffer, sizeof(word1_buffer), "%s", extra_hour_words[hour-21]);
			}
		}
		else{
			snprintf(word1_buffer, sizeof(word1_buffer), " ZERO %s", ones[hour]);
		}
		text_layer_set_text(word_1, word1_buffer);
	}
	
	if(minuteIsTeens){
		snprintf(word2_buffer, sizeof(word2_buffer), "%s", teens[minute-11]);
		text_layer_set_text(word_2, word2_buffer);
		text_layer_set_text(word_3, " ");
	}
}

void update_hour(){
	//APP_LOG(APP_LOG_LEVEL_INFO, "Hour");
	if(hour < 11){
		snprintf(word1_buffer, sizeof(word1_buffer), "%s", ones[hour]);
	}
	else if(hourIsOver10 && hour < 20){
		snprintf(word1_buffer, sizeof(word1_buffer), "%s", teens[hour-11]);
	}
	else if(hour >= 20){
		snprintf(word1_buffer, sizeof(word1_buffer), "%s", extra_hour_words[hour-20]);
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
	isFreshHour = get_fresh_hour();
	hourIsOver10 = get_hour_over_10();
	minuteIsTeens = get_teen_status();
	update_hour();
	update_minute_1();
	update_minute_2();
	try_override();
	fix_fonts();
}

void window_load(Window *window){
	Layer *window_layer = window_get_root_layer(window);
	gotham_bold_c = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GOTHAM_BOLD_31));
	gotham_bold_c1 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GOTHAM_BOLD_25));
	word_1 = text_layer_init(GRect(0, 0, 144, 168), GColorClear, GTextAlignmentLeft, 1);
	word_2 = text_layer_init(GRect(0, 60, 144, 168), GColorClear, GTextAlignmentLeft, 1);
	word_3 = text_layer_init(GRect(0, 90, 144, 168), GColorClear, GTextAlignmentLeft, 1);
	word_4 = text_layer_init(GRect(0, 130, 144, 168), GColorClear, GTextAlignmentLeft, 2);
	theme = inverter_layer_create(GRect(0, 0, 144, 168));
	
	text_layer_set_text(word_4, "JULIET");
	
	layer_add_child(window_layer, text_layer_get_layer(word_1));
	layer_add_child(window_layer, text_layer_get_layer(word_2));
	layer_add_child(window_layer, text_layer_get_layer(word_3));
	layer_add_child(window_layer, text_layer_get_layer(word_4));
	layer_add_child(window_layer, inverter_layer_get_layer(theme));
	
	//Load time instantly
	struct tm *t;
  	time_t temp;        
  	temp = time(NULL);        
  	t = localtime(&temp);
	
	tick_handler(t, SECOND_UNIT);
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
