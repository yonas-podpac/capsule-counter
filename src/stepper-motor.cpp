#include "stepper-motor.h"

/**
 * Stepper Motor Initialiser
 * 
 * Sets up the Stepper Motor's variables, including setting up the 
 * AccelStepper object, setting up the Hall-Sensor and setting the
 * busy variable to false.
 */
StepperMotor::StepperMotor():
    _stepper(1, STEPPIN, DIRPIN),
    _prox_sensor(),
    busy(false),
    _homed(false) {}

/**
 * Stepper Motor Begin Function
 * 
 * Used in logic-handler.cpp's begin function, this function is used for 
 * setting the predefined enable pin as an output, to disable the A4988 
 * driver and to call the Hall-Sensor's begin function.
 */
void StepperMotor::begin() {
    pinMode(ENABLEPIN, OUTPUT);
    
    // Start Disabled to prevent accidental movement / heat
    disable();

    // Initialise Hall Sensor
    _prox_sensor.begin();
}

/**
 * Stepper Motor Enable Function
 * 
 * Used to enable the A4988 driver. The pin should be sent a low signal
 * to indicate that it is enabled, as it is an active-low.
 */
void StepperMotor::enable() { digitalWrite(ENABLEPIN, LOW); }

/**
 * Stepper Motor Disable Function
 * 
 * Used to disable the A4988 driver. The pin should be sent a high signal
 * to indicate that it is disabled, as it is an inactive-high.
 */
void StepperMotor::disable() { digitalWrite(ENABLEPIN, HIGH); }

/**
 * Stepper Motor Move To Function
 * 
 * Used to drive the stepper motor some number of steps from its current
 * position.
 * 
 * @param absoluteSteps number of steps to be driven
 */
void StepperMotor::moveTo(long absoluteSteps) {
    // Enable A4988 driver
    enable();
    // Use AccelStepper moveTo function for Absolute Motion
    _stepper.moveTo(absoluteSteps);
    Serial.print("Distance to Travel: ");
    Serial.println(absoluteSteps);
    busy = true;
}

/**
 * Stepper Motor Move Relative Function
 * 
 * Used to drive the stepper motor some number of steps from its home
 * position.
 * 
 * @param relativeSteps number of steps to be driven
 */
void StepperMotor::moveRelative(long relativeSteps) {
    // Enable A4988 driver
    enable();
    // Use AccelStepper move function for Relative Motion
    _stepper.move(relativeSteps);
    busy = true;
}

/**
 * Stepper Motor Is Busy Function
 * 
 * Used to determine if the stepper motor is being driven or not.
 * 
 * @return true if motor is being drive, false otherwise
 */
bool StepperMotor::isBusy() { return busy; }

bool StepperMotor::isHomed() { return _homed; }

/**
 * Stepper Motor Update Function
 * 
 * Used in the logic-handler.cpp's update function to check if the
 * motor has to be driven, if so, then the busy variable should be 
 * set to be true, otherwise the busy variable is set to be false.
 */
void StepperMotor::update() {
    // Using the AccelStepper's distanceToGo function, check if
    // motor should be driven
    long distanceToGo = _stepper.distanceToGo();
    if (busy) {
        Serial.print("Distance to go: ");
        Serial.println(distanceToGo);
    }
    if (_stepper.distanceToGo() != 0) {
        Serial.println("Motor running");
        _stepper.run();
    }

    // Optional: Auto-disable power when target is reached to 
    // save energy/heat
    else {
        // disable(); // Only Uncomment if you don't need holding torque
        busy = false;
    }
    // _prox_sensor.isOpen();
}

/**
 * Stepper Motor Home Function
 * 
 * Used to home the stepper motor using the Hall-Sensor.
 */
void StepperMotor::home() {
    // Enable the A4988 Driver
    enable();
    // 1. Approach Sensor
    // _stepper.setSpeed(-300);
    _stepper.moveTo(100000);
    while(_prox_sensor.isOpen()) {
        _stepper.run();
    }

    // 2. Immediate Stop and Reset
    _stepper.setCurrentPosition(0);

    // 3. Precision Back-off
    // Move away until sensor is clears, then stop
    _stepper.moveTo(-200);
    while(!_prox_sensor.isOpen()) {
        _stepper.runSpeed();
    }

    // Final zeroing at the exact release point
    _stepper.setCurrentPosition(0);
    _homed = true;
}

void StepperMotor::setSpeed(float speed) {
    if (speed == _currentSpeed) return;

    _currentSpeed = speed;
    if (speed != 0.0) {
        // Calculate time interval between steps in microseconds
        _stepInterval = (unsigned long)(1000000.0 / fabs(speed));
    } else {
        _stepInterval = 0; // Motor stopped
    }
}

bool StepperMotor::runSpeed() {
    // Don't step if speed is 0
    if (_stepInterval == 0) return false;

    unsigned long timeNow = micros();

    // Check if enough time has elapsed since the last step
    if (timeNow - _lastStepTime >= _stepInterval) {
        if (_currentSpeed > 0) {
            _currentPosition++;
            stepForward();   // Your hardware step trigger (e.g. pulse STEP pin HIGH/LOW)
        } else {
            _currentPosition--;
            stepBackward();  // Your hardware step trigger
        }

        _lastStepTime = timeNow;
        return true; // A step was taken
    }

    return false; // No step was due yet
}

void StepperMotor::stepForward() {
    digitalWrite(DIRPIN, HIGH);
    digitalWrite(STEPPIN, HIGH);
    delayMicroseconds(2);
    digitalWrite(STEPPIN, LOW);
}

void StepperMotor::stepBackward() {
    digitalWrite(DIRPIN, LOW);
    digitalWrite(STEPPIN, HIGH);
    delayMicroseconds(2);
    digitalWrite(STEPPIN, LOW);
}

/**
 * Stepper Motor Set Pulse Width Function
 * 
 * Used to set the motor's pulse width using the AccelStepper's
 * setMinPulseWidth function.
 * 
 * @param pw motor pulse-width
 */
void StepperMotor::setPulseWidth(int pw) { _stepper.setMinPulseWidth(pw); }

/**
 * Stepper Motor Set Max Speed Function
 * 
 * Used to set the motor's maximum speed using the AccelStepper's
 * setMaxSpeed function.
 * 
 * @param ms motor max-speed (steps/second)
 */
void StepperMotor::setMaxSpeed(float ms) { _stepper.setMaxSpeed(ms); }

/**
 * Stepper Motor Set Acceleration Function
 * 
 * Used to set the motor's acceleration using the AccelStepper's
 * setAcceleration function.
 * 
 * @param a motor acceleration (steps/second^2)
 */
void StepperMotor::setAcceleration(float a) { _stepper.setAcceleration(a); }

/**
 * Stepper Motor Set Hall Sensor Sensitivity Function
 * 
 * Used to set the hall sensor's sensitivity.
 * 
 * @param s hall sensor sensitivity
 */
void StepperMotor::setHallSensorSensitivity(int s) { _prox_sensor.setSensitivity(s); }