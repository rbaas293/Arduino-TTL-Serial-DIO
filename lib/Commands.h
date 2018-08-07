#ifndef Commands_h
#define Commands_h
#include "Arduino.h"
#include "SerialCommand.h"

//Declare Serial Command Functions
void SetupSerialCommands(void);
void ConfigurePins(void);
void SetOutput(void); //Called using 'SOUT'
void ToggleOutput(void); //Called using 'TOUT'
void PulseOutput(void); //Called using 'POUT'
void GetPinStates(void); //gets the current state of each output PIN and displays it to the user.
void GetCommaPinStates(void);
void SetPWM(void);
void CmdUnknown(const char *iCommand); //Called on anything sent that is not defined as a command (ex. 'TPGTRIG')
void DebugPrint(void); //FUNCTION FOR SERIAL DEBUGING
void CmdQueryScanRate(void); //Called using 'SRT?'
