#include "menu.h"

/**
 * Menu Initialiser
 * 
 * Sets up the Menu's variables including the various classes
 * and the each of the variables.
 */
Menu::Menu(Display& d, Joystick& j, Settings& s, LogicHandler& l): 
    display(d),
    joystick(j),
    settings(s),
    logicHandler(l),
    screen(Screen::LOGO),
    _menuState(MenuState::IDLE),
    _idx(0),
    _overflowMainMin(0),
    _bulkSelectIndex(0),
    _overflowBulkMin(0),
    _overflowBulkIndex(0),
    _settingsIndex(0),
    _overflowSettingsMin(0),
    _overflowSettingsIndex(0),
    _lastJoystickChangeMs(0),
    _joystickChangeIntervalMs(180),
    _holdPosSetting(0),
    _releasePosSetting(0),
    _holdTimeSetting(0),
    _pulseWidthSetting(0),
    _motorSpeed(0),
    _motorAcceleration(0),
    _homingSensitivity(0),
    _helpTimeSetting(0),
    _helpIndex(-1),
    _lastHelpChangeMs(0),
    _helpScrollIntervalMs(50),
    _helpResetFloor(0),
    _logoIntervalMs(5000),
    _lastDisplayedCount(0),
    _continuousCount(true),
    _bulkLimit(100),
    _newBulkOption(0),
    _stopped(true),
    _forwards(true),
    _redraw(true) {}

/**
 * Menu Begin Function
 * 
 * Used in main.cpp's setup function, this function is used for
 * extracting all of the saved settings and assigning them to the 
 * relevant classes, as well as drawing the logo.
 */
void Menu::begin() {
    // Initialise Joystick
    joystick.begin();

    // Extract Bulk Option Variables from Settings
    settings.getBulkOptions(_bulkOptions);
    _bulkOptionsCount = settings.getBulkOptionCount();

    // Extract Logic Handler Variables from Settings
    logicHandler.setHoldPosition(settings.getHoldPosition());
    logicHandler.setReleasePosition(settings.getReleasePosition());
    logicHandler.setHoldTime(settings.getHoldTime());
    logicHandler.setMotorPulseWidth(settings.getMotorPulseWidth());
    logicHandler.setMotorMaxSpeed(settings.getMotorMaxSpeed());
    logicHandler.setMotorAcceleration(settings.getMotorAcceleration());
    logicHandler.setHallSensitivity(settings.getHallSensitivity());

    // Extract Help Message Update Time from Settings
    _helpTime = settings.getHelpTime();
    _helpTimeSetting = _helpTime;
    _helpResetFloor = convertMsToResetFloor(_helpTime);

    // Draw Logo on Startup
    draw();
}

/**
 * Menu Update Function
 * 
 * Used in Display Updating Task assigned to Core 1 of the
 * ESP32. Handles redrawing of the Dot-Matrix as required.
 */
void Menu::update() {
    // Check user decisions
    handleTransition();

    // Update Count when Screen is Counting
    if (screen == Screen::COUNT) {
        updateCountScreen();
    }

    // Update Help Scroll for all screens apart from Logo
    if (screen != Screen::LOGO) {
        updateHelpScroll();
    }

    // Only redraw when required
    if (_redraw) {
        draw();
        _redraw = false;
    }
}

MenuState Menu::getMenuState() { return _menuState; }

/**
 * Menu Handle Transition Function
 * 
 * Based on the joystick values and current screen, this
 * function is used to manage transitions regardless of 
 * the current screen.
 */
void Menu::handleTransition() {
    // Get the current joystick reading
    Direction dir = joystick.read();

    // Only process if movement occurs during interval
    unsigned long now = millis();
    if (now - _lastJoystickChangeMs < _joystickChangeIntervalMs) {
        return;
    }
    _lastJoystickChangeMs = now;

    // Manage transition based on current screen
    switch(screen) {
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
    }
}

/**
 * Menu Update Count Screen Function
 * 
 * Used to check what the current count is from the IR
 * sensor and only updates while on the count screen.
 */
void Menu::updateCountScreen() {
    // Get the current count from IR Sensor
    int currentCount = logicHandler.getCurrentCount();

    // Only update and redraw if count changes
    if (currentCount != _lastDisplayedCount) {
        _lastDisplayedCount = currentCount;
        _redraw = true;
    }
}

/**
 * Menu Handle Selection Overflow Function
 * 
 * Used for handling overflow options for selection 
 * screens, like the bulk options and the settings.
 */
void Menu::handleSelectionOverflow() {
    // Handle Main Index Overflow
    if (_overflowMainMin > 0 && _idx < _overflowMainMin) {
        _overflowMainMin--;
    } else if (_idx >= MAX_DISPLAYED_BULK_SELECT && _overflowMainMin < MENU_COUNT - MAX_DISPLAYED_BULK_SELECT) {
        _overflowMainMin++;
    }

    // Handle Bulk Index Overflow
    if (_overflowBulkMin > 0 && _bulkSelectIndex < _overflowBulkMin) {
        _overflowBulkMin--;
    } else if (_bulkSelectIndex >= MAX_DISPLAYED_BULK_SELECT && _overflowBulkMin < _bulkOptionsCount - MAX_DISPLAYED_BULK_SELECT) {
        _overflowBulkMin++;
    }

    // Handle Settings Index Overflow
    if (_overflowSettingsMin > 0 && _settingsIndex < _overflowSettingsMin) {
        _overflowSettingsMin--;
    } else if (_settingsIndex >= MAX_DISPLAYED_BULK_SELECT && _overflowSettingsMin < SETTING_COUNT - MAX_DISPLAYED_BULK_SELECT) {
        _overflowSettingsMin++;
    }
}

/**
 * Menu Update Help Scroll Function
 * 
 * Used to update the current help message given
 * a settings-defined cycle time.
 */
void Menu::updateHelpScroll() {
    // Check if help message should be cycled
    unsigned long now = millis();
    if (now - _lastHelpChangeMs < _helpScrollIntervalMs) {
        return;
    }
    _lastHelpChangeMs = now;

    // Update current index for help message
    _helpIndex++;
    int sIdx = getScreenIndex(screen);
    int hIdx = floor(_helpIndex/_helpResetFloor);
    if (hIdx >= HELP_MESSAGE_COUNT || _helpMessages[sIdx][hIdx] == "") {
        _helpIndex = 0;
    }

    _redraw = true;
}

/**
 * Menu Handle Logo Function
 * 
 * Used to check if logo has stayed on the screen
 * for a specified interval, and then prompted to
 * redraw the screen to the main menu.
 */
void Menu::handleLogo() {
    // Check if interval has been reached
    if (millis() > _logoIntervalMs) {
        screen = Screen::MAIN;
        _redraw = true;
    }
}

/**
 * Menu Handle Main Menu Function
 * 
 * Used to handle the users selection based on the
 * joystick direction. Up / Down to navigate menu
 * options; Right to select; Left does nothing.
 * 
 * @param dir joystick direction
 */
void Menu::handleMainMenu(Direction dir) {
    // Handle user navigating up
    if (dir == Direction::UP) {
        _idx--;
        if (_idx < 0) {
            _idx = MENU_COUNT - 1;
        }
        handleSelectionOverflow();
    }
    // Handle user navigating down
    else if (dir == Direction::DOWN) {
        _idx++;
        if (_idx >= MENU_COUNT) {
            _idx = 0;
            _overflowMainMin = 0;
        }
        handleSelectionOverflow();
    }
    // Handle user making a selection
    else if (dir == Direction::RIGHT) {
        switch(_idx) {
            // 0th index: Count Screen
            case 0:
                screen = Screen::COUNT;
                _redraw = true;
                break;
            // 1st index: Bulk select Screen
            case 1:
                screen = Screen::SELECT;
                break;
            // 2nd index: New bulk select Screen
            case 2:
                screen = Screen::NEW;
                break;
            // 3rd index: Manual Drive Screen
            case 3:
                screen = Screen::MANUAL;
                break;
            // 4th index: Home Motor Screen
            case 4:
                screen = Screen::HOME;
                break;
            // 5th index: Settings Screen
            case 5:
                screen = Screen::SETTINGS;
                break;
        }

        if (screen == Screen::MANUAL) {
            logicHandler.setManual();
            _menuState = MenuState::MANUAL;
        } else if (screen == Screen::HOME) {
            // TODO: handle homing
            logicHandler.setAuto();
            _menuState = MenuState::HOMING;
        } else {
            logicHandler.setAuto();
            _menuState = MenuState::IDLE;
        }
    }
}

/**
 * Menu Handle Count Function
 * 
 * Used to handle the user using the joystick while
 * on the count screen. Only Left has functionality 
 * and will navigate the user back to the main menu.
 * 
 * @param dir joystick direction
 */
void Menu::handleCount(Direction dir) {
    // Navigate user back to Main Menu
    if (dir == Direction::LEFT) {
        screen = Screen::MAIN;
    }
}

/**
 * Menu Handle Selection Function
 * 
 * Used to handle the user using the joystick while
 * on the bulk selection screen. Given the options
 * on the screen, the user can navigate using Up /
 * Down. Right will select the option. Left will 
 * navigate the user back to the main menu.
 * 
 * @param dir joystick direction
 */
void Menu::handleSelection(Direction dir) {
    // Handle user navigating up
    if (dir == Direction::UP) {
        _bulkSelectIndex--;
        // Handle bulk overflow
        if (_bulkSelectIndex < 0) {
            _bulkSelectIndex = _bulkOptionsCount;
        }
        handleSelectionOverflow();
    }
    // Handle user navigating down
    else if (dir == Direction::DOWN) {
        _bulkSelectIndex++;
        // Handle bulk overflow
        if (_bulkSelectIndex >= _bulkOptionsCount + 1) {
            _bulkSelectIndex = 0;
            _overflowBulkMin = 0;
        }
        handleSelectionOverflow();
    } 
    // Handle user making a selection
    else if (dir == Direction::RIGHT) {
        // Handle Continuous Count
        if (_bulkSelectIndex == 0) {
            _continuousCount = true;
        } 
        // Handle Selected Bulk Count
        else {
            _continuousCount = false;
            _bulkLimit = _bulkOptions[_bulkSelectIndex - 1];
            logicHandler.setLimit(_bulkOptions[_bulkSelectIndex - 1]);
        }
        // Change screen to Count Screen
        screen = Screen::COUNT;
    }
    // Handle user returing to main menu
    else if (dir == Direction::LEFT) {
        screen = Screen::MAIN;
    }
}

/**
 * Menu Handle New Count Function
 * 
 * Used to handle the user using the joystick while
 * on the new bulk option screen. Up / Down are used
 * to increment / decrement the user's new option. 
 * Right is used to confirm a valid selection. Left
 * is used to cancel the selection and return the 
 * user to the main menu.
 * 
 * @param dir joystick direction
 */
void Menu::handleNewCount(Direction dir) {
    // Handle the user incrementing option
    if (dir == Direction::UP) {
        _newBulkOption += 10;
    }
    // Handle the user decrementing option
    else if (dir == Direction::DOWN) {
        _newBulkOption -= 10;
        // Prevent negative options
        if (_newBulkOption < 0) {
            _newBulkOption = 0;
        }
    }
    // Handle the user confirming an option
    else if (dir == Direction::RIGHT) {
        // Only accept options greater than 20-packs
        if (_newBulkOption > 20) {
            // Save new options to preferences
            settings.addBulkOption(_newBulkOption, _bulkOptions);
            settings.getBulkOptions(_bulkOptions);
            _bulkOptionsCount = settings.getBulkOptionCount();
        }
        _newBulkOption = 0;
        // Change screen to main menu
        screen = Screen::MAIN;
    }
    // Handle the user cancelling new option
    else if (dir == Direction::LEFT) {
        _newBulkOption = 0;
        screen = Screen::MAIN;
    }
}

void Menu::handleManual(Direction dir) {
    if (dir == Direction::UP) {
        _stopped = false;
        _forwards = true;
        logicHandler.setMotorSpeed(400);
    } else if (dir == Direction::DOWN) {
        _stopped = false;
        _forwards = false;
        logicHandler.setMotorSpeed(-400);
    } else if (dir == Direction::NONE) {
        _stopped = true;
        _forwards = true;
        logicHandler.setMotorSpeed(0);
    } else if (dir == Direction::LEFT) {
        _stopped = true;
        _forwards = true;
        logicHandler.setMotorSpeed(0);
        screen = Screen::MAIN;
    }
}

void Menu::handleHome(Direction dir) {
    if (dir == Direction::RIGHT) {
        // TODO: Handle homing
    } else if (dir == Direction::LEFT) {
        // TODO: stop homing
        screen = Screen::MAIN;
    }
}

/**
 * Menu Handle Settings Function
 * 
 * Used to handle the user using the joystick while on
 * the settings screen. Up / Down is used to navigate
 * the settings options. Right is used to select a 
 * setting the user wants to adjust. Left is used to
 * return to the main menu.
 * 
 * @param dir joystick direction
 */
void Menu::handleSettings(Direction dir) {
    // Handle the user navigating up
    if (dir == Direction::UP) {
        _settingsIndex--;
        // Handle settings index overflow
        if (_settingsIndex < 0) {
            _settingsIndex = SETTING_COUNT - 1;
        }
        handleSelectionOverflow();
    }
    // Handle the user navigating down
    else if (dir == Direction::DOWN) {
        _settingsIndex++;
        // Handle settings index overflow
        if (_settingsIndex >= SETTING_COUNT) {
            _settingsIndex = 0;
            _overflowSettingsMin = 0;
        }
        handleSelectionOverflow();
    }
    // Handle the user selecting a setting to change
    else if (dir == Direction::RIGHT) {
        switch(_settingsIndex) {
            // 0th index: Hold Position Setting
            case 0:
                screen = Screen::HOLD_POS_SETTING;
                break;
            // 1st index: Release Position Setting
            case 1:
                screen = Screen::RELEASE_POS_SETTING;
                break;
            // 2nd index: Hold Time Setting
            case 2:
                screen = Screen::HOLD_TIME_SETTING;
                break;
            // 3rd index: Motor Pulse Width Setting
            case 3:
                screen = Screen::MOTOR_PULSE_WIDTH_SETTING;
                break;
            // 4th index: Motor Max Speed Setting
            case 4:
                screen = Screen::MOTOR_MAX_SPEED_SETTING;
                break;
            // 5th index: Motor Acceleration Setting
            case 5:
                screen = Screen::MOTOR_ACCELERATION_SETTING;
                break;
            // 6th index: Hall Sensor Sensitivity Setting
            case 6:
                screen = Screen::HALL_SENSITIVITY_SETTING;
                break;
            // 7th index: Help Message Cycle Time Setting
            case 7:
                screen = Screen::HELP_SETTING;
                break;
            // 8th index: Factory Reset Setting
            case 8:
                screen = Screen::FACTORY_RESET;
                break;
        }
    }
    // Handle user navigating back to the main menu
    else if (dir == Direction::LEFT) {
        screen = Screen::MAIN;
    }
}

/**
 * Menu Handle Hold Position Setting Function
 * 
 * Used to handle the user using the joystick while
 * on the hold position setting screen. Up / Down is
 * used to increment / decrement the hold position
 * setting to the user's desired position. Right is
 * used to confirm the user's selection. Left is used
 * to navigate back to the settings screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleHoldPosSetting(Direction dir) {
    // Handle the user incrementing the setting
    if (dir == Direction::UP) {
        _holdPosSetting += 5;
    }
    // Handle the user decrementing the setting
    else if (dir == Direction::DOWN) {
        _holdPosSetting -= 5;
        // Prevent user selecting option below
        // the release position
        if (_holdPosSetting < _releasePosSetting) {
            _holdPosSetting = _releasePosSetting + 5;
        }
    }
    // Handle user confirming setting change
    else if (dir == Direction::RIGHT) {
        // Save new setting to preferences
        settings.putHoldPosition(_holdPosSetting);
        logicHandler.setHoldPosition(settings.getHoldPosition());
        // Navigate back to main menu
        screen = Screen::MAIN;
    }
    // Handle user cancelling setting change
    else if (dir == Direction::LEFT) {
        // Reset setting to current saved setting
        _holdPosSetting = settings.getHoldPosition();
        // Return to Settings Screen
        screen = Screen::SETTINGS;
    }
}

/**
 * Menu Handle Release Position Setting Function
 * 
 * Used to handle the user using the joystick while
 * on the release position setting screen. Up / Down
 * is used to increment / decrement the hold position
 * setting to the user's desired position. Right is
 * used to confirm the user's selection. Left is used
 * to navigate back to the settings screen.
 * 
 * @param dir joystick direction
 */
void Menu::handleReleasePosSetting(Direction dir) {
    // Handle the user incrementing the setting
    if (dir == Direction::UP) {
        _releasePosSetting += 5;
        // Prevent the user from incrementing above 
        // the hold setting
        if (_releasePosSetting >= _holdPosSetting) {
            _releasePosSetting = _holdPosSetting - 5;
        }
    }
    // Handle the user decrementing the setting
    else if (dir == Direction::DOWN) {
        _releasePosSetting -= 5;
        // Prevent the user from decrementing below 0
        if (_releasePosSetting < 0) {
            _releasePosSetting = 0;
        }
    } else if (dir == Direction::RIGHT) {
        settings.putReleasePosition(_releasePosSetting);
        logicHandler.setReleasePosition(_releasePosSetting);
        screen = Screen::MAIN;
    } else if (dir == Direction::LEFT) {
        _releasePosSetting = settings.getReleasePosition();
        screen = Screen::SETTINGS;
    }
}

void Menu::handleHoldTimeSetting(Direction dir) {
    if (dir == Direction::UP) {
        _holdTimeSetting += 100;
    } else if (dir == Direction::DOWN) {
        _holdTimeSetting -= 100;
        if (_holdTimeSetting < 1000) {
            _holdTimeSetting = 1000;
        }
    } else if (dir == Direction::RIGHT) {
        settings.putHoldTime(_holdTimeSetting);
        logicHandler.setHoldTime(settings.getHoldTime());
        screen = Screen::MAIN;
    } else if (dir == Direction::LEFT) {
        _holdTimeSetting = settings.getHoldTime();
        screen = Screen::SETTINGS;
    }
}

void Menu::handleMotorPulseWidthSetting(Direction dir) {
    if (dir == Direction::UP) {
        _pulseWidthSetting += 1;
        if (_pulseWidthSetting >= 20) {
            _pulseWidthSetting = 20;
        }
    } else if (dir == Direction::DOWN) {
        _pulseWidthSetting -= 1;
        if (_pulseWidthSetting < 0) {
            _pulseWidthSetting = 0;
        }
    } else if (dir == Direction::RIGHT) {
        settings.putMotorPulseWidth(_pulseWidthSetting);
        logicHandler.setMotorPulseWidth(_pulseWidthSetting);
        screen = Screen::MAIN;
    } else if (dir == Direction::LEFT) {
        _pulseWidthSetting = settings.getMotorPulseWidth();
        screen = Screen::SETTINGS;
    }
}

void Menu::handleMotorMaxSpeedSetting(Direction dir) {
    if (dir == Direction::UP) {
        _motorSpeed += 10;
        if (_motorSpeed >= 800) {
            _motorSpeed = 800;
        }
    } else if (dir == Direction::DOWN) {
        _motorSpeed -= 10;
        if (_motorSpeed <= -800) {
            _motorSpeed = -800;
        }
    } else if (dir == Direction::RIGHT) {
        settings.putMotorMaxSpeed(_motorSpeed);
        logicHandler.setMotorMaxSpeed(_motorSpeed);
        screen = Screen::MAIN;
    } else if (dir == Direction::LEFT) {
        _motorSpeed = settings.getMotorMaxSpeed();
        screen = Screen::SETTINGS;
    }
}

void Menu::handleMotorAccelerationSetting(Direction dir) {
    if (dir == Direction::UP) {
        _motorAcceleration += 10;
        if (_motorAcceleration >= 200) {
            _motorAcceleration = 200;
        }
    } else if (dir == Direction::DOWN) {
        _motorAcceleration -= 10;
        if (_motorAcceleration <= 10) {
            _motorAcceleration = 10;
        }
    } else if (dir == Direction::RIGHT) {
        settings.putMotorAcceleration(_motorAcceleration);
        logicHandler.setMotorAcceleration(_motorAcceleration);
        screen = Screen::MAIN;
    } else if (dir == Direction::LEFT) {
        _motorAcceleration = settings.getMotorAcceleration();
        screen = Screen::SETTINGS;
    }
}

void Menu::handleHallSensitivitySetting(Direction dir) {
    if (dir == Direction::UP) {
        _homingSensitivity += 10;
        if (_homingSensitivity >= 2000) {
            _homingSensitivity = 2000;
        }
    } else if (dir == Direction::DOWN) {
        _homingSensitivity -= 10;
        if (_homingSensitivity <= 100) {
            _homingSensitivity = 100;
        }
    } else if (dir == Direction::RIGHT) {
        settings.putHallSensitivity(_homingSensitivity);
        logicHandler.setHallSensitivity(_homingSensitivity);
        screen = Screen::MAIN;
    } else if (dir == Direction::LEFT) {
        _homingSensitivity = settings.getHallSensitivity();
        screen = Screen::SETTINGS;
    }
}

void Menu::handleHelpSetting(Direction dir) {
    if (dir == Direction::UP) {
        _helpTimeSetting += 10;
        if (_helpTimeSetting >= 2000) {
            _helpTimeSetting = 2000;
        }
    } else if (dir == Direction::DOWN) {
        _helpTimeSetting -= 10;
        if (_helpTimeSetting <= 100) {
            _helpTimeSetting = 100;
        }
    } else if (dir == Direction::RIGHT) {
        settings.putHelpTime(_helpTimeSetting);
        _helpTime = _helpTimeSetting;
        screen = Screen::MAIN;
    } else if (dir == Direction:: LEFT) {
        _helpTimeSetting = settings.getHelpTime();
        screen = Screen::SETTINGS;
    }
}

// void Menu::handleHelpSettings(Direction dir) {
//     if (dir == Direction::UP) {
//         _helpTimeSetting += 100;
//     } else if (dir == Direction::DOWN) {
//         _helpTimeSetting -= 100;
//         if (_helpTimeSetting < 1000) {
//             _helpTimeSetting = 1000;
//         }
//     } else if (dir == Direction::RIGHT) {
//         settings.putHelpTime(_helpTime);
//         _helpTime = settings.getHelpTime();
//         _helpResetFloor = convertMsToResetFloor(_helpTime);
//         screen = Screen::MAIN;
//     } else if (dir == Direction::LEFT) {
//         _helpTimeSetting = _helpTime;
//         screen = Screen::SETTINGS;
//     }
// }

// void Menu::handleJoystickSettings(Direction dir) {
//     // todo: implement joystick settings control
//     if (dir == Direction::LEFT) {
//         screen = Screen::SETTINGS;
//     }
// }

void Menu::draw() {
    auto& u8g2 = display.get();
    u8g2.clearBuffer();

    switch (screen) {
        case Screen::LOGO:
            drawLogoScreen(u8g2);
            break;
        case Screen::MAIN:
            drawMainMenuScreen(u8g2);
            break;
        case Screen::COUNT:
            drawCountScreen(u8g2);
            break;
        case Screen::SELECT:
            drawSelectionScreen(u8g2);
            break;
        case Screen::NEW:
            drawNewCountScreen(u8g2);
            break;
        case Screen::MANUAL:
            drawManualScreen(u8g2);
            break;
        case Screen::HOME:
            drawHomeScreen(u8g2);
            break;
        case Screen::SETTINGS:
            drawSettingsScreen(u8g2);
            break;
        case Screen::HOLD_SETTING:
            drawHoldSettingsScreen(u8g2);
            break;
        case Screen::HELP_SETTING:
            drawHelpSettingsScreen(u8g2);
            break;
        case Screen::JOYSTICK_SETTING:
            drawJoystickSettingsScreen(u8g2);
            break;
    }

    u8g2.sendBuffer();
}

void Menu::drawLogoScreen(U8G2& u8g2) {
    u8g2.drawXBMP(0, 0, 128, 64, epd_bitmap_podpac_logo);
}

void Menu::drawMainMenuScreen(U8G2& u8g2) {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Main Menu");
    int limit = MAX_DISPLAYED_BULK_SELECT + _overflowMainMin;

    // for (int i = 0; i < MENU_COUNT; i++) {
    for (int i = _overflowMainMin; i < limit; i++) {
        // int y = 22 + i * 10;
        int y = 22 + (i - _overflowMainMin) * 10;
        if (i == _idx) {
            u8g2.drawStr(2, y, ">");
            u8g2.drawFrame(10, y - 8, 110, 10);
            u8g2.drawStr(14, y, _menuItems[i]);
        } else {
            u8g2.drawStr(14, y, _menuItems[i]);
        }
    }

    drawHelpMessage(u8g2);
}

void Menu::drawCountScreen(U8G2& u8g2) {
    char buffer[16];
    if (_continuousCount) {
        snprintf(buffer, sizeof(buffer), "%lu", (unsigned long) _lastDisplayedCount);
    } else {
        snprintf(buffer, sizeof(buffer), "%lu / %d", (unsigned long) _lastDisplayedCount, _bulkLimit);
    }

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Capsule Count:");

    u8g2.setFont(u8g2_font_logisoso20_tf);
    int x = (128 - u8g2.getStrWidth(buffer)) / 2;
    u8g2.drawStr(x, 42, buffer);

    u8g2.setFont(u8g2_font_6x10_tf);
    drawHelpMessage(u8g2);
}

void Menu::drawSelectionScreen(U8G2& u8g2) {
    // TODO: check that the lines below commented lines are correct
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Bulk Selection:");
    int limit = _bulkOptionsCount + 1; // Add 1 for Continuous Count
    if (_bulkOptionsCount >= MAX_DISPLAYED_BULK_SELECT) {
        // limit = MAX_DISPLAYED_BULK_SELECT + _overflowMin;
        limit = MAX_DISPLAYED_BULK_SELECT + _overflowBulkMin;
    }

    // for (int i = _overflowMin; i < limit; i++) {
    for (int i = _overflowBulkMin; i < limit; i++) {
        char buffer[20];
        if (i == 0) {
            snprintf(buffer, sizeof(buffer), "Continuous Count");
        } else {
            snprintf(buffer, sizeof(buffer), "%d", _bulkOptions[i - 1]);
        }
        // int y = 22 + (i - _overflowMin) * 10;
        int y = 22 + (i - _overflowBulkMin) * 10;
        if (i == _bulkSelectIndex) {
            u8g2.drawStr(2, y, ">");
            u8g2.drawFrame(10, y - 8, 110, 10);
            u8g2.drawStr(14, y, buffer);
        } else {
            u8g2.drawStr(14, y, buffer);
        }
    }

    drawHelpMessage(u8g2);
}

void Menu::drawNewCountScreen(U8G2& u8g2) {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "New Bulk Option:");
    
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d", _newBulkOption);

    u8g2.setFont(u8g2_font_logisoso20_tf);
    int x = (128 - u8g2.getStrWidth(buffer)) / 2;
    u8g2.drawStr(x, 42, buffer);

    u8g2.setFont(u8g2_font_6x10_tf);
    drawHelpMessage(u8g2);
}

void Menu::drawManualScreen(U8G2& u8g2) {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Manual Drive:");

    u8g2.setFont(u8g2_font_logisoso16_tf);
    if (_stopped) {
        u8g2.drawStr(10, 42, "Stopped");
    } else if (_forwards) {
        u8g2.drawStr(10, 42, "Forwards");
    } else {
        u8g2.drawStr(10, 42, "Backwards");
    }
    
    // TODO: reimplement help message
    // u8g2.setFont(u8g2_font_6x10_tf);
    // drawHelpMessage(u8g2);
}

void Menu::drawHomeScreen(U8G2& u8g2) {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Home Motor:");

    // TODO: add homing message
    
    u8g2.setFont(u8g2_font_6x10_tf);
    drawHelpMessage(u8g2);
}

void Menu::drawSettingsScreen(U8G2& u8g2) {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Settings");

    for (int i = 0; i < SETTING_COUNT; i++) {
        int y = 22 + i * 10;
        if (i == _settingsIndex) {
            u8g2.drawStr(2, y, ">");
            u8g2.drawFrame(10, y - 8, 110, 10);
            u8g2.drawStr(14, y, _settingsItems[i]);
        } else {
            u8g2.drawStr(14, y, _settingsItems[i]);
        }
    }

    drawHelpMessage(u8g2);
}

void Menu::drawHoldSettingsScreen(U8G2& u8g2) {
    char buffer[16];
    double holdTimeFloat = double(_holdTimeSetting) / 1000;
    snprintf(buffer, sizeof(buffer), "%0.1fs", holdTimeFloat);

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Capsule Hold Time (s)");

    u8g2.setFont(u8g2_font_logisoso20_tf);
    int x = (128 - u8g2.getStrWidth(buffer)) / 2;
    u8g2.drawStr(x, 42, buffer);

    u8g2.setFont(u8g2_font_6x10_tf);
    drawHelpMessage(u8g2);
}

void Menu::drawHelpSettingsScreen(U8G2& u8g2) {
    char buffer[16];
    double helpTimeFloat = double(_helpTimeSetting) / 1000;
    snprintf(buffer, sizeof(buffer), "%0.1fs", helpTimeFloat);

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Help Message Time (s)");

    u8g2.setFont(u8g2_font_logisoso20_tf);
    int x = (128 - u8g2.getStrWidth(buffer)) / 2;
    u8g2.drawStr(x, 42, buffer);

    u8g2.setFont(u8g2_font_6x10_tf);
    drawHelpMessage(u8g2);
}

void Menu::drawJoystickSettingsScreen(U8G2& u8g2) {}

void Menu::drawHelpMessage(U8G2& u8g2) {
    int sIdx = getScreenIndex(screen);
    int hIdx = floor(_helpIndex/_helpResetFloor);
    u8g2.drawStr(0, 62, _helpMessages[sIdx][hIdx]);
}

int Menu::getScreenIndex(Screen s) {
    switch(s) {
        case Screen::LOGO:
            return 0;
        case Screen::MAIN:
            return 1;
        case Screen::COUNT:
            return 2;
        case Screen::SELECT:
            return 3;
        case Screen::NEW:
            return 4;
        case Screen::SETTINGS:
            return 5;
        case Screen::HOLD_SETTING:
            return 6;
        case Screen::HELP_SETTING:
            return 7;
        case Screen::JOYSTICK_SETTING:
            return 8;
        default:
            return -1;
    }
}

int Menu::convertMsToResetFloor(int time) { return 5 * time / 1000; }

int Menu::getContinuousCountCallback() { return _lastDisplayedCount; }