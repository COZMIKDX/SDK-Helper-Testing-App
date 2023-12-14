#include <pebble.h>
#include "TextCollection.h"
#include "Text.h"


struct TextCollection * init_text_collection(uint32_t number_of_texts, GColor text_color, GColor bg_color, uint32_t font_resource_id, Layer * window_layer) {
    struct TextCollection * collection = (struct TextCollection *) malloc(sizeof(struct TextCollection));
    if (collection == NULL)
    {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to allocate collection");
        return NULL;
    }
    collection->length = number_of_texts;
    collection->top = 0;
    collection->text_array = (struct Text **)malloc(sizeof(struct Text *));
    if (collection->text_array == NULL)
    {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to allocate text array for collection");
        return NULL;
    }
    collection->text_color = text_color;
    collection->bg_color = bg_color;
    collection->font = fonts_load_custom_font(resource_get_handle(font_resource_id));

    APP_LOG(APP_LOG_LEVEL_INFO, "text colletion created!");
    return collection;
}

struct Text * add_text(struct TextCollection * collection, GRect spatial_info, char * text, Layer * window_layer) {
    struct Text * new_text = init_text_struct(spatial_info, text, collection->text_color, collection->bg_color, collection->font, window_layer);
    if (new_text == NULL)
    {
        APP_LOG(APP_LOG_LEVEL_ERROR, "init_text_struct returned NULL. Cannot push to array.");
        return NULL;
    }
    push_text(collection, new_text);
    APP_LOG(APP_LOG_LEVEL_INFO, "new text pushed!");
    return new_text;
}

void push_text(struct TextCollection * collection, struct Text * input_text) {
    if (collection->top != (collection->length))
    {
        collection->text_array[collection->top] = input_text;
        collection->top = collection->top + 1;
    }
    else
    {
        return; // Too many text pointers for the current array. I'll make an array allocate and copy function later.
        // Or an error return value.
    }
}

void destroy_text_collection(struct TextCollection * collection) {
    for (uint32_t i = 0; i < collection->length; i++) {
        // Destroy the text layer and the text_struct
        destroy_text_struct(collection->text_array[i]);
    }
    free(collection->text_array);
    free(collection);
    APP_LOG(APP_LOG_LEVEL_INFO, "TextCollection struct destroyed!");
}