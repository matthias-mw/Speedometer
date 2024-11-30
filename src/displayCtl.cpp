/*!
 * \file displayCtl.cpp
 * \brief Display Control
 *
 * This file contains the functions for the display control of the OLED display
 * It provides different pages for displaying all signals of the speedometer.
 *
 * \author Matthias Werner
 * \date   November  2024
 * \version 0.1
 *
 *
 */

#include "displayCtl.h"

//TwoWire I2CWire = TwoWire(0);

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ********************************************************************
// Constructor
DisplayCtl::DisplayCtl(Adafruit_GC9A01A *dispGlobal) {
    Serial.println(F("Error:"));

    // Initialize with the Display object
    this->display = dispGlobal; 
    // Initialize the display
    this->display->begin();

    // Ensure display is properly initialized
    if (!this->display) {
        Serial.println(F("Display initialization failed"));
        for (;;) {
            delay(1000);
        }
    }
}

// ********************************************************************
// Destructor 
DisplayCtl::~DisplayCtl() {
    // Clear the buffer
    // display.clearDisplay();
    // display.display();
}

// ********************************************************************
// Display engine speed
void DisplayCtl::displayEngineSpeed(uint16_t speed) {
    Serial.println(F("Error:3"));
    //this->display->clearDisplay(); // Clear display buffer
    // this->display->setTextSize(3);      // Normal 1:1 pixel scale
    // this->display->setTextColor(SSD1306_WHITE); // Draw white text
    // this->display->setCursor(0,0);     // Start at top-left corner
    // this->display->println(speed);
    // this->display->display();
    this->display->fillScreen(GC9A01A_GREEN);
    unsigned long start = micros();
    this->display->setCursor(120, 120);
    this->display->setTextColor(GC9A01A_WHITE);  
    this->display->setTextSize(3);
    this->display->println("Hello World!");
}

