#include "motor.h"

/**
 * Stepper Motor Initialiser
 * 
 * Function used to set the stepper motor variables.
 */
Motor::Motor() :
    // _stepper(1, STEP_PIN, DIR_PIN),
    _stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN),
    _prox_sensor(),
    _busy(false),
    _shouldHome(false),
    _homed(false),
    homingState(HomingState::IDLE),
    _currentSpeed(0),
    _stepInterval(0),
    _lastStepTime(0),
    _currentPosition(0) {}

/**
 * Stepper Motor Begin Function
 * 
 * Function used to set the pins and setup proximity sensor.
 * 
 * @param pw pulse width
 * @param ms max speed
 * @param a acceleration
 * @param hs hall sensor sensitivity
 */
void Motor::begin(int pw, float ms, float a, int hs) {
    pinMode(ENABLE_PIN, OUTPUT);
    // _stepper.setEnablePin(ENABLE_PIN);
    Serial.print("[Stepper Motor] Pulse width set to ");
    Serial.println(pw);
    _stepper.setMinPulseWidth(pw);
    Serial.print("[Stepper Motor] Max Speed set to ");
    Serial.println(ms);
    _stepper.setMaxSpeed(ms);
    Serial.print("[Stepper Motor] Acceleration set to ");
    Serial.println(a);
    _stepper.setAcceleration(a);
    Serial.print("[Stepper Motor] Prox sensor sensitiviy set to ");
    Serial.println(hs);
    _prox_sensor.setSensitivity(hs);
    _stepper.setPinsInverted(false, true, true);
    
    enable();
    _stepper.setSpeed(-400);
    // setSpeed(-400);
    // _stepper.enableOutputs();
    // // Initially disable motor
    // disable();

    // // Initialise Proximity Sensor
    // _prox_sensor.begin();

    Serial.println("[Stepper Motor] Setup Complete");
}

/**
 * Stepper Motor Enable Function
 * 
 * Function used to enable the motor driver.
 */
void Motor::enable() { 
    Serial.println("[Stepper Motor] Enabled");
    digitalWrite(ENABLE_PIN, LOW); 
}

/**
 * Stepper Motor Disable Function
 * 
 * Function used to disable the motor driver.
 */
void Motor::disable() { 
    Serial.println("[Stepper Motor] Disabled");
    digitalWrite(ENABLE_PIN, HIGH); 
}

/**
 * Stepper Motor Enable Drive Function
 * 
 * Function used to enable the drive toggle.
 */
void Motor::enableDrive() { _drive = true; }

/**
 * Stepper Motor Disable Drive Function
 * 
 * Function used to disable the drive toggle.
 */
void Motor::disableDrive() { _drive = false; }

/**
 * Stepper Motor Move To Function
 * 
 * Function used to set the desired steps the motor should drive to.
 * 
 * @param steps number of steps to be driven
 */
void Motor::moveTo(long steps) {
    // Enable A4988 Driver
    enable();
    // Use AccelStepper moveTo function for Absolute Motion
    _stepper.moveTo(steps);
    _busy = true;

    Serial.print("[Stepper Motor] Steps to be driven: ");
    Serial.println(steps);
}

/**
 * Stepper Motor Move Relative Function
 * 
 * Function used to set the number of steps to be driven to 
 * relative to the home position.
 * 
 * @param steps number of steps to be driven
 */
void Motor::moveRelative(long steps) {
    // Enable A4988 Driver
    enable();
    // Use AccelStepper move function for Relative Motion
    _stepper.move(steps);
    _busy = true;

    Serial.print("[Stepper Motor] Steps relative to home to be driven: ");
    Serial.println(steps);
}

/**
 * Stepper Motor Is Busy Function
 * 
 * Function used to check if motor is busy.
 * 
 * @return true if busy
 */
bool Motor::isBusy() { return _busy; }

/**
 * Stepper Motor Is Homed Function
 * 
 * Function used to check if motor has been homed
 * 
 * @return true if homed
 */
bool Motor::isHomed() { return _homed; }

/**
 * Stepper Motor Home Function
 * 
 * Function used to home stepper motor.
 */
void Motor::home() {
    _shouldHome = true;
    homingState = HomingState::APPROACHING_SENSOR;
    _stepper.moveTo(100000);
    Serial.println("[Stepper Motor] Commencing homing sequence");
    // // Enable A4988 Dirver
    // enable();
    // setSpeed(100);
    // // Approach Sensor
    // Serial.println("[Stepper Motor] Starting to approach proximity sensor");
    // _stepper.moveTo(100000);    // ridiculously big number
    // while(_prox_sensor.isOpen()) _stepper.run();
    // Serial.println("[Stepper Motor] Proximity sensor triggered!");

    // // Stop & Reset
    // _stepper.setCurrentPosition(0);

    // // Precision Back-Off
    // Serial.println("[Stepper Motor] Backing off from proximity sensor");
    // _stepper.moveTo(-200);
    // while(!_prox_sensor.isOpen()) _stepper.runSpeed();
    // Serial.println("[Stepper Motor] Proximity sensor no longer triggered. Resetting zero position");


    // // Reset Home Position
    // _stepper.setCurrentPosition(0);
    // _homed = true;
    // Serial.println("[Stepper Motor] Homed!");
}

/**
 * Stepper Motor Set Speed Function
 * 
 * Function used to set the speed of the stepper motor.
 * 
 * @param speed speed in steps / second
 */
void Motor::setSpeed(float speed) {
    // Do nothing if speed is the same as current speed
    if (speed == _currentSpeed) return;
    Serial.print("[Stepper Motor] Changing speed from ");
    Serial.print(_currentSpeed);
    Serial.print(" steps/second to ");
    Serial.print(speed);
    Serial.println(" steps/second");

    // Set New Speed
    _currentSpeed = speed;
    
    // Recalculate Step Inteveral
    if (speed != 0.0) _stepInterval = (unsigned long)(1000000.0 / fabs(speed));
    else _stepInterval = 0; // Motor Stopped
}

/**
 * Stepper Motor Run Function
 * 
 * Function used to run the stepper motor.
 * 
 * @return true if a step is taken
 */
bool Motor::run() {
    // Don't Step if speed is 0
    if (_stepInterval == 0) return false;

    // Reset Current Time
    unsigned long now = micros();
    
    // Check if enough time has elapsed since the last step
    if (now - _lastStepTime >= _stepInterval) {
        if (_currentSpeed > 0) {
            _currentPosition++;
            stepForward();
        } else {
            _currentPosition--;
            stepBackward();
        }

        _lastStepTime = now;
        return true;    // A step was taken
    }
    return false;       // No steps taken
}

/**
 * Stepper Motor Step Forward Function
 * 
 * Function used to step the motor forwards.
 */
void Motor::stepForward() {
    digitalWrite(DIR_PIN, HIGH);
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(2);
    digitalWrite(STEP_PIN, LOW);
}

/**
 * Stepper Motor Step Backward Function
 * 
 * Function used to step the motor backwards.
 */
void Motor::stepBackward() {
    digitalWrite(DIR_PIN, LOW);
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(2);
    digitalWrite(STEP_PIN, LOW);
}

/**
 * Stepper Motor Update Function
 * 
 * Function used to check if the stepper motor must be driven,
 * if so, then the busy variable should be toggled.
 */
void Motor::update() {
    Serial.println("[Stepper Motor] test");
    bool stepped = _stepper.runSpeed();
    if (stepped) Serial.println("[Stepper Motor] Stepped");
    // // Use AccelStepper's distanceToGo function
    // long distanceToGo = _stepper.distanceToGo();
    // if (distanceToGo != 0) {
    //     Serial.print("[Stepper Motor] Distance to go: ");
    //     Serial.println(distanceToGo);
    //     _stepper.run();
    // }

    // // Target Reached
    // else {
    //     // disable();   // Uncomment if holding torque not required
    //     _busy = false;
    // }





    // if (_drive) {
    //     _stepper.runSpeed();
    // }

    // if (_shouldHome) {
    //     switch(homingState) {
    //         case HomingState::APPROACHING_SENSOR:
    //             _stepper.run();

    //             // Check if Sensor has been Triggered
    //             if (!_prox_sensor.isOpen()) {
    //                 _stepper.setCurrentPosition(0);
    //                 _stepper.moveTo(-200);
    //                 homingState = HomingState::BACKING_OFF;
    //             }
    //             break;
    //         case HomingState::BACKING_OFF:
    //             _stepper.run();

    //             // Check if Sensor is no longer Triggered
    //             if (_prox_sensor.isOpen()) {
    //                 _stepper.setCurrentPosition(0);
    //                 _homed = true;
    //                 _shouldHome = false;
    //                 homingState = HomingState::COMPLETE;
    //                 Serial.println("[Stepper Motor] Homing complete!");
    //             }
    //     };
    // }
}