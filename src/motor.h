#include "config.h"
#include "AccelStepper.h"
#include "prox-sensor.h"

// Stepper Motor Definitions
#define STEP_PIN 18         // Step Output Pin
#define DIR_PIN 19          // Direction Output Pin
#define ENABLE_PIN 21       // Enable Output Pin

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
        bool _homed;                    // Motor Homed Toggle
        float _currentSpeed;            // Current Motor Speed
        unsigned long _stepInterval;    // Timing Interval for Steps
        unsigned long _lastStepTime;    // Last time the motor took a step
        int _currentPosition;           // Current Motor Position
    public:
        Motor();
        void begin();
        void moveTo(long);
        void moveRelative(long);    // TODO: check if required
        bool isBusy();
        bool isHomed();
        void enable();
        void disable();
        void home();
        void setSpeed(float);
        bool run();
        void stepForward();
        void stepBackward();
        void setPulseWidth(int);
        void setMaxSpeed(float);
        void setAcceleration(float);
        void setHallSensitivity(int);
        void update();
};