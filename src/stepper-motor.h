#include "config.h"
#include "AccelStepper.h"
#include "hall-sensor.h"

#define STEPPIN  18         // Step Output Pin
#define DIRPIN 19           // Direction Output Pin
#define ENABLEPIN 21        // Enable Output Pin

/**
 * Stepper Motor Class
 * 
 * Class used to actuate the NEMA17 stepper motor via the A4988 motor
 * driver. The class utilises the AccelStepper library to control the
 * stepper motor.
 */
class StepperMotor {
    private:
        AccelStepper _stepper;      // Stepper Motor Control Library
        HallSensor _prox_sensor;    // Hall-Effect Sensor for Homing
        bool busy;                  // True if motor is driving, false otherwise
        float _currentSpeed;
        bool _homed;
        unsigned long _stepInterval;
        unsigned long _lastStepTime;
        int _currentPosition;
    public:
        StepperMotor();
        void begin();
        void moveTo(long);
        void moveRelative(long);
        bool isBusy();
        bool isHomed();
        void update();
        void enable();
        void disable();
        void home();
        void setSpeed(float);
        bool runSpeed();
        void stepForward();
        void stepBackward();
        void setPulseWidth(int);
        void setMaxSpeed(float);
        void setAcceleration(float);
        void setHallSensorSensitivity(int);
};