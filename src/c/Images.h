#pragma once
#include <pebble.h>
#include "Image.h"

struct Images
{
    uint32_t length;
    uint32_t top;
    struct Image **image_array; // array of pointers
};

struct Images * init_images_struct(uint32_t number_of_images);
void add_image(struct Images *image_list, GRect bounds, uint32_t resource_id, Layer *window_layer);
void push_image(struct Images *image_list, struct Image *input_image);
void de_init_images_struct(struct Images *image_list);