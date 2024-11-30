/**
 * \file  main.c
 * \brief Mainfile for the Speedometer project
 *
 * This file contains the main function for the Speedometer project. This
 * project task provide a simple display with the main task of displaying
 * the engine speed and other information. The Device is connected to a
 * NMEA 2000 network and receives the engine speed from the engine ECU.
 *
 * \author Matthias Werner
 * \date   November  2024
 * \version 0.1
 *
 *
 */
#include <Arduino.h>
#include <hardwareDef.h>
#include <stdint.h>

#include <displayCtl.h>

/// Display constructor for primary hardware SPI connection
Adafruit_GC9A01A displayGlobal(DISPLAY_CS, DISPLAY_DC);

DisplayCtl* displayControl;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Speedometer started");

  displayControl = new DisplayCtl(&displayGlobal);


  // delay(2000);


  displayControl->displayEngineSpeed(3456);
  // delay(2000);

}

void loop()
{
  // put your main code here, to run repeatedly:
  Serial.println("Speedometer running");
  delay(1000);
  //displayControl.displayEngineSpeed(3456);
  delay(2000);
}
