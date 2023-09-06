#include <pebble.h>
#include "Images.h"
#include "Image.h"

struct Images * init_images_struct(uint32_t number_of_images) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Allocating images struct");
    APP_LOG(APP_LOG_LEVEL_INFO, "heap memory %lu", heap_bytes_free());
    APP_LOG(APP_LOG_LEVEL_INFO, "size of Images struct %lu", sizeof(struct Images));
    struct Images *image_list = (struct Images *)malloc(sizeof(struct Images)); // allocate images struct
    APP_LOG(APP_LOG_LEVEL_INFO, "ree malloc");
    if (image_list == NULL){
        APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to allocate images_struct");
        return NULL;
    }
    APP_LOG(APP_LOG_LEVEL_INFO, "images struct allocated");
    APP_LOG(APP_LOG_LEVEL_INFO, "allocating image array");
    image_list->image_array = (struct Image **)malloc(sizeof(struct Image *) * number_of_images);  // allocate images array
    if (image_list->image_array == NULL) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to allocate image array for images_struct");
        return NULL;
    }
    APP_LOG(APP_LOG_LEVEL_INFO, "image array allocated");
    image_list->length = number_of_images;
    image_list->top = 0;
    APP_LOG(APP_LOG_LEVEL_INFO, "images struct created!");
    return image_list;
}

void add_image(struct Images *image_list, GRect bounds, uint32_t resource_id, Layer *window_layer)
{
    struct Image *image_struct = init_image_struct(bounds, resource_id, window_layer);
    if (image_struct == NULL){
        APP_LOG(APP_LOG_LEVEL_ERROR, "init_image_struct returned NULL. Cannot push to array.");
        return;
    }
    push_image(image_list, image_struct);
    APP_LOG(APP_LOG_LEVEL_INFO, "new image pushed!");
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
void destroy_images_struct(struct Images * image_list) {
    for (uint32_t i = 0; i < image_list->length; i++) {
        // Destroy the image, the image_struct, and then the pointer to the image struct.
        destroy_image_struct(image_list->image_array[i]);
    }
    free(image_list->image_array);
    free(image_list);
}