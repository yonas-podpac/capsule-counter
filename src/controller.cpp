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
    holding(false),
    releasing(false),
    irsensorEnabled(false),
    _lastHoldMs(0),
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
    Serial.printf("[Controller] Address at Begin: %p\n", this);
}

/**
 * Controller Set State Function
 * 
 * Function used to set the controller state.
 * 
 * @param state new controller state
 */
void Controller::setState(ControlState state) { _currentState = state; }

/**
 * Controller  In Count State Function
 * 
 * Function used to check if controller is in one of the counting
 * states, i.e. COUNT, HOLD, RELEASE
 * 
 * @return true if controller is in one of the three counting states
 */
bool Controller::inCountState() { return _currentState == ControlState::COUNT || _currentState == ControlState::HOLD || _currentState == ControlState::RELEASE; }

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
        Serial.printf("[Controller] Address at Forwards: %p\n", this);
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
        Serial.printf("[Controller] Address at Backwards: %p\n", this);
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
void Controller::home() { 
    Serial.printf("[Controller] Address at Homing: %p\n", this);
    _currentState = ControlState::HOME;
    motor.home();
}

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
    bool running = motor.update();
    int count = sensor.getCount();
    unsigned long now;
    
    // Handle Control State Change
    switch(_currentState) {
        case ControlState::HOME:
            // Check if Stepper Motor has been Homed
            if (motor.getState() == HomingState::COMPLETE) {
                Serial.println("[Controller] Stepper motor homed");
                _currentState = ControlState::IDLE;
            }
            break;
        case ControlState::COUNT:
            // Check if Sensor Count has reached Limit
            if (count == _bulkLimit) {
                Serial.printf("[Controller] Sensor has counted %d capsules. Initiating holding sequence\n", count);
                _currentState = ControlState::HOLD;
            }
            break;
        case ControlState::HOLD:
            // Move to Hold Position
            if (!holding) {
                Serial.printf("[Controller] Holding Capsules; Moving motor to %ld\n", _holdPos);
                motor.moveTo(_holdPos);
                holding = true;
                // TODO: check if motor has reached position
            }
            if (!running) {
                Serial.printf("[Controller] Motor has reached hold position %ld\n", _holdPos);
                holding = false;
                _lastHoldMs = 0;
                _currentState = ControlState::HOLDING;
            }
            break;
        case ControlState::HOLDING:
            // Check if Motor has held capsules for required time
            now = millis();
            if (now - _lastHoldMs >= _holdTime) {
                Serial.printf("[Controller] Motor has held capsules for %lums; Releasing Capsules\n", _holdTime);
                sensor.reset();
                _currentState = ControlState::RELEASE;
            }
            break;
        case ControlState::RELEASE:
            // Move to Release Position
            if (!releasing) {
                Serial.printf("[Controller] Releasing Capsules; Moving motor to %ld\n", _releasePos);
                motor.moveTo(_releasePos);
                releasing = true;
            }
            if (!running) {
                Serial.printf("[Controller] Motor has reached release position %ld\n", _releasePos);
                releasing = false;
                _currentState = ControlState::COUNT;
            }
            break;
    };

    return false;
}