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
    idxTracker() {}

/**
 * Drawer Begin Function
 * 
 * Function used to set up the class objects.
 */
void Drawer::begin() {
    display.begin();
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

    drawHelpMessage(u8g2);
}

/**
 * Drawer Draw Count Screen Function
 * 
 * Function used to draw the count screen.
 */
void Drawer::drawCountScreen(U8G2& u8g2) {}

/**
 * Drawer Draw Selection Screen Function
 * 
 * Function used to draw the bulk selection screen.
 */
void Drawer::drawSelectionScreen(U8G2& u8g2) {}

/**
 * Drawer Draw New Count Screen Function
 * 
 * Function used to draw the new count screen.
 */
void Drawer::drawNewCountScreen(U8G2& u8g2) {}

/**
 * Drawer Draw Manual Screen Function
 * 
 * Function used to draw the manual motor drive screen.
 */
void Drawer::drawManualScreen(U8G2& u8g2) {}

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
void Drawer::drawHelpMessage(U8G2& u8g2) {}

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