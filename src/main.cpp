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



int i = 10;



void setup(void)
{
  
  Serial.begin(115200); delay(500);
  
  // Init the NMEA2000
  initN2K();

  // Init the display
  initDisplay();



  delay(3000);

}


void loop()
{

  uint32_t dt = millis();

  if (i > 100)
  {
    background.pushImage(0, 0, 240, 240, _scale_2);
  }
  else
  {
    background.pushImage(0, 0, 240, 240, _scale_1);
  }

  showCoolantTemperature(i);
  showEngineSpeed(i * 29);
  showNeedle(i * 30);
  showEngineHours(i * 23.1);
  // showText(i * 21);

  background.pushSprite(0, 0);

  //delay(1);

  i = i + 2;
  if (i > 130)
  {
    i = 10;
  }
  // delay(10);
  updateN2K();
}

