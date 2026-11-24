#include "prox-sensor.h"

/**
 * Proximity Sensor Constructor
 * 
 * Function used to intiailise the proximity sensor class object.
 */
ProxSensor::ProxSensor() { pinMode(PROX_PIN, INPUT_PULLUP); }

/**
 * Proximity Sensor Is Open Function
 * 
 * Function used to check if the reading from the proximity
 * sensor is above the predefined sensitivity.
 * 
 * @return true if reading is above sensitivity
 */
bool ProxSensor::isOpen() { return analogRead(PROX_PIN) > _sensitivity; }

/**
 * Proximity Sensor Set Sensitivity Function
 * 
 * Function used to set the sensitivity of the sensor.
 * 
 * @param s proximity sensor sensitivity
 */
void ProxSensor::setSensitivity(int s) { _sensitivity = s; }