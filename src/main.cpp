#include "menu.h"

TaskHandle_t sensorTask;
TaskHandle_t displayTask;
Display display;
Settings settings;
Joystick joystick;
Sensor sensor;
StepperMotor motor;
LogicHandler logicHandler(sensor, motor);
Menu menu(display, joystick, settings, logicHandler);

enum class MotorCommand {
  SET_IDLE,
  SET_HOMING,
  SET_COUNTING,
  SET_HOLDING,
  SET_RELEASING,
  SET_MANUAL,
  SET_ERROR
};

QueueHandle_t motorQueue;

void logicHandlingTask(void * pvParameters) {
  Serial.print("Logic Handling Task Running on Core ");
  Serial.println(xPortGetCoreID());
  MotorCommand rxCmd;

  for (;;) {
    if (xQueueReceive(motorQueue, &rxCmd, 0) == pdTRUE) {
      if (rxCmd == MotorCommand::SET_IDLE) logicHandler.setState(LogicState::IDLE);
      if (rxCmd == MotorCommand::SET_HOMING) logicHandler.setState(LogicState::HOMING);
      if (rxCmd == MotorCommand::SET_COUNTING) logicHandler.setState(LogicState::COUNTING);
      if (rxCmd == MotorCommand::SET_HOLDING) logicHandler.setState(LogicState::HOLDING);
      if (rxCmd == MotorCommand::SET_RELEASING) logicHandler.setState(LogicState::RELEASING);
      if (rxCmd == MotorCommand::SET_MANUAL) logicHandler.setState(LogicState::MANUAL);
      if (rxCmd == MotorCommand::SET_ERROR) logicHandler.setState(LogicState::ERROR);
    }
    if (logicHandler.update()) {
      vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelay(1);
  }
}

void displayUpdatingTask(void * pvParameters) {
  Serial.print("Display Task Running on Core ");
  Serial.println(xPortGetCoreID());
  MenuState mState;
  MotorCommand cmd;
  
  for (;;) {
    menu.update();
    mState = menu.getMenuState();
    if (mState == MenuState::IDLE) cmd = MotorCommand::SET_IDLE;
    if (mState == MenuState::HOMING) cmd = MotorCommand::SET_HOMING;
    if (mState == MenuState::COUNTING) cmd = MotorCommand::SET_COUNTING;
    if (mState == MenuState::HOLDING) cmd = MotorCommand::SET_HOLDING;
    if (mState == MenuState::RELEASING) cmd = MotorCommand::SET_RELEASING;
    if (mState == MenuState::MANUAL) cmd = MotorCommand::SET_MANUAL;
    if (mState == MenuState::ERROR) cmd = MotorCommand::SET_ERROR;
    xQueueSend(motorQueue, &cmd, portMAX_DELAY);
    delay(1);
  }
}

void setup() {
  Serial.begin(115200);
  motorQueue = xQueueCreate(10, sizeof(MotorCommand));
  display.begin();
  settings.begin();
  logicHandler.begin();
  menu.begin();
  xTaskCreatePinnedToCore(
    logicHandlingTask,
    "Logic Handling",
    10000,
    NULL,
    1,
    &sensorTask,
    0
  );
  delay(500);
  xTaskCreatePinnedToCore(
    displayUpdatingTask,
    "Display Checking",
    10000,
    NULL,
    1,
    &displayTask,
    1
  );
  delay(500);
}

void loop() {}