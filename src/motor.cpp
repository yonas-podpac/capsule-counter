#include "motor.h"

/**
 * Stepper Motor Constructor
 * 
 * Function used to set the stepper motor variables.
 */
Motor::Motor() :
    _prox_sensor(),
    homingState(HomingState::IDLE),
    _currentSpeed(0),
    _currentPosition(0),
    _homingInitiated(false) {
        engine = FastAccelStepperEngine();
        stepper = NULL;
    }

/**
 * Stepper Motor Begin Function
 * 
 * Function used to set the pins and setup proximity sensor.
 * 
 * @param hs hall sensor sensitivity
 */
void Motor::begin(int hs) {
    // Initialise Stepper Motor
    engine.init();
    stepper = engine.stepperConnectToPin(STEP_PIN);
    if (stepper) {
        stepper->setDirectionPin(DIR_PIN, false);
        stepper->setAutoEnable(true);
        stepper->setSpeedInHz(1000);
        stepper->setAcceleration(2000);
    }

    // Initialise Proximity Sensor
    _prox_sensor.setSensitivity(hs);

    Serial.println("[Stepper Motor] Setup Complete");
}

/**
 * Stepper Motor Get State Function
 * 
 * Function used to get the current homing state of the motor.
 * 
 * @return homing state of motor
 */
HomingState Motor::getState() { return homingState; }

/**
 * Stepper Motor Move Forwards Function
 * 
 * Function used to move the stepper motor forwards.
 * 
 * @param freq desired frequency of motor
 */
void Motor::moveForwards(int freq) {
    Serial.printf("[Stepper Motor] Running motor forwards at %dHz\n", freq);
    stepper->setSpeedInHz(freq);
    stepper->runForward();
}

/**
 * Stepper Motor Move Backwards Function
 * 
 * Function used to move the stepper motor backwards.
 * 
 * @param freq desired frequency of motor
 */
void Motor::moveBackwards(int freq) {
    Serial.printf("[Stepper Motor] Running motor backwards at %dHz\n", freq);
    stepper->setSpeedInHz(freq);
    stepper->runBackward();
}

/**
 * Stepper Motor Stop Function
 * 
 * Function used to stop the stepper motor.
 */
void Motor::stop() { stepper->stopMove(); }

/**
 * Stepper Motor Move To Function
 * 
 * Function used to move the stepper motor a desired number of steps.
 * 
 * @param steps number of steps to be taken
 */
void Motor::moveTo(long steps) { stepper->moveTo(steps); }

/**
 * Stepper Motor Home Function
 * 
 * Function used to home the stepper motor.
 */
// void Motor::home() { homingState = HomingState::SHOULD_HOME; }
void Motor::home() {
    Serial.println("[Stepper Motor] Testing motor. Instead of homing, running at 200Hz");
    stepper->setSpeedInHz(200);
    // stepper->runForward();
    MoveResultCode result = stepper->runForward();
    delayMicroseconds(500);
    // Serial.printf("[Stepper Motor] Testing results from runForward: %d\n", (int)result);
}

/**
 * Stepper Motor Update Function
 * 
 * TODO: write function description
 * 
 * @return true if the motor is running
 */
bool Motor::update() {
    if (_prox_sensor.isOpen()) Serial.println("[Stepper Motor] Prox Sensor Open");
    else Serial.println("[Stepper Motor] Prox Sensor Closed");
    // Handle Homing State
    switch(homingState) {
        case(HomingState::SHOULD_HOME):
            // Begin Approaching Sensor Sequence
            Serial.println("[Stepper Motor] Homing sequence started; Motor beginning to appraoch sensor");
            stepper->setSpeedInHz(200);
            stepper->setAcceleration(1000);
            stepper->moveTo(-5000000);
            // stepper->runBackward();
            homingState = HomingState::APPROACHING_SENSOR;
            // if (!_homingInitiated) {
            //     stepper->setSpeedInHz(200);
            //     stepper->move(-5000);
            //     _homingInitiated = true;
            // }
            // if (stepper->isRunning()) {
            //     Serial.println("[Stepper Motor] Check");
            //     _homingInitiated = false;
            // }
            break;
        case(HomingState::APPROACHING_SENSOR):
            // Begin Backing Off Sequence if Proximity Sensor is Triggered
            // if (stepper->isRunning() && !_prox_sensor.isOpen()) {
            if (!_prox_sensor.isOpen()) {
                Serial.println("[Stepper Motor] Proximity sensor triggered; Initiating backing off sequence");
                stepper->forceStop();
                delay(100);
                stepper->setCurrentPosition(0);
                stepper->setSpeedInHz(200);
                stepper->move(200);
                homingState = HomingState::BACKING_OFF;
            }
            // if (!_prox_sensor.isOpen()) {
            //     stop();
            //     stepper->setCurrentPosition(0);
            //     stepper->move(200);
            //     _homingInitiated = true;
            // }
            // if (_homingInitiated && stepper->isRunning()) {
            //     homingState = HomingState::BACKING_OFF;
            //     _homingInitiated = false;
            // }
            break;
        case(HomingState::BACKING_OFF):
            // Complete Homing Sequence if Proximity Sensor is no longer Triggered
            if (stepper->isRunning() && _prox_sensor.isOpen()) {
                Serial.println("[Stepper Motor] Proximity sensor no longer triggered. Motor homed and new home position set");
                stepper->forceStop();
                stepper->setCurrentPosition(0);
                homingState = HomingState::COMPLETE;
            }
            // if (_prox_sensor.isOpen()) {
            //     Serial.println("[Stepper Motor] Proximity sensor no longer triggered. Motor homed and new home position set");
            //     stop();
            //     stepper->setCurrentPosition(0);
            //     homingState = HomingState::COMPLETE;
            // }
            break;
    };

    return stepper->isRunning();
}