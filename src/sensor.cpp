#include "sensor.h"

// Initial Sensor instance is a null pointer
Sensor* Sensor::instance = nullptr;

/**
 * Sensor Initialiser
 * 
 * Sets up the Sensor's variables, such as the count from the sensor,
 * the lastState and the last trigger time in microseconds.
 */
Sensor::Sensor() :
    _count(0),
    _lastState(-1),
    _last_trigger_us(0),
    _active(false) {}

/**
 * Sensor Begin Function
 * 
 * Used in logic-handler.cpp's begin function, this function is used
 * for setting the instance object to be the Sensor class, and for
 * setting the predefined sensor pin to be an input pin.
 */
void Sensor::begin() {
    instance = this;
    pinMode(SENSOR_PIN, INPUT);
}

void Sensor::activate() { _active = true; }

void Sensor::deactivate() { _active = false; }

/**
 * Sensor Is Triggered Function
 * 
 * Used to check when the sensor has been triggered, i.e. returned
 * a signal of the following format:
 * High ^
 *      |      |---|
 *      |      |   |
 * Low  |------|   |---------
 * 
 * @return true if sensor triggered
 */
bool Sensor::isTriggered() {
    int state = digitalRead(SENSOR_PIN);
    bool change = state != _lastState;
    if (state != _lastState) {
        Serial.println("Sensor Triggered!!");
        _lastState = state;
    }

    return change & state == HIGH;
}

/**
 * Sensor Update Function
 * 
 * Used in the logic-handler.cpp's update function to check if the 
 * sensor has been triggered, and if so, to increment the count. 
 * Sensor count should be incremented if the last triggered time is 
 * no greater than the predefined interval.
 */
void Sensor::update() {
    unsigned long now = micros();
    // Check that time interval is reached
    if (now - _last_trigger_us > SENSOR_INTERVAL_US) {
        _last_trigger_us = now;
        // Increment if sensor is triggered
        if (isTriggered() && _active) {
            _count++;
            Serial.print("Count: ");
            Serial.println(_count);
        }
    }
}

/**
 * Sensor Get Count Function
 * 
 * Used for retrieving the current count from the sensor.
 */
int Sensor::getCount() { return _count; }

/**
 * Sensor Reset Function
 * 
 * Used to reset the count.
 */
void Sensor::reset() { _count = 0; }