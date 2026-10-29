#include "motor.h"

/**
 * Stepper Motor Initialiser
 * 
 * Function used to set the stepper motor variables.
 */
Motor::Motor() :
    _stepper(1, STEP_PIN, DIR_PIN),
    _prox_sensor(),
    _busy(false),
    _homed(false),
    _currentSpeed(0),
    _stepInterval(0),
    _lastStepTime(0),
    _currentPosition(0) {}

/**
 * Stepper Motor Begin Function
 * 
 * Function used to set the pins and setup proximity sensor.
 */
void Motor::begin() {
    pinMode(ENABLE_PIN, OUTPUT);

    // Initially disable motor
    disable();

    // Initialise Proximity Sensor
    _prox_sensor.begin();

    Serial.println("[Stepper Motor] Setup Complete");
}

/**
 * Stepper Motor Enable Function
 * 
 * Function used to enable the motor driver.
 */
void Motor::enable() { digitalWrite(ENABLE_PIN, LOW); }

/**
 * Stepper Motor Disable Function
 * 
 * Function used to disable the motor driver.
 */
void Motor::disable() { digitalWrite(ENABLE_PIN, HIGH); }

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
    // Enable A4988 Dirver
    enable();
    // Approach Sensor
    _stepper.moveTo(100000);    // ridiculously big number
    while(_prox_sensor.isOpen()) _stepper.run();

    // Stop & Reset
    _stepper.setCurrentPosition(0);

    // Precision Back-Off
    _stepper.moveTo(-200);
    while(!_prox_sensor.isOpen()) _stepper.runSpeed();

    // Reset Home Position
    _stepper.setCurrentPosition(0);
    _homed = true;
    Serial.println("[Stepper Motor] Homed!");
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
 * Stepper Motor Set Pulse Width Function
 * 
 * Used to set the motor's pulse width using the AccelStepper's
 * setMinPulseWidth function.
 * 
 * @param pw motor pulse-width
 */
void Motor::setPulseWidth(int pw) { _stepper.setMinPulseWidth(pw); }

/**
 * Stepper Motor Set Max Speed Function
 * 
 * Used to set the motor's maximum speed using the AccelStepper's
 * setMaxSpeed function.
 * 
 * @param ms motor max-speed (steps/second)
 */
void Motor::setMaxSpeed(float ms) { _stepper.setMaxSpeed(ms); }

/**
 * Stepper Motor Set Acceleration Function
 * 
 * Used to set the motor's acceleration using the AccelStepper's
 * setAcceleration function.
 * 
 * @param a motor acceleration (steps/second^2)
 */
void Motor::setAcceleration(float a) { _stepper.setAcceleration(a); }

/**
 * Stepper Motor Set Hall Sensor Sensitivity Function
 * 
 * Used to set the hall sensor's sensitivity.
 * 
 * @param s hall sensor sensitivity
 */
void Motor::setHallSensitivity(int s) { _prox_sensor.setSensitivity(s); }

/**
 * Stepper Motor Update Function
 * 
 * Function used to check if the stepper motor must be driven,
 * if so, then the busy variable should be toggled.
 */
void Motor::update() {
    // Use AccelStepper's distanceToGo function
    long distanceToGo = _stepper.distanceToGo();
    if (distanceToGo != 0) {
        Serial.print("[Stepper Motor] Distance to go: ");
        Serial.println(distanceToGo);
        _stepper.run();
    }

    // Target Reached
    else {
        // disable();   // Uncomment if holding torque not required
        _busy = false;
    }
}