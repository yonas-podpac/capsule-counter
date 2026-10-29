#include "config.h"

// Proximity Sensor Definitions
#define PROX_PIN 34     // Proximity-Sensor Input Pin

/**
 * Proximity Sensor Class
 * 
 * Class object used to process data from PSD1202-NP
 * hall-effect sensor.
 */
class ProxSensor {
    private:
        int _sensitivity;       // Sensor Reading Sensitivity
    public:
        ProxSensor();
        void begin();
        bool isOpen();
        void setSensitivity(int);
};