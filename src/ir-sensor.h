#include "config.h"

// IR Sensor Defintions
#define SENSOR_PIN 35               // Sensor Input Pin
#define SENSOR_INTERVAL_US 500      // Trigger Interval

/**
 * IR Sensor Class
 * 
 * Class Object used to handle sensor readings from O6P302 sensor.
 */
class IRSensor {
    private:
        static IRSensor* instance;          // Instance of object
        int _count;                         // Capsule Count Tracker
        int _lastState;                     // Previous Sensor State
        unsigned long _last_trigger_us;     // Timing tracker for sensor in microseconds
        bool _active;                       // Sensor Activated Toggle
        bool isTriggered();
    public:
        explicit IRSensor();
        void begin();
        void activate();
        void deactivate();
        int getCount();
        void reset();
        void update();
};