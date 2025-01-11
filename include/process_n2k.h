/*!
 * \file process_n2k.h
 * \brief Processing all NMEA2000 messages
 * 
 * This file contains the functions for processing all NMEA2000 messages.
 * 
 * \author Matthias Werner
 * \date   January 2025
 * \version 0.1
 * 
 * 
 */

#ifndef _PROCESS_N2K_H_
#define _PROCESS_N2K_H_


#include <hardwareDef.h>

#include <N2kMessages.h>



typedef struct {
  unsigned long PGN;
  void (*Handler)(const tN2kMsg &N2kMsg); 
} tNMEA2000Handler;

void SystemTime(const tN2kMsg &N2kMsg);
void EngineRapid(const tN2kMsg &N2kMsg);
void EngineDynamicParameters(const tN2kMsg &N2kMsg);
void TransmissionParameters(const tN2kMsg &N2kMsg);

extern Stream *OutputStream;


void initN2K( void);




void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);
void updateN2K( void) ;








#endif // _PROCESS_N2K_H_
