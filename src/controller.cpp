#include "controller.h"

IRSensor s;
Motor m;

/**
 * Controller Constructor
 * 
 * Function used to initialise the controller variables.
 */
Controller::Controller():
    sensor(s),
    motor(m),
    _currentState(ControlState::IDLE),
    _bulkLimit(0),
    irsensorEnabled(false),
    motorForwards(false),
    motorBackwards(false) {}

/**
 * Controller Begin Function
 * 
 * Function used to setup the associated sensors and motor.
 * 
 * @param sett Preferences Manager object
 */
void Controller::begin(Settings& sett) {
    sensor.begin();
    settings = sett;
    _holdPos = settings.getHoldPosition();
    _releasePos = settings.getReleasePosition();
    _holdTime = settings.getHoldTime();
    motor.begin(settings.getHallSensitivity());
    Serial.println("[Controller] Setup Complete");
}

/**
 * Controller Set Limit Function
 * 
 * Function for setting the bulk limit.
 * 
 * @param limit new bulk limit
 */
void Controller::setLimit(int limit) { _bulkLimit = limit; }

/**
 * Controller Get Current Count
 * 
 * Function for getting the current capsule count by the IR sensor.
 * 
 * @return current capsule count
 */
int Controller::getCurrentCount() { return sensor.getCount(); }

/**
 * Controller Motor Homed Function
 * 
 * Function that checks if the motor is homed.
 * 
 * @return true if motor is homed
 */
// bool Controller::motorHomed() { /* return motor.isHomed(); */ }
bool Controller::motorHomed() { return false; }

/**
 * Controller Enable IR Sensor Function
 * 
 * Function used to enable the IR Sensor to start counting capsules.
 */
void Controller::enableIRSensor() {
    if (!irsensorEnabled) {
        Serial.println("[Controller] Enabling IR Sensor");
        sensor.activate();
        irsensorEnabled = true;
    }
}

/**
 * Controller Disable IR Sensor Function
 * 
 * Function used to disable the IR sensor, reseting the count.
 */
void Controller::disableIRSensor() {
    if (irsensorEnabled) {
        Serial.println("[Controller] Disabling IR Sensor");
        sensor.deactivate();
        sensor.reset();
        irsensorEnabled = false;
    }
}

/**
 * Controller Move Forwards Function
 * 
 * Function for moving the motor forwards.
 */
void Controller::moveForwards() {
    if (!motorForwards) {
        motor.moveForwards(200);
        motorForwards = true;
        motorBackwards = false;
    }
}

/**
 * Controller Move Backwards Function
 * 
 * Function for moving the motor backwards.
 */
void Controller::moveBackwards() {
    if (!motorBackwards) {
        motor.moveBackwards(200);
        motorBackwards = true;
        motorForwards = false;
    }
}

/**
 * Controller Stop Function
 * 
 * Function for stopping the motor.
 */
void Controller::stop() {
    motor.stop();
    motorForwards = false;
    motorBackwards = false;
}

/**
 * Controller Home Function
 * 
 * Function for homing the motor.
 */
void Controller::home() { motor.home(); }

/**
 * Controller Update Settings Function
 * 
 * Function for updating the controller settings.
 */
void Controller::updateSettings() {/* TODO: implement */}

/**
 * Controller Update Function
 * 
 * Function used for updating the controller.
 * 
 * @return true if motor is being driven
 */
bool Controller::update() {
    // Execute Update Functions
    sensor.update();
    motor.update();
    // bool busy = motor.isBusy();
    // if (motorEnabled) motor.run();

    // return busy;
    return false;
}