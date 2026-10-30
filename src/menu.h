#include "config.h"
#include "drawer.h"
#include "joystick.h"

/**
 * Menu Class
 * 
 * Class object used to manage the screen using the joystick,
 */
class Menu {
    private:
        Drawer drawer;
        Joystick joystick;
        bool _redraw;                               // Redraw Toggle
        unsigned long _logoIntervalMs;              // Logo Interval (milliseconds)
        unsigned long _lastJoystickChangeMs;        // Joystick Change Tracker (milliseconds)
        unsigned long _joystickChangeIntervalMs;    // Joystick Sensitivity (milliseconds)
        size_t _bulkOptionsCount;
        void handleTransition();
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
        void handleFactoryReset(Direction);
        void handleSelectionOverflow();
    public:
        Menu();
        void begin();
        void update();
};