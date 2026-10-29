#include "config.h"
#include "display.h"
#include "joystick.h"
#include "settings.h"
#include "logic-handler.h"

#define MENU_COUNT 6                                    // Number of Menu Items
#define SETTING_COUNT 9                                 // Number of Setting Items
#define SCREEN_COUNT MENU_COUNT + SETTING_COUNT + 2     // Screen Count, i.e. MENU_COUNT + SETTING_COUNT + 2 (logo and main menu)
#define MAX_DISPLAYED_BULK_SELECT 4                     // Maximum number of Bulk-select options
#define HELP_MESSAGE_COUNT 3                            // Number of Help Message descriptions

/**
 * Menu Screen Enumerated Group
 * 
 * Used to identify the current screen to be displayed on
 * the dot-matrix.
 */
enum class Screen{
    LOGO,                           // Logo Screen - Podpac Logo display
    MAIN,                           // Main Menu Sceen - Menu items on display
    COUNT,                          // Count Screen - Current capsule count on display
    SELECT,                         // Select Screen - Bulk options selection on display
    NEW,                            // New Screen - Create new bulk option
    MANUAL,                         // Manual Screen - Drive Motor
    HOME,                           // Homing Screen - Home Stepper Motor
    SETTINGS,                       // Settings Screen - Settings items on display
    HOLD_SETTING,                   // TODO: write description for this
    JOYSTICK_SETTING,               // TODO: write description for this
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

enum class MenuState {
    IDLE,           // Idle State - No functions being executed
    HOMING,         // Homing State - Stepper motor driving to home position
    COUNTING,       // Counting State - IR sensor counting capsules 
    HOLDING,        // Holding State - Stepper motor driving to hold position
    RELEASING,      // Releasing State - Stepper motor driving to release position
    MANUAL,         // Manual State - Stepper Motor on Manual Drive
    ERROR           // Error state - Handler raising some error
};

/**
 * Menu Class
 * 
 * Class used to determine and control the 128x64 dot matrix
 * using a joystick and a number pad.
 */
class Menu {
    private:
        Display& display;                           // Display object controlling dot-matrix
        Joystick& joystick;                         // Joystick object
        Settings& settings;                         // Settings object
        LogicHandler logicHandler;                  // Logic Handler object
        Screen screen;                              // Current Screen

        MenuState _menuState;

        int _idx;                                   // Main Menu Index
        int _overflowMainMin;                       // Overflow Minimum for Main Options
        int _bulkSelectIndex;                       // Bulk Select Index
        int _overflowBulkMin;                       // Overflow Minimum for Bulk Options
        int _overflowBulkIndex;                     // Overflow Index for Bulk Options
        int _settingsIndex;                         // Settings Index
        int _overflowSettingsMin;                   // Overflow Minimum for Settings
        int _overflowSettingsIndex;                 // Overlow Index for Settings
        unsigned long _lastJoystickChangeMs;        // Joystick change tracker (milliseconds)
        unsigned long _joystickChangeIntervalMs;    // Joystick sensitivity (milliseconds)

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
            "8. Help Message"                       // Help Message Update Time Setting Message
        };
        int _holdPosSetting;                        // Hold Position Setting
        int _releasePosSetting;                     // Release Position Setting
        unsigned long _holdTimeSetting;             // Hold Time Setting
        int _pulseWidthSetting;                     // Pulse Width Setting
        float _motorSpeed;                          // Motor Speed Setting
        float _motorAcceleration;                   // Motor Acceleration Setting
        int _homingSensitivity;                     // Homing Sensitivity Setting
        unsigned long _helpTimeSetting;             // Help Time Setting

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
        int _helpIndex;                             // Help Message Index
        unsigned long _lastHelpChangeMs;            // Help Message Tracker (milliseconds)
        unsigned long _helpScrollIntervalMs;        // Help Scroll Interval (milliseconds)
        int _helpResetFloor;                        // Help Scroll Reset Floor

        unsigned long _logoIntervalMs;              // Logo Interval (milliseconds)

        int _lastDisplayedCount;                    // Last Displayed Count from IR Sensor

        bool _continuousCount;                      // Running Continuous Count Mode
        int _bulkLimit;                             // Minimum value that bulk can be
        int _bulkOptions[MAX_BULK_OPTIONS];         // Bulk Options
        size_t _bulkOptionsCount;                   // Bulk Options Count
        unsigned long _helpTime;                    // Help Time

        bool _stopped;
        bool _forwards;

        int _newBulkOption;                         // New Bulk Option
        
        bool _redraw;                               // Redraw boolean

        void handleTransition();
        void updateCountScreen();
        void handleSelectionOverflow();
        void updateHelpScroll();
        void handleLogo();
        void handleMainMenu(Direction);
        void handleCount(Direction);
        void handleSelection(Direction);
        void handleNewCount(Direction);
        void handleManual(Direction);
        void handleHome(Direction);
        void handleSettings(Direction);
        void handleHoldPosSetting(Direction);
        void handleReleasePosSetting(Direction);
        void handleHoldTimeSetting(Direction);
        void handleMotorPulseWidthSetting(Direction);
        void handleMotorMaxSpeedSetting(Direction);
        void handleMotorAccelerationSetting(Direction);
        void handleHallSensitivitySetting(Direction);
        void handleHelpSetting(Direction);
        // void handleHoldSettings(Direction);
        // void handleHelpSettings(Direction);
        // void handleJoystickSettings(Direction);
        void draw();
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
        void drawHoldSettingsScreen(U8G2&);
        void drawHelpSettingsScreen(U8G2&);
        void drawJoystickSettingsScreen(U8G2&);
        void drawHelpMessage(U8G2&);
        int getScreenIndex(Screen);
        int convertMsToResetFloor(int);
    public:
        Menu(Display&, Joystick&, Settings&, LogicHandler&);
        void begin();
        void update();
        MenuState getMenuState();
        int getBulkLimit();
        int getContinuousCountCallback();
        int* getBulkCountCallback();
};