#include "hall-sensor.h"

/**
 * Hall Sensor Initialiser
 * 
 * Used to initialise the class object.
 */
HallSensor::HallSensor() {}

/**
 * Hall Sensor Begin Function
 * 
 * Used in the stepper-motor.cpp's begin function, this function is
 * used for setting the predefined sensor pin as an Input-Pullup.
 */
void HallSensor::begin() { pinMode(HALL_PIN, INPUT_PULLUP); }

/**
 * Hall Sensor Is Open Function
 * 
 * Used to check if the reading from the hall sensor is above the 
 * set sensitivity.
 * 
 * @returns true if reading above sensitivity, false otherwise
 */
bool HallSensor::isOpen() {
    uint16_t val = analogRead(HALL_PIN);
    return val > _sensitivity;
    return true;
}

/**
 * Hall Sensor Set Sensitivity Function
 * 
 * Used to set the sensitivity of the hall sensor.
 * 
 * @param s hall sensor sensitivity
 */
void HallSensor::setSensitivity(int s) { _sensitivity = s; }