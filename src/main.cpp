#include "controller.h"
#include "menu.h"

// Define Class Objects
struct Menu2ControlCommand {
  int limit;
  bool enableIR;
  bool enableMotor;
  bool homeMotor;
  MotorState motorState;
};
struct Control2MenuCommand {
  int currentCount;
  bool motorHomed;
};

// Initialise Objects
Controller* controller = nullptr;
Menu menu;
Settings settings;
TaskHandle_t controllerTask = NULL;
TaskHandle_t menuTask = NULL;
QueueHandle_t menu2ControlCommandQueue;
QueueHandle_t control2MenuCommandQueue;

/**
 * Controller Task Function
 * 
 * Function used to build the task for the controller.
 */
void controllerHandlerTask(void * pvParameters) {
  Controller* ctrl = (Controller*)pvParameters;

  if (pvParameters == NULL) {
    Serial.println("[Main] CRITICAL ERROR: Controller pointer is null");
    vTaskDelete(NULL);
  }
  // Print Core task is running on
  Serial.print("[Main] Controller Task running on core ");
  Serial.println(xPortGetCoreID());
  Control2MenuCommand cmd;
  Menu2ControlCommand rxCmd;

  // Build Task Update Loop
  for(;;) {
    // Check Menu to Control Command Queue
    // if (xQueueReceive(menu2ControlCommandQueue, &rxCmd, 0) == pdTRUE) {
    //   // Handle Limit Setting
    //   if (rxCmd.limit > 0) ctrl->setLimit(rxCmd.limit);

    //   // Handle Enabling IR Sensor
    //   if (rxCmd.enableIR) ctrl->enableIRSensor();
    //   else ctrl->disableIRSensor();

    //   // Handle Enabling Motor
    //   if (rxCmd.enableMotor) ctrl->enableMotor();
    //   else ctrl->disableMotor();

    //   // Handle Homing Motor
    //   if (rxCmd.homeMotor) ctrl->home();

    //   // Handle Motor Direction
    //   if (rxCmd.motorState == MotorState::FORWARDS) ctrl->moveForwards();
    //   if (rxCmd.motorState == MotorState::BACKWARDS) ctrl->moveBackwards();
    //   if (rxCmd.motorState == MotorState::STOPPED) ctrl->stop();
    // }

    // Update Controller
    ctrl->update();
    taskYIELD();

    // // Check if Motor is Homed
    // if (ctrl->motorHomed()) cmd.motorHomed = true;
    // else cmd.motorHomed = false;

    // // Set Current Capsule Count
    // cmd.currentCount = ctrl->getCurrentCount();

    // // Send Command from Controller to Menu
    // xQueueSend(control2MenuCommandQueue, &cmd, portMAX_DELAY);
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
  Menu2ControlCommand cmd;
  Control2MenuCommand rxCmd;

  // Build Task Update Loop
  for (;;) {
    // Check Control to Menu Command Queue
    if (xQueueReceive(control2MenuCommandQueue, &rxCmd, 0) == pdTRUE) {
      // Set the Current Capsuel Count
      menu.setCurrentCount(rxCmd.currentCount);
      
      // Set the Motor Homed Status
      menu.setMotorHomed(rxCmd.motorHomed);
    }

    // Update Menu
    menu.update();
    Screen curr = menu.getCurrentScreen();

    // Only update if available
    if (menu.updateAvailable()) {
      // Handle Count Screen
      if (curr == Screen::COUNT) {
        cmd.limit = menu.getBulkLimit();
        cmd.enableIR = true;
        cmd.enableMotor = true;
      }

      // Handle Manual Screen
      else if (curr == Screen::MANUAL) {
        cmd.enableMotor = true;
        cmd.motorState = menu.getMotorState();
      }

      // Handle Home Screen
      else if (curr == Screen::HOME) {
        if (menu.getMotorState() == MotorState::HOMING) {
          cmd.enableMotor = true;
          cmd.homeMotor = true;
        }
      }

      // Handle Other Screens
      else {
        cmd.enableIR = false;
        cmd.enableMotor = false;
      }
      
      // Reset update available
      menu.resetUpdateAvailable();
    }

    // Send Command from Menu to Controller
    xQueueSend(menu2ControlCommandQueue, &cmd, portMAX_DELAY);
    // delay(1);
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("[Main] Capsule Counter Setup Beginning");
  menu2ControlCommandQueue = xQueueCreate(10, sizeof(Menu2ControlCommand));
  control2MenuCommandQueue = xQueueCreate(10, sizeof(Control2MenuCommand));
  settings.begin();
  controller = new Controller();
  controller->begin(settings);
  menu.begin(settings);
  Serial.println("[Main] Capsule Counter Setup Complete");
  // xTaskCreatePinnedToCore(menuHandlerTask, "Menu Handling", 10000, NULL, 1, &menuTask, 0);
  // delay(500);
  xTaskCreatePinnedToCore(controllerHandlerTask, "Control Handling", 16384, controller, 2, &controllerTask, 1);
  delay(500);
}

void loop() {}