#include "config.h"
#include "display.h"
#include "index-tracker.h"

// Drawer Definitions
#define MENU_COUNT 6                                    // Number of Menu Items
#define SETTING_COUNT 9                                 // Number of Setting Items
#define SCREEN_COUNT MENU_COUNT + SETTING_COUNT + 2     // Screen Count, i.e. MENU_COUNT + SETTING_COUNT + 2 (logo and main menu)
#define MAX_DISPLAYED_BULK_SELECT 4                     // Maximum number of Bulk-select options
#define HELP_MESSAGE_COUNT 3                            // Number of Help Message descriptions

/**
 * Screen Enumerated Group
 * 
 * Class used to define the available screens to be drawn.
 */
enum class Screen {
    LOGO,                           // Logo Screen - Podpac Logo display
    MAIN,                           // Main Menu Sceen - Menu items on display
    COUNT,                          // Count Screen - Current capsule count on display
    SELECT,                         // Select Screen - Bulk options selection on display
    NEW,                            // New Screen - Create new bulk option
    MANUAL,                         // Manual Screen - Drive Motor
    HOME,                           // Homing Screen - Home Stepper Motor
    SETTINGS,                       // Settings Screen - Settings items on display
    HOLD_POS_SETTING,               // Hold Position Setting Screen - Set the hold position setting 
    RELEASE_POS_SETTING,            // Release Position Setting Screen - Set the release position setting
    HOLD_TIME_SETTING,              // Hold Time Setting Screen - Set the hold time setting
    MOTOR_PULSE_WIDTH_SETTING,      // Motor Pulse Width Setting - Set the pulse width for the stepper motor
    MOTOR_MAX_SPEED_SETTING,        // Motor Max Speed Setting - Set the maximum speed for the stepper motor
    MOTOR_ACCELERATION_SETTING,     // Motor Acceleration Setting - Set the acceleration for the stepper motor
    HALL_SENSITIVITY_SETTING,       // Hall Sensor Sensitivity Setting - Set the sensitivity for the hall effect sensor
    HELP_SETTING,                   // Help Setting - Set the help cycle time setting
    FACTORY_RESET                   // Factory Reset - Set all settings to default values
};

/**
 * Drawer Class
 * 
 * Class object used to update the 128x64 dot matrix.
 */
class Drawer{
    private:
        Display display;            // Display Object
        Screen screen;              // Current Screen
        // Menu Items
        const char* _menuItems[MENU_COUNT] = {
            "1. Count Capsules",                    // Count Capsules Menu Message
            "2. Bulk Select",                       // Bulk-Packing Options Menu Message
            "3. New Bulk Packs",                    // Assign new Bulk-Packing Option Menu Message
            "4. Manual Drive",                      // Manually Drive Motor
            "5. Home Motor",                        // Home Motor
            "6. Settings"                           // Settings Menu Message
        };
        // Settings Items
        const char* _settingsItems[SETTING_COUNT] = {
            "1. Hold-Position",                     // Hold Position Setting Message
            "2. Release-Position"                   // Release Position Setting Message
            "3. Hold-Time",                         // Hold Time Setting Message
            "4. Pulse-Width",                       // Pulse Width Setting Message
            "5. Motor Speed",                       // Motor Speed Setting Message
            "6. Motor Acceleration",                // Motor Acceleration Setting Message
            "7. Homing Sensitivity",                // Homing Sensitvity Setting Message
            "8. Help Message",                      // Help Message Update Time Setting Message
            "9. Factory Reset"                      // Factory Reset Message
        };
        // Help Messages
        const char* _helpMessages[SCREEN_COUNT][HELP_MESSAGE_COUNT] = {
            {"", "", ""},
            {"Up/Down = Select", "Right = Confirm", ""},
            {"Left = Main Menu", "", ""},
            {"Up/Down = Select", "Right = Confirm", "Left = Main Menu"},
            {"Up/Down = Change", "Right = Confirm", "Left = Cancel"},
            {"Up = Forward", "Down = Backward", "Left = Cancel"},
            {"Right = Home Motor", "Left = Main Menu", ""},
            {"Up/Down = Select", "Right = Confirm", "Left = Main Menu"},
            {"Up/Down = Change", "Right = Confirm", "Left = Settings"},
            {"Up/Down = Change", "Right = Confirm", "Left = Settings"},
            {"Up/Down = Change", "Right = Confirm", "Left = Settings"},
            {"Up/Down = Change", "Right = Confirm", "Left = Settings"},
            {"Up/Down = Change", "Right = Confirm", "Left = Settings"},
            {"Up/Down = Change", "Right = Confirm", "Left = Settings"},
            {"Up/Down = Change", "Right = Confirm", "Left = Settings"},
            {"Up/Down = Change", "Right = Confirm", "Left = Settings"}
        };
        void drawLogoScreen(U8G2&);
        void drawMainMenuScreen(U8G2&);
        void drawCountScreen(U8G2&);
        void drawSelectionScreen(U8G2&);
        void drawNewCountScreen(U8G2&);
        void drawManualScreen(U8G2&);
        void drawHomeScreen(U8G2&);
        void drawSettingsScreen(U8G2&);
        void drawHoldPosSettingScreen(U8G2&);
        void drawReleasePosSettingScreen(U8G2&);
        void drawHoldTimeSettingScreen(U8G2&);
        void drawMotorPulseWidthSettingScreen(U8G2&);
        void drawMotorMaxSpeedSettingScreen(U8G2&);
        void drawMotorAccelerationSettingScreen(U8G2&);
        void drawHallSensitivitySettingScreen(U8G2&);
        void drawHelpSettingsScreen(U8G2&);
        void drawFactoryReset(U8G2&);
        void drawHelpMessage(U8G2&);
    public:
        IndexTracker idxTracker;
        Drawer();
        void begin();
        Screen getCurrentScreen();
        void setCurrentScreen(Screen);
        void draw();
};