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
    _currentPosition(0) {
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
    _prox_sensor.begin();
    _prox_sensor.setSensitivity(hs);

    Serial.println("[Stepper Motor] Setup Complete");
}

/**
 * Stepper Motor Move Forwards Function
 * 
 * Function used to move the stepper motor forwards.
 * 
 * @param freq desired frequency of motor
 */
void Motor::moveForwards(int freq) {
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
 * Stepper Motor Home Function
 * 
 * Function used to home the stepper motor.
 */
void Motor::home() { homingState = HomingState::SHOULD_HOME; }

/**
 * Stepper Motor Update Function
 * 
 * TODO: write function description
 */
void Motor::update() {
    // Handle Homing State
    switch(homingState) {
        case(HomingState::SHOULD_HOME):
            // Begin Approaching Sensor Sequence
            homingState = HomingState::APPROACHING_SENSOR;
            moveBackwards(100);
            break;
        case(HomingState::APPROACHING_SENSOR):
            // Begin Backing Off Sequence if Proximity Sensor is Triggered
            if (!_prox_sensor.isOpen()) {
                stop();
                stepper->setCurrentPosition(0);
                homingState = HomingState::BACKING_OFF;
                stepper->move(200);
            }
            break;
        case(HomingState::BACKING_OFF):
            // Complete Homing Sequence if Proximity Sensor is no longer Triggered
            if (_prox_sensor.isOpen()) {
                stop();
                stepper->setCurrentPosition(0);
                homingState = HomingState::COMPLETE;
            }
            break;
    };
}