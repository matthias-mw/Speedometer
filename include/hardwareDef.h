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



// --------> NMEA2000 <---------------------
/// GPIO where the CAN-TX is connected
#define ESP32_CAN_TX_PIN (gpio_num_t)35
/// GPIO where the CAN-RX is connected
#define ESP32_CAN_RX_PIN (gpio_num_t)34


#endif // _HARDWAREDEF_H_