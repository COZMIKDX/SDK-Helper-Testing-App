#pragma once
#include <pebble.h>

int get_steps();
int get_distance_walked();
void health_handler( HealthEventType event, void * context);