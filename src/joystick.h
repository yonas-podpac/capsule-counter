#include "config.h"

#define LR_PIN 33           // Left-Right Input Pin
#define UD_PIN 32           // Up-Down Input Pin
#define SAMPLE_SIZE 32      // Sample size for Calibration

/**
 * Joystick Direction Enumeration Group
 * 
 * Used to define the cardinal directions on the Joystick, 
 * i.e. up, down, left, and right, none as a default null.
 */
enum class Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/**
 * Joystick Class
 * 
 * Class used to control the Arduino Joystick.
 */
class Joystick {
    private:
        int _rawX;                  // x-value read by the LR Pin
        int _rawY;                  // y-value read by the UD Pin
        int _x;                     // Actual x-position
        int _y;                     // Actual y-position
        int _centreX;               // x-coordinate for centre of Joystick
        int _centreY;               // y-coordinate for centre of Joystick
        int _deadzone = 150;        // Position where reading should not be made
        int _threshold = 180;       // Position from which reading should be made
        int applyDeadzone(int, int);
    public:
        Joystick();
        void begin();
        void calibrateCentre(size_t);
        void setDeadzone(int);
        void setThreshold(int);
        Direction read();
};