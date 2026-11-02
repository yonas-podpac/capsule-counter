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
    Serial.println("[Controller] Enabling IR Sensor");
    sensor.activate();
}

/**
 * Controller Disable IR Sensor Function
 * 
 * Function used to disable the IR sensor, reseting the count.
 */
void Controller::disableIRSensor() {
    Serial.println("[Controller] Disabling IR Sensor");
    sensor.deactivate();
    sensor.reset();
}

/**
 * Controller Set Motor Manual Function
 * 
 * Function for setting the motor to manual mode.
 */
void Controller::setMotorManual() {
    Serial.println("[Controller] Motor set to manual drive");
    motor.enable();
}

/**
 * Controller Set Motor Auto Function
 * 
 * Function for setting the motor to auto mode.
 */
void Controller::setMotorAuto() {
    Serial.println("[Controller] Motor set to automatic drive");
    motor.disable();    // TODO: is this correct?
}

/**
 * Controller Set Motor Speed Function
 * 
 * Function for setting the motor speed.
 * 
 * @param speed motor speed in steps/second
 */
void Controller::setMotorSpeed(float speed) { motor.setSpeed(speed); }

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

    return busy;
}