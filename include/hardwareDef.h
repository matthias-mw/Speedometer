/*! \file 	hardwareDef.h
 *  \brief  Definition of Hardware Interfaces
 *
 *  Test
 *
 * \author 		Matthias Werner
 * \date		10 Sep 2022
 *
 * - Hardware:          az-delivery-devkit-v4
 * - Prozessor:         ESP32-WROOM
 *
 * ************************************************************
 * \version PROJECT_VERSION
 *************************************************************/

#ifndef _HARDWAREDEF_H_
#define _HARDWAREDEF_H_

#include <Arduino.h>


// Displayhardware

/// Define PIN for Date/Command Pin
#define DISPLAY_DC  17
/// Define PIN for Chip Select
#define DISPLAY_CS  5
/// Define PIN for Reset
#define DISPLAY_RST 8
/// Define PIN for Backlight
#define DISPLAY_BL 16



#endif // _HARDWAREDEF_H_