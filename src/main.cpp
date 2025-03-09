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


/*!
 * \brief Calculate the display brightness
 *
 * This function will calculate the display brightness based on the
 * analog value of the brightness sensor.
 *
 * \return uint8_t brightness setpoint for Display
 */
uint8_t calcDisplayBrightness();

/*!
 * \brief Set the display brightness
 *
 * This function will calculate the display brightness based on the
 * analog value of the brightness sensor.
 *
 */
void setDisplayBrightness();

// Setup the the software
void setup(void)
{

  Serial.begin(115200);
  delay(500);

  
  // Init the display
  initDisplay();

  // Initial Display Delay for Startscreen
  delay(5000);

  //set the resolution to 12 bits (0-4096)
  analogReadResolution(12);

  pinMode(TFT_BL, OUTPUT);

  pinMode(BRIGHTNESS_PIN, INPUT);

  // Init the NMEA2000
  initN2K();

}

// *****************************************************************************
// Main Loop
// *****************************************************************************
void loop()
{
  // Update the NMEA2000 messages
  //updateN2K();

  // Set the display brightness
  setDisplayBrightness();

  // Show the actual values on the screen
  // updateDisplay(DisplayData.EngineSpeed, DisplayData.EngineCoolantTemperature, DisplayData.EngineHours, DisplayData.LowOilPressureWarning);
  
  updateDisplay(analogRead(BRIGHTNESS_PIN), 75, calcDisplayBrightness(), 0);

  delay(250);

  Serial.println("Loop");
  updateN2K();
}

// *****************************************************************************
// Calculate the display brightness
// *****************************************************************************
uint8_t calcDisplayBrightness()
{

  uint8_t brightness = 0;
  uint8_t cnt = 0;
  uint32_t value = 0;


  // Read the brightness value 8 times and calculate the average
  while (cnt < 8)
  {
    value += analogRead(BRIGHTNESS_PIN);
    cnt++;
  }
  value = value >> 3;

  // Adjust the brightness
  if (value > ANALOG_VALUE_NIGHT)
  {
    //Night, it is dark
    brightness = BRIGHTNESS_OUTPUT_NIGHT;
  }
  else if (value > ANALOG_VALUE_DAYLIGHT)
  {
    //Normal Daylight grey sky
    brightness = BRIGHTNESS_OUTPUT_DAYLIGHT;
  }
  else 
  {
    //Now its bright Daylight
    brightness = BRIGHTNESS_OUTPUT_MAX_DAYLIGHT;
  }

  // Only if Debug is enabled
#if DEBUG_LEVEL == 1

  Serial.print(millis());
  Serial.print(": Brightness Sensor Analog Value: ");
  Serial.print(value);
  Serial.print(" -- > Brightness: ");
  Serial.println(brightness);
#endif

  // return the brightness value
  return brightness;

}

// *****************************************************************************
// Set Display Brightness
// *****************************************************************************
void setDisplayBrightness()
{

  uint8_t brightness = 0;

  // Calculate the brightness
  brightness = calcDisplayBrightness();

  // Set the brightness
  analogWrite(TFT_BL, brightness);
}

