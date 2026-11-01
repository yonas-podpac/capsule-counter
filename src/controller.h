#include "config.h"
#include "ir-sensor.h"
#include "motor.h"

/* Controller States */
enum ControlState {
    IDLE,
    HOME,
    COUNT,
    HOLD,
    RELEASE,
    MANUAL,
    ERROR
};

/**
 * Controller Class
 * 
 * Class object used to control hardware, including sensors & motor.
 */
class Controller {
    private:
        IRSensor sensor;                        // IR Sensor Object
        Motor motor;                            // Stepper Motor Object
        volatile ControlState _currentState;    // Current Control State
        int _bulkLimit;                         // Bulk Count Limit
        long _holdPos;                          // Hold Position
        long _releasePos;                       // Release Position
        unsigned long _holdTime;                // Hold Time
    public:
        Controller();
        void begin(long, long, unsigned long, int, float, float, int);
        bool update();
};