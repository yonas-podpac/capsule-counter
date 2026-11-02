#include "controller.h"
#include "menu.h"

// Define Class Objects
struct Menu2ControlCommand {
  bool enableIR;
  bool motorManual;
  int motorSpeed;
  bool home;
  bool settingsChange;
};
struct Control2MenuCommand {
  int currentCount;
  MotorState motorState;
};

// Initialise Objects
Controller controller;
Menu menu;
Settings settings;
TaskHandle_t controllerTask;
TaskHandle_t menuTask;
QueueHandle_t menu2ControlCommandQueue;
QueueHandle_t control2MenuCommandQueue;

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
  menu2ControlCommandQueue = xQueueCreate(10, sizeof(Menu2ControlCommand));
  control2MenuCommandQueue = xQueueCreate(10, sizeof(Control2MenuCommand));
  settings.begin();
  // controller.begin(settings.getHoldPosition(), settings.getReleasePosition(), settings.getHoldTime(), settings.getMotorPulseWidth(), settings.getMotorMaxSpeed(), settings.getMotorAcceleration(), settings.getHallSensitivity());
  controller.begin(settings);
  menu.begin(settings);
  Serial.println("[Main] Capsule Counter Setup Complete");
  xTaskCreatePinnedToCore(controllerHandlerTask, "Control Handling", 10000, NULL, 1, &controllerTask, 0);
  delay(500);
  xTaskCreatePinnedToCore(menuHandlerTask, "Menu Handling", 10000, NULL, 1, &menuTask, 1);
  delay(500);
}

void loop() {}