#pragma once
#include <pebble.h>
#include "Text.h"

struct TextCollection {
    uint32_t length;
    uint32_t top;
    struct Text ** text_array;
    GColor text_color;
    GColor bg_color;
    GFont font;
};

struct TextCollection * init_text_collection(uint32_t number_of_texts, GColor text_color, GColor bg_color, uint32_t font_resource_id, Layer * window_layer);
struct Text * add_text(struct TextCollection * collection, GRect spatial_info, char * text, Layer * window_layer);
void push_text(struct TextCollection * collection, struct Text * input_text);
void destroy_text_collection(struct TextCollection * collection);