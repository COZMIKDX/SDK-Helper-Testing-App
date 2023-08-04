#include <pebble.h>
#include "mainWindow.h"


// checks if we can get step data from the start of the day to right now.
// If we can, get the number of steps and send them to a function in mainWindow.c that will write the steps to a text layer.
int get_steps() {
    HealthMetric steps = HealthMetricStepCount;

    time_t start = time_start_of_today();
    time_t end = time(NULL);

    // check if step data for the specified time range is available.
    HealthServiceAccessibilityMask mask = health_service_metric_accessible(steps, start, end);

    if (mask & HealthServiceAccessibilityMaskAvailable) {
        // Data is available.
        // Set the steps in the main menu.
        //set_steps((int)health_service_sum_today(steps));
        APP_LOG(APP_LOG_LEVEL_INFO, "Health data is available");
        return (int)health_service_sum_today(steps);
    }
    else 
    {
        APP_LOG(APP_LOG_LEVEL_INFO, "No data is available!");
    }
    return 0;
}

int get_distance_walked() {
    HealthMetric distance = HealthMetricWalkedDistanceMeters;

    time_t start = time_start_of_today();
    time_t end = time(NULL);

    // check if step data for the specified time range is available.
    HealthServiceAccessibilityMask mask = health_service_metric_accessible(distance, start, end);

    if (mask & HealthServiceAccessibilityMaskAvailable) {
        // Data is available.
        // Set the steps in the main menu.
        //set_steps((int)health_service_sum_today(steps));
        APP_LOG(APP_LOG_LEVEL_INFO, "Walked distance data is available");
        return (int)health_service_sum_today(distance);
    }
    else 
    {
        APP_LOG(APP_LOG_LEVEL_INFO, "No data is available!");
    }
    return 0;
}

void health_handler(HealthEventType event, void * context) {
    /*if (event == HealthEventMovementUpdate) {
        get_steps();
    }*/

    switch(event) {
    case HealthEventSignificantUpdate:
      APP_LOG(APP_LOG_LEVEL_INFO, 
              "New HealthService HealthEventSignificantUpdate event");
      break;
    case HealthEventMovementUpdate:
      APP_LOG(APP_LOG_LEVEL_INFO, 
              "New HealthService HealthEventMovementUpdate event");
      break;
    case HealthEventSleepUpdate:
      APP_LOG(APP_LOG_LEVEL_INFO, 
              "New HealthService HealthEventSleepUpdate event");
      break;
    case HealthEventHeartRateUpdate:
      APP_LOG(APP_LOG_LEVEL_INFO,
              "New HealthService HealthEventHeartRateUpdate event");
      break;
    default:
        APP_LOG(APP_LOG_LEVEL_INFO, "Default case.");
  }
}