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

#include <NMEA2000_CAN.h>
#include <N2kMessages.h>
#include <N2kMessagesEnumToStr.h>

typedef struct {
  unsigned long PGN;
  void (*Handler)(const tN2kMsg &N2kMsg); 
} tNMEA2000Handler;

void SystemTime(const tN2kMsg &N2kMsg);
void EngineRapid(const tN2kMsg &N2kMsg);
void EngineDynamicParameters(const tN2kMsg &N2kMsg);
void TransmissionParameters(const tN2kMsg &N2kMsg);

tNMEA2000Handler NMEA2000Handlers[]={
  {126992L,&SystemTime},
  {127488L,&EngineRapid},
  {127489L,&EngineDynamicParameters},
  {127493L,&TransmissionParameters},
  {0,0}
};

Stream *OutputStream;

void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);


int i = 10;

//*****************************************************************************
template<typename T> void PrintLabelValWithConversionCheckUnDef(const char* label, T val, double (*ConvFunc)(double val)=0, bool AddLf=false, int8_t Desim=-1 ) {
  OutputStream->print(label);
  if (!N2kIsNA(val)) {
    if ( Desim<0 ) {
      if (ConvFunc) { OutputStream->print(ConvFunc(val)); } else { OutputStream->print(val); }
    } else {
      if (ConvFunc) { OutputStream->print(ConvFunc(val),Desim); } else { OutputStream->print(val,Desim); }
    }
  } else OutputStream->print("not available");
  if (AddLf) OutputStream->println();
}

void setup(void)
{
  
  Serial.begin(115200); delay(500);
  OutputStream=&Serial;
//   while (!Serial) 
   
//  NMEA2000.SetN2kCANReceiveFrameBufSize(50);
  // Do not forward bus messages at all
  NMEA2000.SetForwardType(tNMEA2000::fwdt_Text);
  NMEA2000.SetForwardStream(OutputStream);
  // Set false below, if you do not want to see messages parsed to HEX withing library
  NMEA2000.EnableForward(false);
  NMEA2000.SetMsgHandler(HandleNMEA2000Msg);
//  NMEA2000.SetN2kCANMsgBufSize(2);
  NMEA2000.Open();
  OutputStream->print("Running...");


  // Init the display
  initDisplay();



  delay(3000);

}

//*****************************************************************************
//NMEA 2000 message handler
void HandleNMEA2000Msg(const tN2kMsg &N2kMsg) {
  int iHandler;
  
  // Find handler
  OutputStream->print("In Main Handler: "); OutputStream->println(N2kMsg.PGN);
  for (iHandler=0; NMEA2000Handlers[iHandler].PGN!=0 && !(N2kMsg.PGN==NMEA2000Handlers[iHandler].PGN); iHandler++);
  
  if (NMEA2000Handlers[iHandler].PGN!=0) {
    NMEA2000Handlers[iHandler].Handler(N2kMsg); 
  }
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
  NMEA2000.ParseMessages();
}


//*****************************************************************************
void EngineRapid(const tN2kMsg &N2kMsg) {
    unsigned char EngineInstance;
    double EngineSpeed;
    double EngineBoostPressure;
    int8_t EngineTiltTrim;
    
    if (ParseN2kEngineParamRapid(N2kMsg,EngineInstance,EngineSpeed,EngineBoostPressure,EngineTiltTrim) ) {
      PrintLabelValWithConversionCheckUnDef("Engine rapid params: ",EngineInstance,0,true);
      PrintLabelValWithConversionCheckUnDef("  RPM: ",EngineSpeed,0,true);
      PrintLabelValWithConversionCheckUnDef("  boost pressure (Pa): ",EngineBoostPressure,0,true);
      PrintLabelValWithConversionCheckUnDef("  tilt trim: ",EngineTiltTrim,0,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void EngineDynamicParameters(const tN2kMsg &N2kMsg) {
    unsigned char EngineInstance;
    double EngineOilPress;
    double EngineOilTemp;
    double EngineCoolantTemp;
    double AltenatorVoltage;
    double FuelRate;
    double EngineHours;
    double EngineCoolantPress;
    double EngineFuelPress; 
    int8_t EngineLoad;
    int8_t EngineTorque;
    tN2kEngineDiscreteStatus1 Status1;
    tN2kEngineDiscreteStatus2 Status2;
    
    if (ParseN2kEngineDynamicParam(N2kMsg,EngineInstance,EngineOilPress,EngineOilTemp,EngineCoolantTemp,
                                 AltenatorVoltage,FuelRate,EngineHours,
                                 EngineCoolantPress,EngineFuelPress,
                                 EngineLoad,EngineTorque,Status1,Status2) ) {
      PrintLabelValWithConversionCheckUnDef("Engine dynamic params: ",EngineInstance,0,true);
      PrintLabelValWithConversionCheckUnDef("  oil pressure (Pa): ",EngineOilPress,0,true);
      PrintLabelValWithConversionCheckUnDef("  oil temp (C): ",EngineOilTemp,&KelvinToC,true);
      PrintLabelValWithConversionCheckUnDef("  coolant temp (C): ",EngineCoolantTemp,&KelvinToC,true);
      PrintLabelValWithConversionCheckUnDef("  altenator voltage (V): ",AltenatorVoltage,0,true);
      PrintLabelValWithConversionCheckUnDef("  fuel rate (l/h): ",FuelRate,0,true);
      PrintLabelValWithConversionCheckUnDef("  engine hours (h): ",EngineHours,&SecondsToh,true);
      PrintLabelValWithConversionCheckUnDef("  coolant pressure (Pa): ",EngineCoolantPress,0,true);
      PrintLabelValWithConversionCheckUnDef("  fuel pressure (Pa): ",EngineFuelPress,0,true);
      PrintLabelValWithConversionCheckUnDef("  engine load (%): ",EngineLoad,0,true);
      PrintLabelValWithConversionCheckUnDef("  engine torque (%): ",EngineTorque,0,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void TransmissionParameters(const tN2kMsg &N2kMsg) {
    unsigned char EngineInstance;
    tN2kTransmissionGear TransmissionGear;
    double OilPressure;
    double OilTemperature;
    unsigned char DiscreteStatus1;
    
    if (ParseN2kTransmissionParameters(N2kMsg,EngineInstance, TransmissionGear, OilPressure, OilTemperature, DiscreteStatus1) ) {
      PrintLabelValWithConversionCheckUnDef("Transmission params: ",EngineInstance,0,true);
                        OutputStream->print("  gear: "); PrintN2kEnumType(TransmissionGear,OutputStream);
      PrintLabelValWithConversionCheckUnDef("  oil pressure (Pa): ",OilPressure,0,true);
      PrintLabelValWithConversionCheckUnDef("  oil temperature (C): ",OilTemperature,&KelvinToC,true);
      PrintLabelValWithConversionCheckUnDef("  discrete status: ",DiscreteStatus1,0,true);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

//*****************************************************************************
void SystemTime(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    uint16_t SystemDate;
    double SystemTime;
    tN2kTimeSource TimeSource;
    
    if (ParseN2kSystemTime(N2kMsg,SID,SystemDate,SystemTime,TimeSource) ) {
                      OutputStream->println("System time:");
      PrintLabelValWithConversionCheckUnDef("  SID: ",SID,0,true);
      PrintLabelValWithConversionCheckUnDef("  days since 1.1.1970: ",SystemDate,0,true);
      PrintLabelValWithConversionCheckUnDef("  seconds since midnight: ",SystemTime,0,true);
                        OutputStream->print("  time source: "); PrintN2kEnumType(TimeSource,OutputStream);
    } else {
      OutputStream->print("Failed to parse PGN: "); OutputStream->println(N2kMsg.PGN);
    }
}

