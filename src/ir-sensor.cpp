#include "ir-sensor.h"

// Initial IR Sensor Instance should be null-pointer
IRSensor* IRSensor::instance = nullptr;

/**
 * IR Sensor Initialiser
 * 
 * Function used to initialise the IR Sensor variables.
 */
IRSensor::IRSensor() :
    _count(0),
    _lastState(-1),
    _last_trigger_us(0),
    _active(false) {}

/**
 * IR Sensor Begin Fuction
 * 
 * Function used to setup the local instance and set the pins.
 */
void IRSensor::begin() {
    instance = this;
    pinMode(SENSOR_PIN, INPUT);
    Serial.println("[IR Sensor] Setup Complete");
}

/**
 * IR Sensor Activate Function
 * 
 * Function used to activate the IR Sensor.
 */
void IRSensor::activate() { _active = true; }

/**
 * IR Sensor Deactivate Function
 * 
 * Function used to deactivate the IR Sensor.
 */
void IRSensor::deactivate() { _active = false; }

/**
 * IR Sensor Is Triggered Function
 * 
 * Function used to check when the sensor has been triggered,
 * i.e. returning the following signal format:
 * High ^
 *      |       |---|
 *      |       |   |
 * Low  |-------|   |-------->
 * 
 * @return true if IR Sensor is triggered
 */
bool IRSensor::isTriggered() {
    int state = digitalRead(SENSOR_PIN);
    bool change = state != _lastState;
    // Handle Initialisation
    if (_lastState == -1) {
        _lastState = state;
    } else if (state != _lastState) {
        Serial.println("[IR Sensor] Triggered!");
        _lastState = state;
    }

    return change & state == HIGH;
}

/**
 * IR Sensor Get Count Function
 * 
 * Function used for retrieving the current count from the sensor.
 */
int IRSensor::getCount() { return _count; }

/**
 * IR Sensor Reset Function
 * 
 * Function used to reset the current count.
 */
void IRSensor::reset() { _count = 0; }

/**
 * IR Sensor Update Function
 * 
 * Function used to check if the IR sensor has been triggered,
 * if so, incrementing the count. IR Sensor count should only 
 * be incremented if the last triggered time is no greater than
 * the predefined interval.
 */
void IRSensor::update() {
    unsigned long now = micros();
    // Check that the interval is reached
    if (now - _last_trigger_us > SENSOR_INTERVAL_US) {
        _last_trigger_us = now;
        // Increment Sensor if triggered and active
        if (isTriggered() && _active) {
            _count++;
            Serial.print("[IR Sensor] Count: ");
            Serial.println(_count);
        }
    }
}