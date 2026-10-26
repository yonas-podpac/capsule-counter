#include "config.h"

#define SENSOR_PIN 35               // Sensor Input Pin
#define SENSOR_INTERVAL_US 500      // Trigger interval (microseconds)

/**
 * Sensor Class
 * 
 * Class used to get sensor readings from a O6P302 sensor.
 */
class Sensor {
    private:
        static Sensor* instance;            // Instance of class object
        int _count;                         // Tracker for capsule count
        int _lastState;                     // Previous state of sensor
        unsigned long _last_trigger_us;     // Tracker for timing in microseconds
        bool isTriggered();
    public:
        explicit Sensor();
        void begin();
        void update();
        int getCount();
        void reset();
};