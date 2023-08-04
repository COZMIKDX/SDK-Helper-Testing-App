#include <pebble.h>
#include "Texts.h"
#include "Text.h"


struct Texts * init_texts_struct(uint32_t number_of_texts, GColor text_color, GColor bg_color, uint32_t font_resource_id, Layer * window_layer) {
    struct Texts * texts_struct = (struct Texts *) malloc(sizeof(struct Texts));
    texts_struct->length = number_of_texts;
    texts_struct->top = 0;
    texts_struct->text_array = (struct Text **)malloc(sizeof(struct Text *));
    texts_struct->text_color = text_color;
    texts_struct->bg_color = bg_color;
    texts_struct->font = fonts_load_custom_font(resource_get_handle(font_resource_id));

    return texts_struct;
}

void add_text(struct Texts * text_list, GRect spatial_info, char * text, Layer * window_layer) {
    struct Text * new_text = init_text(spatial_info, text, text_list->text_color, text_list->bg_color, text_list->font, window_layer);
    push_text(text_list, new_text);
}

void push_text(struct Texts * text_list, struct Text * input_text) {
    if (text_list->top != (text_list->length))
    {
        text_list->text_array[text_list->top] = input_text;
        text_list->top = text_list->top + 1;
    }
    else
    {
        return; // Too many text pointers for the current array. I'll make an array allocate and copy function later.
        // Or an error return value.
    }
}

void destroy_texts_struct(struct Texts * texts_struct) {
    for (uint32_t i = 0; i < texts_struct->length; i++) {
        // Destroy the image, the image_struct, and then the pointer to the image struct.
        destroy_text_struct(texts_struct->text_array[i]);
        free((texts_struct->text_array)[i]);
    }
    free(texts_struct->text_array);
    free(texts_struct);
}