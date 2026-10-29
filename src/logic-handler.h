#include "config.h"
#include "sensor.h"
#include "stepper-motor.h"

/**
 * Logic Handler State Enumerated Group
 * 
 * Used to identify the current state of the Logic Handler.
 */
enum LogicState {
    IDLE,           // Idle State - No functions being executed
    HOMING,         // Homing State - Stepper motor driving to home position
    COUNTING,       // Counting State - IR sensor counting capsules 
    HOLDING,        // Holding State - Stepper motor driving to hold position
    RELEASING,      // Releasing State - Stepper motor driving to release position
    MANUAL,         // Manual State - Stepper Motor on Manual Drive
    ERROR           // Error state - Handler raising some error
};

/**
 * Logic Handler Class
 * 
 * Class used to control the stepper motor based on the count
 * made by the IR sensor.
 */
class LogicHandler {
    private:
        Sensor& sensor;                 // IR Sensor object
        StepperMotor& motor;            // Stepper Motor object

        volatile LogicState currentState;             // Current Logic Handler State
        int _limit;                     // Bulk-counting limit
        long _holdPosition;             // Stepper Motor Hold Position (steps)
        long _releasePosition;          // Stepper Motor Release Position (steps)
        unsigned long _holdTime;        // Stepper Motor Hold Time (milliseconds)
        unsigned long _lastHoldMs;      // Tracker for hold time (milliseconds)
        unsigned long _homeError;       // Homing Error Timeout (milliseconds)
        void hold();
        void release();
    public:
        LogicHandler(Sensor&, StepperMotor&);
        void begin();
        void home();
        bool update();
        void setState(LogicState);
        LogicState getState();
        bool homed();
        void setManual();
        void setAuto();
        void setMotorSpeed(float);
        void activateSensor();
        void deactivateSensor();
        void setLimit(int);
        int getCurrentCount();
        void setHoldPosition(long);
        void setReleasePosition(long);
        void setHoldTime(unsigned long);
        void setMotorPulseWidth(int);
        void setMotorMaxSpeed(float);
        void setMotorAcceleration(float);
        void setHallSensitivity(int);
};