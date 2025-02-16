/*!
 * \file main.cpp
 * \brief Main File fot the Speedometer Project
 * 
 * This file contains the main loop for the Speedometer project.
 * 
 * \author Matthias Werner
 * \date   January 2025
 * \version 0.1
 *  * 
 */

 #include <hardwareDef.h>
#include <Arduino.h>
#include "displayCtl.h"
#include <process_n2k.h>

// Setup the the software
void setup(void)
{
  
  Serial.begin(115200); delay(500);
  
  // Init the NMEA2000
  initN2K();

  // Init the display
  initDisplay();

  // Initial Display Delay for Startscreen
  delay(3000);

}


void loop()
{

  // Update the NMEA2000 messages
  updateN2K();

  // Show the actual values on the screen
  updateDisplay(DisplayData.EngineSpeed, DisplayData.EngineCoolantTemperature, DisplayData.EngineHours,0);
  

  delay(20);
  
}

