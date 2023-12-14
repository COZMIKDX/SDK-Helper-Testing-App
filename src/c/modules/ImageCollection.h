#pragma once
#include <pebble.h>
#include "Image.h"

struct ImageCollection
{
    uint32_t length;
    uint32_t top;
    struct Image **image_array; // array of pointers
};

struct ImageCollection * init_image_collection(uint32_t number_of_images);
void add_image(struct ImageCollection *image_list, GRect bounds, uint32_t resource_id, Layer *window_layer);
void push_image(struct ImageCollection *image_list, struct Image *input_image);
void destroy_image_collection(struct ImageCollection *image_list);