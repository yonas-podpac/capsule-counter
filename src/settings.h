#include "config.h"

#define MAX_BULK_OPTIONS 100                        // Maximum number of Bulk Options
#define PROJECT_KEY "capsule-counter"               // Project key name
#define BULK_OPTIONS_KEY "bulk-options"             // Bulk-Options key name
#define DEFAULT_BULK_OPTIONS {60, 100, 500}         // Default Bulk Options
#define HOLD_POS_KEY "hold-position"                // Hold-Position key name
#define DEFAULT_HOLD_POS 200                        // Default Hold-Position
#define RELEASE_POS_KEY "release_pos_key"           // Release-Position key name
#define DEFAULT_RELEASE_POS 100                     // Default Release Position
#define HOLD_TIME_KEY "hold-time"                   // Hold-Time key name
#define DEFAULT_HOLD_TIME 5000                      // Default Hold-Time
#define MOTOR_PULSE_WIDTH_KEY "pulse-width-key"     // Motor Pulse-Width key name
#define DEFAULT_MOTOR_PULSE_WIDTH 20                // Default Motor Pulse-Width
#define MOTOR_MAX_SPEED_KEY "max-speed-key"         // Motor Max-Speed key name
#define DEFAULT_MOTOR_MAX_SPEED 100                 // Default Motor Max-Speed
#define MOTOR_ACCELERATION_KEY "accel-key"          // Motor Acceleration key name
#define DEFAULT_MOTOR_ACCELERATION 200              // Default Motor Acceleration
#define HALL_SENSITIVITY_KEY "sensitivity-key"      // Hall Sensor Sensitivity key name
#define DEFAULT_HALL_SENSITIVITY 2000               // Default Hall Sensor Sensitivity
#define HELP_TIME_KEY "help-time"                   // Help Message Cycle Time key name
#define DEFAULT_HELP_TIME 2000                      // Default Help Message Cycle Time

/**
 * Settings Class
 * 
 * Class used for updating and retrieving values from the Preferences.
 */
class Settings {
    private:
        Preferences pref;                       // Preferences library pointer
        size_t _bulkOptionCount;                // Current size of bulk-packing options
        int _bulkOptions[MAX_BULK_OPTIONS];     // Bulk-packing options list
        long _holdPos;                          // Stepper Motor Hold-Position (steps)
        long _releasePos;                       // Stepper Motor Release-Position (steps)
        unsigned long _holdTimeMs;              // Stepper Motor Hold-Time (microseconds)
        int _pulseWidth;                        // Stepper Motor Pulse-Width
        float _maxSpeed;                        // Stepper Motor Max-Speed (steps/second)
        float _acceleration;                    // Stepper Motor Acceleration (steps/second^2)
        int _hallSensitivity;                   // Hall Sensor Sensitivity
        unsigned long _helpTimeMs;              // Display Help Time Update (microseconds)
        size_t countBulkOptions();
        bool contains(int);
        std::tuple<int, int> partition(int[], int, int);
        void quickSort(int[], int, int);
    public:
        Settings();
        void begin();
        void getBulkOptions(int*);
        size_t getBulkOptionCount();
        void putBulkOptions(int*);
        void addBulkOption(int, int*);
        long getHoldPosition();
        void putHoldPosition(long);
        long getReleasePosition();
        void putReleasePosition(long);
        unsigned long getHoldTime();
        void putHoldTime(unsigned long);
        int getMotorPulseWidth();
        void putMotorPulseWidth(int);
        float getMotorMaxSpeed();
        void putMotorMaxSpeed(float);
        float getMotorAcceleration();
        void putMotorAcceleration(float);
        int getHallSensitivity();
        void putHallSensitivity(int);
        unsigned long getHelpTime();
        void putHelpTime(unsigned long);
        void factoryReset();
};