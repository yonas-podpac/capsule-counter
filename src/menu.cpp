#include "menu.h"

Drawer drw;
Joystick jystk;

/**
 * Menu Initialiser
 * 
 * Function used to initialise the Menu variables.
 */
Menu::Menu() :
    drawer(drw),
    joystick(jystk),
    _redraw(true),
    _logoIntervalMs(5000),
    _lastJoystickChangeMs(0),
    _joystickChangeIntervalMs(180),
    _lastHelpChangeMs(0),
    _helpScrollIntervalMs(50) {}

/**
 * Menu Begin Function
 * 
 * Function used to set up menu variables.
 */
void Menu::begin(Settings& settings) {
    drawer.begin(settings);
    joystick.begin();
    Serial.println("[Menu] Setup Complete");
    drawer.draw();
}

/**
 * Menu Handle Transition Function
 * 
 * Function used to read the current joystick values and
 * pass the computed direction to the required screen.
 */
void Menu::handleTransition() {
    // Retrieve Direction Reading
    Direction dir = joystick.read();

    // Process movements within interval
    unsigned long now = millis();
    if (now - _lastJoystickChangeMs < _joystickChangeIntervalMs) return;
    _lastJoystickChangeMs = now;

    // Manage Transition
    switch(drawer.getCurrentScreen()) {
        case Screen::LOGO:
            handleLogo();
            break;
        case Screen::MAIN:
            handleMainMenu(dir);
            break;
        case Screen::COUNT:
            handleCount(dir);
            break;
        case Screen::SELECT:
            handleSelection(dir);
            break;
        case Screen::NEW:
            handleNewCount(dir);
            break;
        case Screen::MANUAL:
            handleManual(dir);
            break;
        case Screen::HOME:
            handleHome(dir);
            break;
        case Screen::SETTINGS:
            handleSettings(dir);
            break;
        case Screen::HOLD_POS_SETTING:
            handleHoldPosSetting(dir);
            break;
        case Screen::RELEASE_POS_SETTING:
            handleReleasePosSetting(dir);
            break;
        case Screen::HOLD_TIME_SETTING:
            handleHoldTimeSetting(dir);
            break;
        case Screen::MOTOR_PULSE_WIDTH_SETTING:
            handleMotorPulseWidthSetting(dir);
            break;
        case Screen::MOTOR_MAX_SPEED_SETTING:
            handleMotorMaxSpeedSetting(dir);
            break;
        case Screen::MOTOR_ACCELERATION_SETTING:
            handleMotorAccelerationSetting(dir);
            break;
        case Screen::HALL_SENSITIVITY_SETTING:
            handleHallSensitivitySetting(dir);
            break;
        case Screen::HELP_SETTING:
            handleHelpSetting(dir);
            break;
        case Screen::FACTORY_RESET:
            handleFactoryReset(dir);
            break;
    }
}

/**
 * Menu Handle Logo Function
 * 
 * Function used to update the screen if logo finishes
 * displaying.
 */
void Menu::handleLogo() {
    // Check if interval has been reached
    if (millis() > _logoIntervalMs) {
        drawer.setCurrentScreen(Screen::MAIN);
        _redraw = true;
    }
}

/**
 * Menu Handle Main Menu Function
 * 
 * Function used to handle the user selection within the
 * main menu screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleMainMenu(Direction dir) {
    // Handle Joystick Up
    if (dir == Direction::UP) {
        drawer.idxTracker.decrementMainIndex();
        if (drawer.idxTracker.getMainIndex() < 0) {
            drawer.idxTracker.setMainIndex(MENU_COUNT - 1);
        }
        handleSelectionOverflow();
    }

    // Handle Joystick Down
    else if (dir == Direction::DOWN) {
        drawer.idxTracker.incrementMainIndex();
        if (drawer.idxTracker.getMainIndex() >= MENU_COUNT) {
            drawer.idxTracker.setMainIndex(0);
            drawer.idxTracker.setMainOverflow(0);
        }
        handleSelectionOverflow();
    }
    
    // Handle Joystick Right
    else if (dir == Direction::RIGHT) {
        switch(drawer.idxTracker.getMainIndex()) {
            case 0: // 0th Index: Count Screen
                drawer.setCurrentScreen(Screen::COUNT);
                // TODO: motor control?
                _redraw = true;
                break;
            case 1: // 1st Index: Bulk Select Screen
                drawer.setCurrentScreen(Screen::SELECT);
                break;
            case 2: // 2nd Index: New Bulk Select Screen
                drawer.setCurrentScreen(Screen::NEW);
                break;
            case 3: // 3rd Index: Manual Drive Screen
                drawer.setCurrentScreen(Screen::MANUAL);
                break;
            case 4: // 4th Index: Home Motor Screen
                drawer.setCurrentScreen(Screen::HOME);
                break;
            case 5: // 5th Index: Settings Screen
                drawer.setCurrentScreen(Screen::SETTINGS);
                break;
        }

        // TODO: update controller?
    }
}

/**
 * Menu Handle Count Function
 * 
 * Function used to handle the user selection within the
 * capsule counting screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleCount(Direction dir) {
    // Handle Joystick Left
    if (dir == Direction::LEFT) drawer.setCurrentScreen(Screen::MAIN);
}

/**
 * Menu Handle Selection Function
 * 
 * Function used to handle the user selection within the
 * bulk selection screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleSelection(Direction dir) {
    // Handle Joystick Up
    if (dir == Direction::UP) {
        drawer.idxTracker.decrementBulkIndex();
        // Handle Bulk Overflow
        if (drawer.idxTracker.getBulkIndex() < 0) {
            drawer.idxTracker.setBulkIndex(drawer.getBulkOptionsCount());
        }
        handleSelectionOverflow();
    }
    
    // Handle Joystick Down
    else if (dir == Direction::DOWN) {
        drawer.idxTracker.incrementBulkIndex();
        // Handle Bulk Overflow
        if (drawer.idxTracker.getBulkIndex() > drawer.getBulkOptionsCount() + 1) {
            drawer.idxTracker.setBulkIndex(0);
            drawer.idxTracker.setBulkOverflow(0);
        }
        handleSelectionOverflow();
    }

    // Handle Joystick Right
    else if (dir == Direction::RIGHT) {
        // Handle Continuous Count
        if (drawer.idxTracker.getBulkIndex() == 0) drawer.enableContinuousCount();
        // Handle Bulk Selection
        else {
            drawer.disableContinuousCount();
            drawer.setBulkLimit();
            // TODO: Set controller limit
        }

        // Change Screen
        drawer.setCurrentScreen(Screen::COUNT);
        // TODO: set controller settings
    }

    // Handle Joystick Left
    else if (dir == Direction::LEFT) drawer.setCurrentScreen(Screen::MAIN);
}

/**
 * Menu Handle New Count Function
 * 
 * Function used to handle the user selection within the
 * new count entry screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleNewCount(Direction dir) {
    // Handle Joystick Up
    if (dir == Direction::UP) drawer.incrementNewBulkOption();

    // Handle Joystick Down
    else if (dir == Direction::DOWN) {
        drawer.decrementNewBulktOption();
        
        // Prevent Options Less than 30
        if (drawer.getNewBulkOption() <= 30) drawer.setNewBulkOption(30);
    }

    // Handle Joystick Right
    else if (dir == Direction::RIGHT) {
        // Only accept options greater than 20
        if (drawer.getNewBulkOption() >= 30) {
            // Save New Options
            // TODO: save options to preferences
        }
        drawer.setNewBulkOption(30);
        // Change Screen
        drawer.setCurrentScreen(Screen::MAIN);
    }

    // Handle Joystick Left
    else if (dir == Direction::LEFT) {
        drawer.setNewBulkOption(30);
        drawer.setCurrentScreen(Screen::MAIN);
    }
}

/**
 * Menu Handle Manual Function
 * 
 * Function used to handle the user selection within the
 * manual motor driver screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleManual(Direction dir) {}

/**
 * Menu Handle Home Function
 * 
 * Function used to handle the user selection within the
 * motor homing screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleHome(Direction dir) {}

/**
 * Menu Handle Settings Function
 * 
 * Function used to handle the user selection within the
 * settings selection screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleSettings(Direction dir) {}

/**
 * Menu Handle Hold Position Setting Function
 * 
 * Function used to handle the user selection within the
 * hold position setting screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleHoldPosSetting(Direction dir) {}

/**
 * Menu Handle Release Position Setting Function
 * 
 * Function used to handle the user selection within the
 * release position setting screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleReleasePosSetting(Direction dir) {}

/**
 * Menu Handle Hold Time Setting Function
 * 
 * Function used to handle the user selection within the
 * hold time setting screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleHoldTimeSetting(Direction dir) {}

/**
 * Menu Handle Motor Pulse Width Setting Function
 * 
 * Function used to handle the user selection within the
 * motor pulse width setting screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleMotorPulseWidthSetting(Direction dir) {}

/**
 * Menu Handle Motor Maximum Speed Setting Function
 * 
 * Function used to handle the user selection within the
 * motor maximum speed setting screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleMotorMaxSpeedSetting(Direction dir) {}

/**
 * Menu Handle Motor Acceleration Setting Function
 * 
 * Function used to handle the user selection within the
 * motor acceleration setting screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleMotorAccelerationSetting(Direction dir) {}

/**
 * Menu Handle Hall Sensitivity Setting Function
 * 
 * Function used to handle the user selection within the
 * hall sensitivity setting screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleHallSensitivitySetting(Direction dir) {}

/**
 * Menu Handle Help Setting Function
 * 
 * Function used to handle the user selection within the
 * help setting screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleHelpSetting(Direction dir) {}

/**
 * Menu Handle Factory Reset Function
 * 
 * Function used to handle the user selection within the
 * factory reset screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleFactoryReset(Direction dir) {}

/**
 * Menu Handle Selection Overflow Function
 * 
 * Function used for handling the overflow of user selection
 * on the various screens.
 */
void Menu::handleSelectionOverflow() {
    // Handle Main Index Overflow
    int mainIdx = drawer.idxTracker.getMainIndex();
    int mainOverflow = drawer.idxTracker.getMainOverflow();
    if (mainOverflow > 0 && mainIdx < mainOverflow) {
        drawer.idxTracker.decrementMainOverflow();
    } else if (mainIdx >= MAX_DISPLAYED_BULK_SELECT && mainOverflow < MENU_COUNT - MAX_DISPLAYED_BULK_SELECT) {
        drawer.idxTracker.incrementMainOverflow();
    }

    // Handle Bulk Index Overflow
    int bulkIdx = drawer.idxTracker.getBulkIndex();
    int bulkOverflow = drawer.idxTracker.getBulkOverflow();
    if (bulkOverflow > 0 && bulkIdx < bulkOverflow) {
        drawer.idxTracker.decrementBulkOverflow();
    } else if (bulkIdx >= MAX_DISPLAYED_BULK_SELECT && bulkOverflow < drawer.getBulkOptionsCount() - MAX_DISPLAYED_BULK_SELECT) {
        drawer.idxTracker.incrementBulkOverflow();
    }

    // Handle Settings Index Overflow
    int settIdx = drawer.idxTracker.getSettingsIndex();
    int settOverflow = drawer.idxTracker.getSettingsOverflow();
    if (settOverflow > 0 && settIdx < settOverflow) {
        drawer.idxTracker.decrementSettingsOverflow();
    } else if (settOverflow >= MAX_DISPLAYED_BULK_SELECT && settOverflow < SETTING_COUNT - MAX_DISPLAYED_BULK_SELECT) {
        drawer.idxTracker.incrementSettingsOverflow();
    }
}

/**
 * Menu Update Help Scroll Function
 * 
 * Function used to update the help scroll cycling.
 */
void Menu::updateHelpScroll() {
    // Check if message should be cycled
    unsigned long now = millis();
    if (now - _lastHelpChangeMs < _helpScrollIntervalMs) return;
    _lastHelpChangeMs = now;

    // Update Current Index
    drawer.idxTracker.incrementHelpIndex();
    if (drawer.helpMessageCondition()) drawer.idxTracker.setHelpIndex(0);

    _redraw = true;
}

/**
 * Menu Update Function
 * 
 * Function used to check user's screen selection.
 */
void Menu::update() {
    // Handle Joystick Changes
    handleTransition();

    // Update Count when Screen is Counting
    // TODO: implement

    // Update Help Scroll
    if (drawer.getCurrentScreen() != Screen::LOGO) updateHelpScroll();

    // Redraw if required
    if (_redraw) {
        drawer.draw();
        _redraw = false;
    }
}