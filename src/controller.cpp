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
    _currentState(ControlState::IDLE),
    _bulkLimit(0) {}

/**
 * Controller Begin Function
 * 
 * Function used to setup the associated sensors and motor.
 * 
 * @param hPos hold position
 * @param rPos release position
 * @param hTime hold time
 * @param pw motor pulse width
 * @param ms motor maximum speed
 * @param acc motor acceleartion
 * @param hs hall sensitivity
 */
void Controller::begin(long hPos, long rPos, unsigned long hTime, int pw, float ms, float acc, int hs) {
    sensor.begin();
    motor.begin();
    _holdPos = hPos;
    _releasePos = rPos;
    _holdTime = hTime;
    motor.setPulseWidth(pw);
    motor.setMaxSpeed(ms);
    motor.setAcceleration(acc);
    motor.setHallSensitivity(hs);
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