#include <pebble.h>
#include "Images.h"
#include "Image.h"

struct Images * init_images_struct(uint32_t number_of_images) {
    struct Images *image_list = (struct Images *)malloc(sizeof(struct Images)); // allocate images struct
    image_list->image_array = (struct Image **)malloc(sizeof(struct Image *));  // allocate images array
    image_list->length = number_of_images;
    image_list->top = 0;
    return image_list;
}

void add_image(struct Images *image_list, GRect bounds, uint32_t resource_id, Layer *window_layer)
{
    struct Image *image_struct = init_image(bounds, resource_id, window_layer);
    push_image(image_list, image_struct);
}

void push_image(struct Images * image_list, struct Image * input_image) {
    // Access the pointer for the array on pointers and then write the new image pointer value to the next unused slot.
    if (image_list->top != (image_list->length))
    {
        image_list->image_array[image_list->top] = input_image;
        image_list->top = image_list->top + 1;
    }
    else
    {
        return; // Too many images for the current array. I'll make an array allocate and copy function later.
    }
}

// Free what the image struct pointers are pointing at and then free the array holding the pointers.
void de_init_images_struct(struct Images * image_list) {
    for (uint32_t i = 0; i < image_list->length; i++) {
        // Destroy the image, the image_struct, and then the pointer to the image struct.
        destroy_image_struct(image_list->image_array[i]);
        free((image_list->image_array)[i]);
    }
    free(image_list->image_array);
    free(image_list);
}