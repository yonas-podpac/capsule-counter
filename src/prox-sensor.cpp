#include "prox-sensor.h"

/**
 * Proximity Sensor Initialiser
 * 
 * Function used to intiailise the proximity sensor class object.
 */
ProxSensor::ProxSensor() {}

/**
 * Proximity Sensor Begin Function
 * 
 * Function used to set the pins for the proximity sensor.
 */
void ProxSensor::begin() {
    pinMode(PROX_PIN, INPUT_PULLUP);
    Serial.println("[Proximity Sensor] Setup Complete");
}
/**
 * Proximity Sensor Is Open Function
 * 
 * Function used to check if the reading from the proximity
 * sensor is above the predefined sensitivity.
 * 
 * @return true if reading is above sensitivity
 */
bool ProxSensor::isOpen() {
    uint16_t val = analogRead(PROX_PIN);
    if (val > _sensitivity) Serial.println("[Proximity Sensor] Not Triggered!");
    else Serial.println("[Proximity Sensor] Triggered!");
    return val > _sensitivity;
}

/**
 * Proximity Sensor Set Sensitivity Function
 * 
 * Function used to set the sensitivity of the sensor.
 * 
 * @param s proximity sensor sensitivity
 */
void ProxSensor::setSensitivity(int s) { _sensitivity = s; }