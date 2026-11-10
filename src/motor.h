#include "config.h"
#include "FastAccelStepper.h"
#include "prox-sensor.h"

// Stepper Motor Definitions
#define STEP_PIN 25         // Step Output Pin
#define DIR_PIN 26          // Direction Output Pin
// #define ENABLE_PIN 21       // Enable Output Pin

/* Homing States */
enum class HomingState {
    IDLE,
    SHOULD_HOME,
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
        FastAccelStepperEngine engine;
        FastAccelStepper *stepper;
        ProxSensor _prox_sensor;        // Proximity Sensor Object
        HomingState homingState;        // Homing State
        float _currentSpeed;            // Current Motor Speed
        int _currentPosition;           // Current Motor Position
    public:
        Motor();
        void begin(int);
        void moveForwards(int);
        void moveBackwards(int);
        void stop();
        void home();
        void update();
};