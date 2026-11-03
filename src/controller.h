#include "config.h"
#include "ir-sensor.h"
#include "motor.h"
#include "settings.h"

// Controller Definitions
#define FORWARDS_SPEED -400
#define BACKWARDS_SPEED 400
#define STOP_SPEED 0

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
        Settings settings;                      // Preferences Manager Object
        volatile ControlState _currentState;    // Current Control State
        int _bulkLimit;                         // Bulk Count Limit
        long _holdPos;                          // Hold Position
        long _releasePos;                       // Release Position
        unsigned long _holdTime;                // Hold Time
        bool motorEnabled;                      // Motor Enabled Toggle
        bool irsensorEnabled;                   // IR Sensor Enabled Toggle
    public:
        Controller();
        void begin(Settings&);
        void enableIRSensor();
        void disableIRSensor();
        void enableMotor();
        void disableMotor();
        void moveForwards();
        void moveBackwards();
        void stop();
        void home();
        void updateSettings();
        bool update();
};