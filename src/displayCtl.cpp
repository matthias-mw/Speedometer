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

#include <displayCtl.h>

//TwoWire I2CWire = TwoWire(0);

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ********************************************************************
// Constructor
DisplayCtl::DisplayCtl(Adafruit_SSD1306 *dispGlobal) {
  Serial.println(F("Error:"));

  // Initialize with the I2C addr 0x3D (for the 128x64)
  this->display = dispGlobal; 

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!this->display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;){
      delay(1000);
      Serial.println(F("Error:"));
    }; // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  this->display->display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  this->display->clearDisplay();
}

// ********************************************************************
// Destructor 
DisplayCtl::~DisplayCtl() {
  // // Clear the buffer
  // display.clearDisplay();
  // display.display();
}

// ********************************************************************
// Draw a line
void DisplayCtl::drawline() {
  int16_t i;
  Serial.println(F("Error2:"));
  // display.clearDisplay(); // Clear display buffer

  // for(i=0; i<display.width(); i+=4) {
  //   display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
  //   display.display(); // Update screen with each newly-drawn line
  //   delay(1);
  // }
  // for(i=0; i<display.height(); i+=4) {
  //   display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
  //   display.display();
  //   delay(1);
  // }
 
}
// ********************************************************************
// Display engine speed
void DisplayCtl::displayEngineSpeed(uint16_t speed) {
  Serial.println(F("Error:3"));
  this->display->clearDisplay(); // Clear display buffer
  this->display->setTextSize(3);      // Normal 1:1 pixel scale
  this->display->setTextColor(SSD1306_WHITE); // Draw white text
  this->display->setCursor(0,0);     // Start at top-left corner
  this->display->println(speed);
  this->display->display();
}

