#include <pebble.h>
#include "Text.h"

// _set_text() docs says it uses a pointer to a string to draw text.
// So if the screen ever needs to redraw, I'll need the string data around somewhere. 
// I'll put it in the 
struct Text * init_text_struct(GRect spatial_info, char * text, GColor text_color, GColor bg_color, GFont font, Layer * window_layer) {
    struct Text * text_struct = (struct Text *) malloc(sizeof(struct Text));
    if (text_struct == NULL)
    {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to allocate Text struct");
        return NULL;
    }
    text_struct->text_layer = text_layer_create(spatial_info);
    if (text_struct->text_layer == NULL)
    {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to allocate text layer");
        return NULL;
    }
    text_struct->text = text;

    text_layer_set_background_color(text_struct->text_layer, bg_color);
    text_layer_set_text_color(text_struct->text_layer, text_color);
    text_layer_set_font(text_struct->text_layer, font);
    text_layer_set_text_alignment(text_struct->text_layer, GTextAlignmentLeft);
    text_layer_set_text(text_struct->text_layer, text_struct->text); 
    layer_add_child(window_layer, text_layer_get_layer(text_struct->text_layer));

    return text_struct;
    APP_LOG(APP_LOG_LEVEL_INFO, "Text struct successfully created!");
}

void update_text(struct Text * text_struct, char * text) {
    text_struct->text = text;
    text_layer_set_text(text_struct->text_layer, text_struct->text);
}

void destroy_text_struct(struct Text * text_struct) {
    text_layer_destroy(text_struct->text_layer);
    free(text_struct);
    APP_LOG(APP_LOG_LEVEL_INFO, "text struct destroyed!");
}