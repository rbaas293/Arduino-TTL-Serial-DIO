/*
 By: Ryan Baas
 Date: 02-02-18

This code is designed to make a arduino into a simple Serial DIO Controller. It is usefull for any application requiring TTL digital logic.
04-16-18: ALPHA/POC VERSION 1.0
08-16-18: ITS BEEN 4 MONTHS SINCE THIS PROJECT WAS STARTED AS A POC. WOW TIME FLIES
*/
//Included Libraries:
#include <Arduino.h>
#include <string.h>
#include <SerialCommand.h>
//#include <SoftwareSerial.h>
//#include <EEPROM.h>
#include <Servo.h>
#include "Commands.h"
#include "ISRs.h"
#include <DirectIO.h>
#include <PinChangeInterrupt.h>

#define str(x) #x //just playin around.
#define kSerialBaudRate 9600

#ifndef AVR_UNO
    #ifndef AVR_MEGA2560
    #define AVR_MEGA2560 //At this point if both are NOT defined, then define as a MEGA
    #define MANDEF // SET A FLAG IF THE BOARD WAS NOT AUTO DEFINED. Stands for: MANUALY DEFINED
    #endif
#endif

#ifdef AVR_UNO //If the board is an Arduino UNO
//PINS 0 & 1 TO BE USED FOR SERIAL R//X AND TX
#define S0 0        //D0  Serial Rx
#define S1 1        //D1  Serial Tx
//DEFINE OUTPUT AND INPUT ALIASES
#define O2 2        //D2  MULTI-PURPOSE
#define O3 3        //D3  MULTI-PURPOSE
#define O4 4        //D4  Output
#define O5 5        //D5  Output
#define O6 6        //D6  Output
#define O7 7        //D7  Output
#define I8 8        //D8  Input
#define I9 9        //D9  Input
#define I10 10      //D10 Input
#define I11 11      //D11 Input
#define I12 12      //D12 Input
#define I13 13      //D13 Input

#endif

#ifdef AVR_MEGA2560 //If the board is an Arduino MEGA 2560
//PINS 0 & 1 TO BE USED FOR SERIAL R//X AND TX
#define S0 0        //D0  Serial Rx
#define S1 1        //D1  Serial Tx
//DEFINE OUTPUT AND INPUT ALIASES
#define O2 2        //D2  MULTI-PURPOSE
#define O3 3        //D3  MULTI-PURPOSE
#define O4 4        //D4  Output
#define O5 5        //D5  Output
#define O6 6        //D6  Output
#define O7 7        //D7  Output
#define I8 8        //D8  Input
#define I9 9        //D9  Input
#define I10 10      //D10 Input
#define I11 11      //D11 Input
#define I12 12      //D12 Input
#define I13 13      //D13 Input

#endif
//Initilize Objects:
SerialCommand gSerialCommands;
//void SetupSerialCommands(void);
//Set Up Functions/Methods:
char GetCurState(int iPinNum);    //gets the CURRENT state of the specified pin number.
bool GetNextState(char *iPinNum); //gets the NEXT state of the specified pin number.
char *i2str(int i, char *buf);    //int to string fuction, cuz sting(int) would not work.
unsigned long ElapsedTime(unsigned long iStartTime, unsigned long iCurTime);
void myDelay(int ix);

//Declare Serial Command Functions
//void SetupSerialCommands(void);
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

void ISR2(void); //Sends [  STCHG,<PIN-NUMBER>,<NEW STATE>  ]
void ISR3(void); //Sends [  STCHG,<PsIN-NUMBER>,<NEW STATE>  ]
void ISR4(void);
void ISR8(void);
void ISR9(void);
void ISR10(void);
void ISR11(void);
void ISR12(void);
void ISR13(void);

//void SetupSerialCommands(void);
//Set Up Variables and Arrays
unsigned long gScanRate;
unsigned long gScanRateMin;
unsigned long gScanRateMax;
unsigned long ScanStartMicros;
bool O2_State, O3_State;
bool AnalogAsDigital = 0; //defaut us 'false'. Change to 'true' if you would like to use Analog pins as Digital:
char gAck[4];
char gNack[5];
char gbuf[8];
char* gOutAliases[]={"S1", "O2", "O3", "O4", "O5"};
char* gInAliases[]={"I8", "I9", "I10","I11","I12","I3"};
char* gCharPins[]={"0","1","2","3","4","5","6","7","8","9","10","11","12","13"};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void setup() {
 //cli();//stop interrupts

 //SetupSerialCommands();

 //DEFINE Anolog Pins
 if (AnalogAsDigital == true) //If this flag is set, then define A0-A5 as Digital instead of Analog.
 {
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
 }
 else{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
 }
 //DEFINE OUTPUTS
 pinMode(O2, OUTPUT);
 pinMode(O3, OUTPUT);
 pinMode(O4, OUTPUT);
 pinMode(O5, OUTPUT);
 pinMode(O6, OUTPUT);
 pinMode(O7, OUTPUT);
 //DEFINE INPUTS
 pinMode(I8, INPUT);
 pinMode(I9, INPUT);
 pinMode(I10,INPUT);
 pinMode(I11,INPUT);
 pinMode(I12,INPUT);
 pinMode(I13,INPUT);
//attachPCINT(interrupt, function, mode);
attachPCINT(digitalPinToPCINT(I8), ISR8, CHANGE);
attachPCINT(digitalPinToPCINT(I9), ISR9, CHANGE);
attachPCINT(digitalPinToPCINT(I10), ISR10, CHANGE);
attachPCINT(digitalPinToPCINT(I11), ISR11, CHANGE);
attachPCINT(digitalPinToPCINT(I12), ISR12, CHANGE);
attachPCINT(digitalPinToPCINT(I13), ISR13, CHANGE);

Serial.begin(kSerialBaudRate);

//The following statments create serial commands to communicate with the ardunioover USB:
//gSerialCommands.addCommand("<string to call cmd>", <function to execute>);
gSerialCommands.addCommand("CONF?", ConfigurePins);   //Configure the Pin definitions
gSerialCommands.addCommand("SOUT", SetOutput);        //Sets the specified output to the specified state.
gSerialCommands.addCommand("TOUT", ToggleOutput);     //Toggles the specified output from the current output state.
gSerialCommands.addCommand("POUT", PulseOutput);      //sets the output high for a specified amount of time.
gSerialCommands.addCommand("STAT", GetPinStates);     //gets the current state of each output PIN and displays it to the user.
gSerialCommands.addCommand("CSTAT", GetCommaPinStates);     //gets the current state of each output PIN and SENDS BACK AS COMMA SEPERATED LIST. FIRST PARAMETER IS PIN 0 STAT. FROM THERE IT INCRIMENTS BY 1.
gSerialCommands.addCommand("PWM", SetPWM);            //sets the specified PWM Pin to output at the specified frequency, duty cycle, and starting phase. (ex. PWM,<pin>,<duty-cycle>,<frequency>,<starting-phase>)
gSerialCommands.addCommand("DBUG", DebugPrint);       //Prints debug info that might be usefull
gSerialCommands.addCommand("SRT?", CmdQueryScanRate); //Gets the scan rate of the last loop interation.
gSerialCommands.setDefaultHandler(CmdUnknown);        //Default Handler used to send back a notification telling the sender that the command is Unknown (ex. UNK,<your-command>)

//Set inital values
gScanRateMin = 999999;
gScanRateMax = 0;
strcpy(gAck, "ACK");
strcpy(gNack, "NACK");
digitalWrite(O2, LOW);digitalWrite(O3, LOW);digitalWrite(O4, LOW);digitalWrite(O5, LOW);digitalWrite(O5, LOW);digitalWrite(O6, LOW);digitalWrite(O7, LOW); //INITALIZE OUTPUTS
///////////////////TESTING///////////////////////////////
#ifdef MANDEF
Serial.print("Board is manually defined as: ");
#else
Serial.print("Board = ")
#endif

#ifdef AVR_UNO
Serial.println("AVR_UNO");
#elif defined AVR_MEGA2560
Serial.println("AVR_MEGA2560");
#endif
///////////////////TESTING///////////////////////////////
Serial.println("READY");
//sei();//allow interrupts
}

void loop()
{
  // to run repeatedly:
ScanStartMicros = micros();

 cli();//stop interrupts

 // check for serial activity
  gSerialCommands.readSerial();
  gScanRate = ElapsedTime(ScanStartMicros, micros());
  if (gScanRate < gScanRateMin) gScanRateMin = gScanRate;
  if (gScanRate > gScanRateMax) gScanRateMax = gScanRate;

  sei();//allow interrupts
}

/*char GetSerialCommandTxt(char cmd, char arg1, char arg2)
{;;} */

bool GetCurState(char *iPinNum) //gets the current state of the specified PIN.
{
  int pin;
  bool PinState;

  pin = atoi(iPinNum);

    if ( pin == I9 || pin == I8) //If the pin called was an input then use digitalRead
    {
      PinState = digitalRead(pin);
    }
    else                      //Then the pin is an output, so use the following to obtain the current state.
    {
      PinState = (0!=(*portOutputRegister( digitalPinToPort(pin) ) & digitalPinToBitMask(pin)));
    }

  return PinState;
}

bool GetNextState(char *iPinNum) //gets the NEXT state of the specified pin number.
                                //Will return TRUE or FALSE or HIGH or LOW  :   Completed by Ryan Baas Date: 20170206
{
  bool PinState, NextPinState;

  PinState = GetCurState(iPinNum);
  NextPinState = !PinState;
  return NextPinState;
}


unsigned long ElapsedTime(unsigned long iStartTime, unsigned long iCurTime)
/* Calculates the time elapsed between the start time and the current time
 Assumes the start should always be before the current and then compensates
 if the milli or micro clock has rolled over    */
{
  unsigned long elapsedTime;

  if (iCurTime >= iStartTime)
  {
    elapsedTime = iCurTime - iStartTime;
  }
  else // clock rolled over
  {
    elapsedTime =  (unsigned long)0xFFFFFFFF - iStartTime + iCurTime;
  }
  return elapsedTime;
}

char *i2str(int i, char *buf) //FUNNCTION TAKEN FROM ONLINE: INT TO STRING
{
  byte l=0;
  if(i<0) buf[l++]='-';
  boolean leadingZ=true;
  for(int div=10000, mod=0; div>0; div/=10){
    mod=i%div;
    i/=div;
    if(!leadingZ || i!=0){
       leadingZ=false;
       buf[l++]=i+'0';
    }
    i=mod;
  }
  buf[l]=0;
  return buf;
}

void myDelay(int ix)   {
  for(int i=0; i<=ix; i++)
  {
    delayMicroseconds(1000);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////
//SERIAL COMMAND HANDLERS below
////////////////////////////////////////////////////////////////////////////////////////////
/*
void SetupSerialCommands(void) //SerialCommand() SetupSerialCommand()
{
  Serial.begin(kSerialBaudRate);
//The following statments create serial commands to communicate with the ardunioover USB:
//gSerialCommands.addCommand("<string to call cmd>", <function to execute>);
  gSerialCommands.addCommand("CONF?", ConfigurePins);   //Configure the Pin definitions
  gSerialCommands.addCommand("SOUT", SetOutput);        //Sets the specified output to the specified state.
  gSerialCommands.addCommand("TOUT", ToggleOutput);     //Toggles the specified output from the current output state.
  gSerialCommands.addCommand("POUT", PulseOutput);      //sets the output high for a specified amount of time.
  gSerialCommands.addCommand("STAT", GetPinStates);     //gets the current state of each output PIN and displays it to the user.
  gSerialCommands.addCommand("CSTAT", GetCommaPinStates);     //gets the current state of each output PIN and SENDS BACK AS COMMA SEPERATED LIST. FIRST PARAMETER IS PIN 0 STAT. FROM THERE IT INCRIMENTS BY 1.
  gSerialCommands.addCommand("PWM", SetPWM);            //sets the specified PWM Pin to output at the specified frequency, duty cycle, and starting phase. (ex. PWM,<pin>,<duty-cycle>,<frequency>,<starting-phase>)
  gSerialCommands.addCommand("DBUG", DebugPrint);       //Prints debug info that might be usefull
  gSerialCommands.addCommand("SRT?", CmdQueryScanRate); //Gets the scan rate of the last loop interation.
  gSerialCommands.setDefaultHandler(CmdUnknown);        //Default Handler used to send back a notification telling the sender that the command is Unknown (ex. UNK,<your-command>)
}
*/
void ConfigurePins(void)
{
;;  //Future use.
}
void SetOutput(void) //Called using 'SOUT'
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
  char *OutNum;                 //creates a pointer to hold the first argument
  char *State;                  //creates a pointer to hold the second argument
  char *result;                 //creates a pointer for the ACK or NACK reponses
  result = gNack;               //initalize Default Result

  //Load command arguments into variables:
  OutNum = gSerialCommands.next();
  State = gSerialCommands.next();

  //Print command back to user
  Serial.print(OutNum);
  Serial.print(",");
  Serial.println(State);

    //Check to see if arguments were in fact acceptable:
    if (OutNum != NULL && (atoi(State) == 0 || atoi(State) == 1) && OutNum != I9 && OutNum != I8 )
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

void ToggleOutput(void) //Called using 'TOUT'
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
  char *OutNum;                 //creates a pointer to hold the first argument
  char *result;                 //creates a pointer for the ACK or NACK reponses
  bool CurState, NextState;
  result = gNack;

  //Load command arguments into variables: just a little change
  OutNum = gSerialCommands.next();
  CurState = GetCurState(OutNum);
  Serial.println(OutNum);

  //--------------------------
    //Find out what our next state is going to be.
    NextState = GetNextState(OutNum);

    //Check to see if arguments were in fact acceptable:
    if (NextState != CurState && OutNum != I9 && OutNum != I8 )
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

void PulseOutput(void) //Called using 'POUT'
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
  char *OutNum;                 //creates a pointer to hold the first argument
  char *PulseTime;              //creates a pointer to hold the second argument
  char *result;                 //creates a pointer for the ACK or NACK reponses

  result = gNack;

   //Load command arguments into variables:
    OutNum = gSerialCommands.next();
    PulseTime = gSerialCommands.next();

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

void GetPinStates(void) //gets the current state of each output PIN and displays it to the user.
{                       //CALLED BY 'STAT'   :     Completed by Ryan Baas Date: 20170206

  char *PinNum, *result;
  //int j; // corisponds to inputs state return
  //char* PinStatestxt[4];
  //bool PinState; // corisponds to inputs state return

  ////Load command arguments into variables & initalize result:
  PinNum = gSerialCommands.next();
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

void GetCommaPinStates(void)
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
char *result, *iPin, *iSteps;   //, *iFreq, *iDutyCycle, *result;


//SAVE INPUT PARAMETERS TO VARIABLES
iPin = gSerialCommands.next();
iSteps = gSerialCommands.next();
//iFreq = gSerialCommands.next();
//iDutyCycle = gSerialCommands.next();
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
int pos = 0;    // variable to store the servo position
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



void CmdUnknown(const char *iCommand) //Called on anything sent that is not defined as a command (ex. 'TPGTRIG')
{
  Serial.print("UNK,");
  Serial.println(iCommand);
}

void DebugPrint(void) //FUNCTION FOR SERIAL DEBUGING
{
  //Your Debugging Statments Here:
  Serial.println("Put your DEBUGGING code in the function 'DebugPrint' and it will be displayed here.");
  /*for (int i = 7;i<=8;i++)
  {
  Serial.println();
  Serial.println(GetCurState(i2str(i,gbuf)));
} */
}

void CmdQueryScanRate() //Called using 'SRT?'
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
//////////////////////////////////////////////////////////////////////////////////////////////
// START ISRs
/////////////////////////////////////////////////////////////////////////////////////////////
void ISR2() //Sends [  STCHG,<PIN-NUMBER>,<NEW STATE>  ]
{
  //O2_State = !O2_State;
  //Serial.print("STCHG,2,"); //Send STCHG. AKA STATE CHANGE
  //Serial.println(O2_State);
}

void ISR3() //Sends [  STCHG,<PIN-NUMBER>,<NEW STATE>  ]
{
  //O3_State = !O3_State;
  //Serial.print("STCHG,3,"); //Send STCHG. AKA STATE CHANGE
  //Serial.println(O3_State);
}

void ISR4()
{
// Placeholder

}
void ISR8()
{
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(I8));
    if(trigger == RISING){
    ;;  // Do something
    }
    else if(trigger == FALLING){
    ;;  // Do something
    }
    else
    ;;  // Wrong usage (trigger == CHANGE)

}
void ISR9()
{
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(I9));
    if(trigger == RISING){
    ;;  // Do something
    }
    else if(trigger == FALLING){
    ;;  // Do something
    }
    else
      // Wrong usage (trigger == CHANGE)
;;
}
void ISR10()
{
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(I10));
    if(trigger == RISING){
    ;;  // Do something
    }
    else if(trigger == FALLING){
    ;;  // Do something
    }
    else
    ;;  // Wrong usage (trigger == CHANGE)

}
void ISR11()
{
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(I11));
    if(trigger == RISING){
    ;;  // Do something
    }
    else if(trigger == FALLING){
    ;;  // Do something
    }
    else
    ;;  // Wrong usage (trigger == CHANGE)

}
void ISR12()
{
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(I12));
    if(trigger == RISING){
    ;;  // Do something
    }
    else if(trigger == FALLING){
    ;;  // Do something
    }
    else
    ;;  // Wrong usage (trigger == CHANGE)

}
void ISR13()
{
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(I13));
    if(trigger == RISING){
    ;;  // Do something
    }
    else if(trigger == FALLING){
    ;;  // Do something
    }
    else
    ;;  // Wrong usage (trigger == CHANGE)
}
