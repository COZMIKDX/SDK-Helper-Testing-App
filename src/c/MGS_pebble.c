#include <pebble.h>
#include "mainWindow.h"

void init(void) {
  main_window_create();
}

void deinit(void) {
  main_window_destroy();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
