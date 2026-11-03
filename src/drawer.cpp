#include "drawer.h"

// Display disp;
// IndexTracker i;

/**
 * Drawer Initialiser
 * 
 * Function used to initialise the variables.
 */
Drawer::Drawer() :
    display(),
    screen(Screen::LOGO),
    motorState(MotorState::STOPPED),
    idxTracker(),
    _continuousCount(true),
    _bulkLimit(60),
    _newBulkOption(30),
    _lastDisplayedCount(0),
    _helpResetFloor(0) {}

/**
 * Drawer Begin Function
 * 
 * Function used to set up the class objects.
 * 
 * @param sett preferences manager object
 */
void Drawer::begin(Settings& sett) {
    display.begin();
    settings = sett;
    _helpResetFloor = 5 * settings.getHelpTime() / 1000;
    _bulkOptionsCount = settings.getBulkOptionCount();
    settings.getBulkOptions(_bulkOptions);
    Serial.println("[Drawer] Setup Complete");
}

/**
 * Drawer Get Current Screen Function
 * 
 * Function used to get the current screen.
 */
Screen Drawer::getCurrentScreen() { return screen; }

/**
 * Drawer Set Current Screen Function
 * 
 * Function used to set the current screen.
 */
void Drawer::setCurrentScreen(Screen s) { screen = s; }

/**
 * Drawer Get Screen Index
 * 
 * Function used to get the index of the enumerated screen.
 * 
 * @return index of screen
 */
int Drawer::getScreenIndex(Screen s) {
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
        case Screen::MANUAL:
            return 5;
        case Screen::HOME:
            return 6;
        case Screen::SETTINGS:
            return 7;
        case Screen::HOLD_POS_SETTING:
            return 8;
        case Screen::RELEASE_POS_SETTING:
            return 9;
        case Screen::HOLD_TIME_SETTING:
            return 10;
        case Screen::MOTOR_PULSE_WIDTH_SETTING:
            return 11;
        case Screen::MOTOR_MAX_SPEED_SETTING:
            return 12;
        case Screen::MOTOR_ACCELERATION_SETTING:
            return 13;
        case Screen::HALL_SENSITIVITY_SETTING:
            return 14;
        case Screen::HELP_SETTING:
            return 15;
        case Screen::FACTORY_RESET:
            return 16;
        default:
            return -1;
    }
}

/**
 * Drawer Get Motor State Function
 * 
 * Function used to get the current state of the motor.
 * 
 * @return current motor state
 */
MotorState Drawer::getMotorState() { return motorState; }

/**
 * Drawer Set Motor State Function
 * 
 * Function used to set the state of the motor.
 * 
 * @param state new state of the motor
 */
void Drawer::setMotorState(MotorState state) { motorState = state; }

/**
 * Drawer Enable Contiuous Count Function
 * 
 * Function used to enable the continuous counter
 */
void Drawer::enableContinuousCount() { _continuousCount = true; }

/**
 * Drawer Disable Contiuous Count Function
 * 
 * Function used to disable the continuous counter
 */
void Drawer::disableContinuousCount() { _continuousCount = false; }

/**
 * Drawer Get Bulk Options Count Function
 * 
 * Function for getting the bulk options count.
 * 
 * @return number of bulk options available
 */
size_t Drawer::getBulkOptionsCount() { return _bulkOptionsCount; }

/**
 * Drawer Set Bulk Options Count Function
 * 
 * Function for setting the bulk options count.
 * 
 * @param count number of available bulk options
 */
void Drawer::setBulkOptionsCount(size_t count) { _bulkOptionsCount = count; }

/**
 * Drawer Set Bulk Limit Function
 * 
 * Function used to set the bulk counting limit.
 */
void Drawer::setBulkLimit() { _bulkLimit = _bulkOptions[idxTracker.getBulkIndex() - 1]; }

/**
 * Drawer Get New Bulk Option Function
 * 
 * Function for getting the new bulk option.
 * 
 * @return new bulk option
 */
int Drawer::getNewBulkOption() { return _newBulkOption; }

/**
 * Drawer Set New Bulk Option Function
 * 
 * Function for setting the new bulk option.
 * 
 * @param option new bulk option
 */
void Drawer::setNewBulkOption(int option) { _newBulkOption = option; }

/**
 * Drawer Increment New Bulk Option Function
 * 
 * Function for adding 10 to the new bulk option.
 */
void Drawer::incrementNewBulkOption() { _newBulkOption += 10; }

/**
 * Drawer Decrement New Bulk Option Function
 * 
 * Function for removing 10 from the new bulk option.
 */
void Drawer::decrementNewBulktOption() { _newBulkOption -= 10; }

/**
 * Drawer Get Last Displayed Count Function
 * 
 * Function for getting the last displayed capsule count.
 * 
 * @return last displayed count
 */
int Drawer::getLastDisplayedCount() { return _lastDisplayedCount; }

/**
 * Drawer Set Last Displayed Count Function
 * 
 * Function for updating the displayed capsule count.
 * 
 * @param count new count to be displayed
 */
void Drawer::setLastDisplayedCount(int count) { _lastDisplayedCount = count; }

/**
 * Drawer Help Message Condition Function
 * 
 * Function used to check conditions for printing help message.
 * 
 * @return true if conditions met
 */
bool Drawer::helpMessageCondition() {
    // Get Indices
    int sIdx = getScreenIndex(screen);
    int hIdx = floor(idxTracker.getHelpIndex() / _helpResetFloor);
    return hIdx > HELP_MESSAGE_COUNT || _helpMessages[sIdx][hIdx] == "";
}

/**
 * Drawer Draw Logo Screen
 * 
 * Function used to draw the Podpac Logo.
 */
void Drawer::drawLogoScreen(U8G2& u8g2) { u8g2.drawXBMP(0, 0, 128, 64, epd_bitmap_podpac_logo); }

/**
 * Drawer Draw Main Menu Screen
 * 
 * Function used to draw the main menu.
 */
void Drawer::drawMainMenuScreen(U8G2& u8g2) {
    // Draw Header Line
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Main Menu");

    // Handle Overflow
    int limit = MAX_DISPLAYED_BULK_SELECT + idxTracker.getMainOverflow();

    // Draw Menu Options
    for (int i = idxTracker.getMainOverflow(); i < limit; i++) {
        int y = 22 + (i - idxTracker.getMainOverflow()) * 10;
        if (i == idxTracker.getMainIndex()) {
            u8g2.drawStr(2, y, ">");
            u8g2.drawFrame(10, y - 8, 110, 10);
            u8g2.drawStr(14, y, _menuItems[i]);
        } else {
            u8g2.drawStr(14, y, _menuItems[i]);
        }
    }

    // Draw Help Message
    drawHelpMessage(u8g2);
}

/**
 * Drawer Draw Count Screen Function
 * 
 * Function used to draw the count screen.
 */
void Drawer::drawCountScreen(U8G2& u8g2) {
    // Draw Header Line
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Capsule Count:");

    // Draw Counter
    char buffer[16];
    if (_continuousCount) snprintf(buffer, sizeof(buffer), "%lu", (unsigned long) _lastDisplayedCount);
    else snprintf(buffer, sizeof(buffer), "%lu / %d", (unsigned long) _lastDisplayedCount, _bulkLimit);
    u8g2.setFont(u8g2_font_logisoso20_tf);
    int x = (128 - u8g2.getStrWidth(buffer)) / 2;
    u8g2.drawStr(x, 42, buffer);

    // Draw Help Message
    drawHelpMessage(u8g2);
}

/**
 * Drawer Draw Selection Screen Function
 * 
 * Function used to draw the bulk selection screen.
 */
void Drawer::drawSelectionScreen(U8G2& u8g2) {
    // Draw Header Line
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Bulk Selection:");

    // Handle Overflow
    int limit = _bulkOptionsCount + 1;  // Add 1 for continuous count
    if (_bulkOptionsCount >= MAX_DISPLAYED_BULK_SELECT) limit = MAX_DISPLAYED_BULK_SELECT + idxTracker.getBulkOverflow();

    // Draw Bulk Options
    for (int i = idxTracker.getBulkOverflow(); i < limit; i++) {
        char buffer[20];
        if (i == 0) snprintf(buffer, sizeof(buffer), "Continuous Count");
        else snprintf(buffer, sizeof(buffer), "%d", _bulkOptions[i - 1]);

        int y = 22 + (i - idxTracker.getBulkOverflow()) * 10;
        if (i == idxTracker.getBulkIndex()) {
            u8g2.drawStr(2, y, ">");
            u8g2.drawFrame(10, y - 8, 110, 10);
            u8g2.drawStr(14, y, buffer);
        } else u8g2.drawStr(14, y, buffer);
    }

    // Draw Help Message
    drawHelpMessage(u8g2);
}

/**
 * Drawer Draw New Count Screen Function
 * 
 * Function used to draw the new count screen.
 */
void Drawer::drawNewCountScreen(U8G2& u8g2) {
    // Draw Header Line
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "New Bulk Option:");

    // Draw New Bulk Option
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d", _newBulkOption);
    u8g2.setFont(u8g2_font_logisoso20_tf);
    int x = (128 - u8g2.getStrWidth(buffer)) / 2;
    u8g2.drawStr(x, 42, buffer);

    // Draw Help Message
    drawHelpMessage(u8g2);
}

/**
 * Drawer Draw Manual Screen Function
 * 
 * Function used to draw the manual motor drive screen.
 */
void Drawer::drawManualScreen(U8G2& u8g2) {
    // Draw Header Line
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, "Manual Drive:");

    // Draw Current Direction of Drive
    u8g2.setFont(u8g2_font_logisoso16_tf);
    if (motorState == MotorState::STOPPED) {
        u8g2.drawStr(10, 42, "Stopped");
    } else if (motorState == MotorState::FORWARDS) {
        u8g2.drawStr(10, 42, "Forwards");
    } else if (motorState == MotorState::BACKWARDS) {
        u8g2.drawStr(10, 42, "Backwards");
    }
    
    // Draw Help Message
    drawHelpMessage(u8g2);
}

/**
 * Drawer Draw Home Screen Function
 * 
 * Function used to draw the homing motor screen.
 */
void Drawer::drawHomeScreen(U8G2& u8g2) {}

/**
 * Drawer Draw Settings Screen Function
 * 
 * Function used to draw the settings screen.
 */
void Drawer::drawSettingsScreen(U8G2& u8g2) {}

/**
 * Drawer Draw Hold Position Setting Screen Function
 * 
 * Function used to draw the hold position setting screen.
 */
void Drawer::drawHoldPosSettingScreen(U8G2& u8g2) {}

/**
 * Drawer Draw Release Position Setting Screen Function
 * 
 * Function used to draw the release position setting screen.
 */
void Drawer::drawReleasePosSettingScreen(U8G2& u8g2) {}

/**
 * Drawer Draw Hold Time Setting Screen Function
 * 
 * Function used to draw the hold time settting screen.
 */
void Drawer::drawHoldTimeSettingScreen(U8G2& u8g2) {}

/**
 * Drawer Draw Motor Pulse Width Setting Screen Function
 * 
 * Function used to draw the motor pulse width setting screen.
 */
void Drawer::drawMotorPulseWidthSettingScreen(U8G2& u8g2) {}

/**
 * Drawer Draw Motor Maximum Speed Setting Screen Function
 * 
 * Function used to draw the motor maximum speed setting screen.
 */
void Drawer::drawMotorMaxSpeedSettingScreen(U8G2& u8g2) {}

/**
 * Drawer Draw Motor Acceleration Setting Screen Function
 * 
 * Function used to draw the motor acceleration setting screen.
 */
void Drawer::drawMotorAccelerationSettingScreen(U8G2& u8g2) {}

/**
 * Drawer Draw Hall Sensitivity Setting Screen Function
 * 
 * Function used to draw the hall sensitivity setting screen.
 */
void Drawer::drawHallSensitivitySettingScreen(U8G2& u8g2) {}

/**
 * Drawer Draw Help Message Settings Screen Function
 * 
 * Function used to draw the help settings screen.
 */
void Drawer::drawHelpSettingsScreen(U8G2& u8g2) {}

/**
 * Drawer Draw Factory Reset Screen Function
 * 
 * Function used to draw the factory reset screen.
 */
void Drawer::drawFactoryReset(U8G2& u8g2) {}

/**
 * Drawer Draw Help Message Function
 * 
 * Function used to draw the rolling help message at the bottom
 * of the various screens.
 */
void Drawer::drawHelpMessage(U8G2& u8g2) {
    int sIdx = getScreenIndex(screen);
    int hIdx = floor(idxTracker.getHelpIndex() / _helpResetFloor);
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 62, _helpMessages[sIdx][hIdx]);
}

/**
 * Drawer Draw Function
 * 
 * Function used to build the screen layout based on
 * current screen.
 */
void Drawer::draw() {
    // Clear Screen
    auto& u8g2 = display.get();
    u8g2.clearBuffer();

    // Redraw Screen
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
        case Screen::HOLD_POS_SETTING:
            drawHoldPosSettingScreen(u8g2);
            break;
        case Screen::RELEASE_POS_SETTING:
            drawReleasePosSettingScreen(u8g2);
            break;
        case Screen::HOLD_TIME_SETTING:
            drawHoldTimeSettingScreen(u8g2);
            break;
        case Screen::MOTOR_PULSE_WIDTH_SETTING:
            drawMotorPulseWidthSettingScreen(u8g2);
            break;
        case Screen::MOTOR_MAX_SPEED_SETTING:
            drawMotorMaxSpeedSettingScreen(u8g2);
            break;
        case Screen::MOTOR_ACCELERATION_SETTING:
            drawMotorAccelerationSettingScreen(u8g2);
            break;
        case Screen::HALL_SENSITIVITY_SETTING:
            drawHallSensitivitySettingScreen(u8g2);
            break;
        case Screen::HELP_SETTING:
            drawHelpSettingsScreen(u8g2);
            break;
        case Screen::FACTORY_RESET:
            drawFactoryReset(u8g2);
            break;
    }

    // Send Data do Dot Matrix
    u8g2.sendBuffer();
}