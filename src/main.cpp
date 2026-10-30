#include "controller.h"
#include "menu.h"
#include "settings.h"

// Define Class Objects
struct Command {};

// Initialise Objects
Controller controller;
Menu menu;
Settings settings;
TaskHandle_t controllerTask;
TaskHandle_t menuTask;
QueueHandle_t commandQueue;

/**
 * Controller Task Function
 * 
 * Function used to build the task for the controller.
 */
void controllerHandlerTask(void * pvParameters) {
  // Print Core task is running on
  Serial.print("[Main] Controller Task running on core ");
  Serial.println(xPortGetCoreID());

  // Build Task Update Loop
  for(;;) {
    if (controller.update()) {
      vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelay(1);
  }
}

/**
 * Menu Task Function
 * 
 * Function used to build the task for the menu.
 */
void menuHandlerTask(void * pvParameters) {
  // Print Core task is running on
  Serial.print("[Main] Menu Task running on core ");
  Serial.println(xPortGetCoreID());

  // Build Task Update Loop
  for (;;) {
    menu.update();
    delay(1);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("[Main] Capsule Counter Setup Beginning");
  controller.begin();
  menu.begin();
  settings.begin();
  Serial.println("[Main] Capsule Counter Setup Complete");
  xTaskCreatePinnedToCore(controllerHandlerTask, "Control Handling", 10000, NULL, 1, &controllerTask, 0);
  delay(500);
  xTaskCreatePinnedToCore(menuHandlerTask, "Menu Handling", 10000, NULL, 1, &menuTask, 1);
  delay(500);
}

void loop() {}