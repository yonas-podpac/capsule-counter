#include "config.h"
#include "AccelStepper.h"
#include "prox-sensor.h"

// Stepper Motor Definitions
#define STEP_PIN 25         // Step Output Pin
#define DIR_PIN 26          // Direction Output Pin
#define ENABLE_PIN 21       // Enable Output Pin

/* Homing States */
enum class HomingState {
    IDLE,
    APPROACHING_SENSOR,
    BACKING_OFF,
    COMPLETE
};

/**
 * Stepper Motor Class
 * 
 * Class object used to actuate the NEMA17 stepper motor
 * via the A4988 motor driver. The class utilises the 
 * AccelStepper library to control the stepper motor.
 */
class Motor {
    private:
        AccelStepper _stepper;          // Stepper Motor Control Object
        ProxSensor _prox_sensor;        // Proximity Sensor Object
        bool _busy;                     // Motor Busy Toggle
        bool _drive;                    // Motor Drive Toggle
        bool _shouldHome;               // Should Home Motor Toggle
        bool _homed;                    // Motor Homed Toggle
        HomingState homingState;        // Homing State
        float _currentSpeed;            // Current Motor Speed
        unsigned long _stepInterval;    // Timing Interval for Steps
        unsigned long _lastStepTime;    // Last time the motor took a step
        int _currentPosition;           // Current Motor Position
    public:
        Motor();
        void begin(int, float, float, int);
        void moveTo(long);
        void moveRelative(long);    // TODO: check if required
        bool isBusy();
        bool isHomed();
        void enable();
        void disable();
        void enableDrive();
        void disableDrive();
        void home();
        void setSpeed(float);
        bool run();
        void stepForward();
        void stepBackward();
        // void setPulseWidth(int);
        // void setMaxSpeed(float);
        // void setAcceleration(float);
        // void setHallSensitivity(int);
        void update();
};