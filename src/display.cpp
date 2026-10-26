#include "display.h"

/**
 * Dislay Initialiser
 * 
 * Sets up the U8G2 library using the predefined pins from
 * the display header file.
 */
Display::Display() : u8g2(U8G2_R0, CLOCK_PIN, DATA_PIN, CS_PIN) {}

/**
 * Display Begin Function
 * 
 * Used in main.cpp's setup function, this function is used
 * for initialising the U8G2 object.
 */
void Display::begin() { u8g2.begin(); }

/**
 * Display Get Function
 * 
 * Used for retrieving a reference to the U8G2 object.
 * 
 * @return U8G2 reference
 */
U8G2_ST7920_128X64_F_SW_SPI& Display::get() { return u8g2; }