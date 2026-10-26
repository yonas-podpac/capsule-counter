#include "joystick.h"

/**
 * Joystick Initialiser
 * 
 * Sets up the Joystick's pins as input pins, sets the resolution
 * on reading the analog signals, and calibrates the centre position
 * of the Joystick.
 */
Joystick::Joystick() {
    pinMode(LR_PIN, INPUT);
    pinMode(UD_PIN, INPUT);
    analogReadResolution(12);
    calibrateCentre(SAMPLE_SIZE);
}

/**
 * Joystick Calibrate Centre Function
 * 
 * Used when the joystick is setup, and defines the centre position.
 * Note: the joystick should not be used while it is being calibrated.
 * 
 * @param samples Sample size for calibration
 */
void Joystick::calibrateCentre(size_t samples) {
    long sx = 0;    // Sum of x-values
    long sy = 0;    // Sum of y-values

    // Iterate up to Sample size
    for (size_t i = 0; i < samples; i++) {
        sx += analogRead(LR_PIN);
        sy += analogRead(UD_PIN);
        delay(5);
    }

    // Centre Point is the average of the Samples
    _centreX = sx / static_cast<int>(samples);
    _centreY = sy / static_cast<int>(samples);
}

/**
 * Joystick Apply Deadzone Function
 * 
 * Used to prevent small movements triggering joystick motion.
 * 
 * @return value of movement or 0
 */
int Joystick::applyDeadzone(int value, int deadzone) {
    // Only allow movements above deadzone values
    if (abs(value) < deadzone) {
        return 0;
    }

    return value;
}

/**
 * Joystick Set Deadzone Function
 * 
 * Used to change the deadzone value.
 * 
 * @param dz new deadzone value
 */
void Joystick::setDeadzone(int dz) { _deadzone = dz; }

/**
 * Joystick Set Threshold Function
 * 
 * Used to change the threshold value.
 * 
 * @param thres new threshold value
 */
void Joystick::setThreshold(int thres) { _threshold = thres; }

/**
 * Joystick Read Function
 * 
 * Used to read the current position of the joystick.
 * 
 * @return direction of the joystick, or none
 */
Direction Joystick::read() {
    // Read the current x-y values of the joystick
    _rawX = analogRead(LR_PIN);
    _rawY = analogRead(UD_PIN);

    // Ensure x-y values are outside the deadzone
    _x = applyDeadzone(_rawX - _centreX, _deadzone);
    _y = applyDeadzone(_rawY - _centreY, _deadzone);

    // Check if values exceed the threshold
    if (_x < -_threshold) return Direction::LEFT;
    if (_x > _threshold) return Direction::RIGHT;
    if (_y > _threshold) return Direction::UP;
    if (_y < -_threshold) return Direction::DOWN;
    
    return Direction::NONE;
}