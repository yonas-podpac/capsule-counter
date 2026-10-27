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
      if (rxCmd == MotorCommand::SET_IDLE) logicHandler.setState(State::IDLE);
      if (rxCmd == MotorCommand::SET_HOMING) logicHandler.setState(State::HOMING);
      if (rxCmd == MotorCommand::SET_COUNTING) logicHandler.setState(State::COUNTING);
      if (rxCmd == MotorCommand::SET_HOLDING) logicHandler.setState(State::HOLDING);
      if (rxCmd == MotorCommand::SET_RELEASING) logicHandler.setState(State::RELEASING);
      if (rxCmd == MotorCommand::SET_MANUAL) logicHandler.setState(State::MANUAL);
      if (rxCmd == MotorCommand::SET_ERROR) logicHandler.setState(State::ERROR);
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
  LogicState lState;
  MotorCommand cmd;
  
  for (;;) {
    menu.update();
    lState = menu.getLogicState();
    if (lState == LogicState::IDLE) cmd = MotorCommand::SET_IDLE;
    if (lState == LogicState::HOMING) cmd = MotorCommand::SET_HOMING;
    if (lState == LogicState::COUNTING) cmd = MotorCommand::SET_COUNTING;
    if (lState == LogicState::HOLDING) cmd = MotorCommand::SET_HOLDING;
    if (lState == LogicState::RELEASING) cmd = MotorCommand::SET_RELEASING;
    if (lState == LogicState::MANUAL) cmd = MotorCommand::SET_MANUAL;
    if (lState == LogicState::ERROR) cmd = MotorCommand::SET_ERROR;
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