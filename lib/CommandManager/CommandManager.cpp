#include <CommandManager.h>
//#include <RunMode.h>


CommandManager::CommandManager(unsigned long iBaud)
{
	mBaudRate = iBaud;
	strcpy(gAck, "ACK");
	strcpy(gNack, "NACK");
}


CommandManager::~CommandManager(void)
{

}


void CommandManager::Begin()
{
	Serial.begin(mBaudRate);
}

void CommandManager::SetupSerialCommands(void) {
  //Serial.begin(mBaudRate);
  //The following statments create serial commands to communicate with the ardunioover USB:
  //mSerCmd.addCommand("<string to call cmd>", <function to execute>);
  mSerCmd.addCommand("CONF?", ConfigurePins);   //Configure the Pin definitions
  mSerCmd.addCommand("SOUT", SetOutput);        //Sets the specified output to the specified state.
  mSerCmd.addCommand("TOUT", ToggleOutput);     //Toggles the specified output from the current output state.
  mSerCmd.addCommand("POUT", PulseOutput);      //sets the output high for a specified amount of time.
  mSerCmd.addCommand("STAT", GetPinStates);     //gets the current state of each output PIN and displays it to the user.
  mSerCmd.addCommand("CSTAT", GetCommaPinStates);     //gets the current state of each output PIN and SENDS BACK AS COMMA SEPERATED LIST. FIRST PARAMETER IS PIN 0 STAT. FROM THERE IT INCRIMENTS BY 1.
  mSerCmd.addCommand("PWM", SetPWM);            //sets the specified PWM Pin to output at the specified frequency, duty cycle, and starting phase. (ex. PWM,<pin>,<duty-cycle>,<frequency>,<starting-phase>)
  mSerCmd.addCommand("DBUG", DebugPrint);       //Prints debug info that might be usefull
  mSerCmd.addCommand("SRT?", CmdQueryScanRate); //Gets the scan rate of the last loop interation.
  mSerCmd.setDefaultHandler(CmdUnknown);        //Default Handler used to send back a notification telling the sender that the command is Unknown (ex. UNK,<your-command>)
}

 void CommandManager::Read(void)
{
	mSerCmd.readSerial();
}

//void CmdMgrQueryMode(void)
//{
//	Serial.println("zzz");
//}

 void CommandManager::ConfigurePins(void)
{
;;  //Future use.
}
 void CommandManager::SetOutput(void) //Called using 'SOUT'
// FINISHED: Completed by Ryan Baas Date: 20170206
/* COMMAND INFO & EXAMPLES:
 FORMAT: SOUT,<OUTPUT NUMBER>,<DESIRED STATE>    //Command is space sensitive.
     EX. SOUT,S1,1          //Sets OUTPUT 1 to the ON state.
     EX. SOUT,02,0          //Sets OUTPUT 2 to the OFF state.
     EX. SOUT,03,1          //Sets OUTPUT 3 to the ON state.
     EX. SOUT,04,1          //Sets OUTPUT 4 to the ON state.
     EX. SOUT,05,0          //Sets OUTPUT 5 to the OFF state.
     EX. SOUT,<OUTPUT NUMBER>,0    //Sets the specified OUTPUT to the OFF state.            */
{
  //char *OutNum;                 //creates a pointer to hold the first argument
  //char *State;                  //creates a pointer to hold the second argument
  //char *result;                 //creates a pointer for the ACK or NACK reponses
  result = gNack;               //initalize Default Result

  //Load command arguments into variables:
  OutNum = mSerCmd.next();
  State = mSerCmd.next();

  //Print command back to user
  Serial.print(OutNum);
  Serial.print(",");
  Serial.println(State);

    //Check to see if arguments were in fact acceptable:
    if (OutNum != NULL && (atoi(State) == 0 || atoi(State) == 1))
    {
      digitalWrite(atoi(OutNum), atoi(State));
      result = gAck;
    }
    //Print result:
    Serial.print(result);
    if (result == gAck)       //if ack then, print ack and display Current State of pin:
    {
      Serial.print(",");
      Serial.println(GetCurState(OutNum)); //This can be used as a confirmation that the pin got set HIGH.
    }
}

 void CommandManager::ToggleOutput(void) //Called using 'TOUT'
// FINISHED: Completed by Ryan Baas Date: 20170206
/* COMMAND INFO & EXAMPLES:
 FORMAT: TOUT,<OUTPUT NUMBER>    //Command is space sensitive.
     EX. TOUT,S1         //Toggles OUTPUT 1 to the ON state.
     EX. TOUT,02         //Toggles OUTPUT 2 to the OFF state.
     EX. TOUT,03         //Toggles OUTPUT 3 to the ON state.
     EX. TOUT,04         //Toggles OUTPUT 4 to the ON state.
     EX. TOUT,05         //Toggles OUTPUT 5 to the OFF state.
     EX. TOUT,<OUTPUT NUMBER>    //Sets the specified OUTPUT to the opposite of the current state.            */
{
  //char *OutNum;                 //creates a pointer to hold the first argument
  //char *result;                 //creates a pointer for the ACK or NACK reponses
  //bool CurState, NextState;
  result = gNack;

  //Load command arguments into variables: just a little change
  OutNum = mSerCmd.next();
  CurState = GetCurState(OutNum);
  Serial.println(OutNum);

  //--------------------------
    //Find out what our next state is going to be.
    NextState = GetNextState(OutNum);

    //Check to see if arguments were in fact acceptable:
    if (NextState != CurState)
    {
      digitalWrite(atoi(OutNum), GetNextState(OutNum));
      result = gAck;
    }
    //Print result:
    Serial.print(result);
    if (result == gAck)       //if ack then, print ack and display Current State of pin:
    {
      Serial.print(",");
      Serial.println(GetCurState(OutNum)); //This can be used as a confirmation that the pin got set HIGH.
    }

}

 void CommandManager::PulseOutput(void) //Called using 'POUT'
// NEEDS WORK!! Might be finished..:
/* COMMAND INFO & EXAMPLES:
 FORMAT: POUT,<OUTPUT NUMBER>,<PULSE DURATION>    //Command is space sensitive.
     EX. POUT,S1,500      //Toggles OUTPUT 1 to the ON state for 500ms then goes LOW.
     EX. POUT,02         //Toggles OUTPUT 2 to the OFF state.
     EX. POUT,03         //Toggles OUTPUT 3 to the ON state.
     EX. POUT,04         //Toggles OUTPUT 4 to the ON state.
     EX. POUT,05         //Toggles OUTPUT 5 to the OFF state.
     EX. POUT,<OUTPUT NUMBER>,<PULSE DURATION>    //Sets the specified OUTPUT to the opposite of the current state.            */
{
  //char *OutNum;                 //creates a pointer to hold the first argument
  //char *PulseTime;              //creates a pointer to hold the second argument
  //char *result;                 //creates a pointer for the ACK or NACK reponses

  result = gNack;

   //Load command arguments into variables:
    OutNum = mSerCmd.next();
    PulseTime = mSerCmd.next();

    //Print command back to user
    Serial.print(OutNum);
    Serial.print(",");
    Serial.println(PulseTime);

    if (OutNum != NULL && PulseTime != NULL && PulseTime < 10000)
    { // need a .contains() type of function here so we can only call this command on output pins.
    digitalWrite(atoi(OutNum), HIGH);
    delay(long(PulseTime));
    digitalWrite(atoi(OutNum), LOW);
    result = gAck;
    }
    //Print result:
    Serial.println(result);
   /* if (result == gAck)       //if ack then, print ack and display Current State of pin:
    {
      Serial.print(",");
      Serial.println(PULSETIME MESURMENT); //This can be used as a confirmation that the pin got set HIGH.
    }   */
}

 void CommandManager::GetPinStates(void) //gets the current state of each output PIN and displays it to the user.
{                       //CALLED BY 'STAT'   :     Completed by Ryan Baas Date: 20170206

  //char *PinNum, *result;
  //int j; // corisponds to inputs state return
  //char* PinStatestxt[4];
  //bool PinState; // corisponds to inputs state return

  ////Load command arguments into variables & initalize result:
  PinNum = mSerCmd.next();
  result = gNack;

  if(PinNum != NULL) //if we have a argument, send back only the specified pin state:
  {
    //Print Out Command:
    Serial.println(PinNum);
    //UPDATE and Print Out Response:
    result = gAck;
    Serial.print(result);
    Serial.print(",");
    Serial.println(GetCurState(PinNum));
  }
  else
  {
    //Print result, NL, Table format, then data.
    result = gAck;
    Serial.println(result);
    Serial.println();                       //NL
    Serial.println("  PIN  | STATE ");     //Table Headers
      Serial.println("-------|-------");     //Dividers
    //Get each output PIN
    for (int i=1; i <= 13; i++) //PRINT ALL 5 OUTPUTS AND THEIR CURRENT STATE
    {

      //j = i+1;
      Serial.print("   ");                  //BlankSpace
      Serial.print(gCharPins[i]);
      //Serial.print(gOutAliases[i]);         //Print output/INPUT number
      if (i>=10)
      {
        Serial.print("  |   ");              //Spaces (Future)
      }
      else
      {
           Serial.print("   |   ");              //Spaces (Future)
      }
      Serial.print(GetCurState(i2str(i,gbuf)));   //Print current state
                 Serial.println("   ");                  //Spcaces (Future)

    }
    /*
    for (int i=7; i <= 8; i++) //THIS WILL BE USEFULL BUT FOR NOW I JUST NEED TO GET ON WITH THIS PROJECT
    {
      j = i-7;
      Serial.print("   ");                  //BlankSpace
      Serial.print(gInAliases[j]);         //Print output/INPUT number
           Serial.print("  |   ");              //Spaces (Future)
      Serial.print(GetCurState(i2str(i,gbuf)));   //Print current state
                 Serial.println("   ");                  //Spcaces (Future)
    }
    */
     //Get each input state
//    for (int i; i <= 1; i++)
//    {
//      j = i+7;
//      Serial.print("   ");                  //BlankSpace
//      Serial.print(gInAliases[i]);         //Print output/INPUT number
//           Serial.print("  |   ");              //Spaces (Future)
//      Serial.print(GetCurState(i2str(j,gbuf)));   //Print current state
//                 Serial.println("   ");                  //Spcaces (Future)
//    }
  }
}

//Serial.println(bitRead(PORTD,3)); //Reads bit 3 of register PORTD which contains the current state (high/low) of pin 3.

 void CommandManager::GetCommaPinStates(void)
{
    //Print command back to user
    Serial.print("CSTAT");
    Serial.print(",");
 for (int i = 0; i<=13; i++)
 {
  Serial.print(GetCurState(i2str(i,gbuf)));   //Print current state
  if(i == 13)
  {

  }
  else{
  Serial.print(",");
  }
 }
 Serial.println();
}

 void SetPWM(void)
{
 // NEEDS WORK!!
/* COMMAND INFO & EXAMPLES:
 FORMAT: PWM,<PIN>,<DESIRED-ANGLE>    //<FREQUENCY><DUTY-CYCLE>    //Command is space sensitive.
     EX. PWM,11,22,50      //Sets pin 11 to output PWM with 22_Hz and 50% Duty Cycle
     */
//INITIALIZE PARAMETER POINTERS
///char *result, *iPin, *iSteps;   //, *iFreq, *iDutyCycle, *result;


//SAVE INPUT PARAMETERS TO VARIABLES
iPin = mSerCmd.next();
iSteps = mSerCmd.next();
//iFreq = mSerCmd.next();
//iDutyCycle = mSerCmd.next();
result = gNack;

if(iPin != NULL && iSteps != NULL) //if we have a argument(Pin Number), send back an Ack and use default values:
  {
    //Print Out Command:
    Serial.print(iPin);
    Serial.print(",");
    Serial.println(iSteps);
    //UPDATE and Print Out Response:
    result = gAck;
    Serial.println(result);

//Move MTR To Positon
// This should be re done into a class
for (int i = 0; i <= atoi(iSteps); i += 1)  // goes from 0 degrees to 180 degrees
  {
  digitalWrite(atoi(iPin), HIGH);
  delayMicroseconds(1); // Approximately 10% duty cycle @ 10KHz
  digitalWrite(atoi(iPin), LOW);
  delayMicroseconds(100 - 1);
  }
//int pos = 0;    // variable to store the servo position
/* for (pos = 0; pos <= atoi(iSteps); pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  } TOUT,4
 */
/*  for (pos = 180; pos >= 0; pos -= 1)  // goes from 180 degrees to 0 degrees
 *   {
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
      } */

  }

}//end function 'SetPWM'



 void CommandManager::CmdUnknown(const char *iCommand) //Called on anything sent that is not defined as a command (ex. 'TPGTRIG')
{
  Serial.print("UNK,");
  Serial.println(iCommand);
}

 void CommandManager::DebugPrint(void) //FUNCTION FOR SERIAL DEBUGING
{
  //Your Debugging Statments Here:
  Serial.println("Put your DEBUGGING code in the function 'DebugPrint' and it will be displayed here.");
  /*for (int i = 7;i<=8;i++)
  {
  Serial.println();
  Serial.println(GetCurState(i2str(i,gbuf)));
} */
}

 void CommandManager::CmdQueryScanRate() //Called using 'SRT?'
/* COMMAND INFO & EXAMPLES:
 FORMAT: SRT?               //Command has no parameters.
     EX. SRT?               //Returns the scan rate of the previous microprocessor loop.  */
{
  Serial.print(gScanRate);
  Serial.print(",");
  Serial.print(gScanRateMin);
  Serial.print(",");
  Serial.println(gScanRateMax);
}
