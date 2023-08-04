#pragma once
#include <pebble.h>
#include "Text.h"

struct Texts {
    uint32_t length;
    uint32_t top;
    struct Text ** text_array;
    GColor text_color;
    GColor bg_color;
    GFont font;
};

struct Texts * init_texts_struct(uint32_t number_of_texts, GColor text_color, GColor bg_color, uint32_t font_resource_id, Layer * window_layer);
void add_text(struct Texts * text_list, GRect spatial_info, char * text, Layer * window_layer);
void push_text(struct Texts * text_list, struct Text * input_text);
void destroy_texts_struct(struct Texts * texts_struct);