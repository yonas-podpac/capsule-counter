#include "config.h"

#define HALL_PIN 34     // Hall-Sensor Input Pin

/**
 * Hall Sensor Class
 * 
 * Class used to get readings from the PSD1202-NP hall-effect
 * sensor.
 */
class HallSensor {
    private:
        int _sensitivity;       // Sensor Reading Sensitivity
    public:
        HallSensor();
        void begin();
        bool isOpen();
        void setSensitivity(int);
};