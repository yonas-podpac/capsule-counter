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
    HOLDING,
    RELEASE,
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
        bool holding;                           // Holding Toggle
        long _releasePos;                       // Release Position
        bool releasing;                         // Releasing Toggle
        unsigned long _holdTime;                // Hold Time
        unsigned long _lastHoldMs;              // Tracker for Holding Time
        bool irsensorEnabled;                   // IR Sensor Enabled Toggle
        bool motorForwards;                     // Move Forwards Toggle
        bool motorBackwards;                    // Move Backwards Toggle
    public:
        Controller();
        void begin(Settings&);
        void setState(ControlState);
        bool inCountState();
        void setLimit(int);
        int getCurrentCount();
        bool motorHomed();
        void enableIRSensor();
        void disableIRSensor();
        void moveForwards();
        void moveBackwards();
        void stop();
        void home();
        void updateSettings();
        bool update();
};