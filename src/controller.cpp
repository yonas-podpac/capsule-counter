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
    _bulkLimit(0),
    motorEnabled(false),
    irsensorEnabled(false) {}

/**
 * Controller Begin Function
 * 
 * Function used to setup the associated sensors and motor.
 * 
 * @param sett Preferences Manager object
 */
void Controller::begin(Settings& sett) {
    sensor.begin();
    motor.begin();
    settings = sett;
    _holdPos = settings.getHoldPosition();
    _releasePos = settings.getReleasePosition();
    _holdTime = settings.getHoldTime();
    motor.setPulseWidth(settings.getMotorPulseWidth());
    motor.setMaxSpeed(settings.getMotorMaxSpeed());
    motor.setAcceleration(settings.getMotorAcceleration());
    motor.setHallSensitivity(settings.getHallSensitivity());
    Serial.println("[Controller] Setup Complete");
}

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
 * Controller Enable Motor Function
 * 
 * Function for enabling the motor.
 */
void Controller::enableMotor() {
    if (!motorEnabled) {
        Serial.println("[Controller] Motor enabled");
        motor.enable();
        motorEnabled = true;
    }
}

/**
 * Controller Disable Motor Function
 * 
 * Function for disabling the motor.
 */
void Controller::disableMotor() {
    if (motorEnabled) {
        Serial.println("[Controller] Motor disabled");
        motor.disable();    // TODO: is this correct?
        motorEnabled = false;
    }
}

/**
 * Controller Move Forwards Function
 * 
 * Function for moving the motor forwards.
 */
void Controller::moveForwards() { motor.setSpeed(FORWARDS_SPEED); }

/**
 * Controller Move Backwards Function
 * 
 * Function for moving the motor backwards.
 */
void Controller::moveBackwards() { motor.setSpeed(BACKWARDS_SPEED); }

/**
 * Controller Stop Function
 * 
 * Function for stopping the motor.
 */
void Controller::stop() { motor.setSpeed(STOP_SPEED); }

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
    bool busy = motor.isBusy();
    if (motorEnabled) motor.run();

    return busy;
}