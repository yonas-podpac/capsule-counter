#include "controller.h"

IRSensor s;
Motor m;

/**
 * Controller Initialiser
 * 
 * Function used to initialise the controller variables.
 */
Controller::Controller():
    sensor(s),
    motor(m),
    _currentState(ControlState::IDLE) {}

/**
 * Controller Begin Function
 * 
 * Function used to setup the associated sensors and motor.
 */
void Controller::begin() {
    sensor.begin();
    motor.begin();
    Serial.println("[Controller] Setup Complete");
}

/**
 * Controller Update Function
 * 
 * Function used for updating the controller.
 * 
 * @return true if motor is being driven
 */
bool Controller::update() { return false; }