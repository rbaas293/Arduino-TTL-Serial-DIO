/*
CommandManager is designed to be a all in one container, if you would, for all
serial communication data. That data including variables, arrays, and functions.
Date: 20180821
Name: Ryan Baas
*/

#ifndef CommandManager_h
#define CommandManager_h
#include <Arduino.h>
#include <SerialCommand.h>

extern bool NextState;

class CommandManager
{
public:
	CommandManager(unsigned long iBaud);
	~CommandManager(void);

	void Begin(void);
	static void SetupSerialCommands(void);
	void Read(void);
	//static void On_Command1(void);
	static void ConfigurePins(void);
	void SetOutput(void); //Called using 'SOUT'
  void ToggleOutput(void); //Called using 'TOUT'
	void PulseOutput(void); //Called using 'POUT'
	void GetPinStates(void); //gets the current state of each output PIN and displays it to the user.
  void GetCommaPinStates(void);
	void SetPWM(void);
	void CmdUnknown(const char *iCommand); //Called on anything sent that is not defined as a command (ex. 'TPGTRIG')
	void DebugPrint(void); //FUNCTION FOR SERIAL DEBUGING
	void CmdQueryScanRate(void); //Called using 'SRT?'
	SerialCommand mSerCmd;
protected:
	unsigned long mBaudRate;
	bool O2_State, O3_State;
	char *result;                 //creates a pointer for the ACK or NACK reponses
	char *OutNum;                 //creates a pointer to hold the first argument
	char *State;                  //creates a pointer to hold the second argument
	char *PulseTime;							//For `PulseOutput`
	char *PinNum;									//for `GetPinStates`
	char *iPin, *iSteps;					//For `SetPWM`
};

void CmdMgrQueryMode(void);
//void CmdMgrUnknownCommand(const char *iCommand);


#endif // TPGCommandManager_h
