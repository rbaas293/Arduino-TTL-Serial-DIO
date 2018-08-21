/*
 By: Ryan Baas
 Date: 02-02-18

This code is designed to make a arduino into a simple Serial DIO Controller. It is usefull for any application requiring TTL digital logic.
04-16-18: ALPHA/POC VERSION 1.0
08-16-18: ITS BEEN 4 MONTHS SINCE THIS PROJECT WAS STARTED AS A POC. WOW TIME FLIES
*/
//Included Libraries:

#include "Arduino.h"
#include <string.h>
//#include <SerialCommand.h>
//#include <SoftwareSerial.h>
//#include <EEPROM.h>
#include <Servo.h>
#include <DirectIO.h>
#include <PinChangeInterrupt.h>
#include <CommandManager.h>
#include <PinDef.h>

#define str(x) #x //just playin around.
//PinDef?


//Initilize Objects:
CommandManager cm(9600);
//SerialCommand gSerialCommands;
//void SetupSerialCommands(void);

//Set Up Functions/Methods:
char GetCurState(int iPinNum);    //gets the CURRENT state of the specified pin number.
bool GetNextState(char *iPinNum); //gets the NEXT state of the specified pin number.
char *i2str(int i, char *buf);    //int to string fuction, cuz sting(int) would not work.
unsigned long ElapsedTime(unsigned long iStartTime, unsigned long iCurTime);
void myDelay(int ix);

//Declare Serial Command Functions

void ISR2(void); //Sends [  STCHG,<PIN-NUMBER>,<NEW STATE>  ]
void ISR3(void); //Sends [  STCHG,<PsIN-NUMBER>,<NEW STATE>  ]
void ISR4(void);
void ISR8(void);
void ISR9(void);
void ISR10(void);
void ISR11(void);
void ISR12(void);
void ISR13(void);


//Set Up Variables and Arrays
char gAck[4];
char gNack[5];
unsigned long gScanRate;
unsigned long gScanRateMin;
unsigned long gScanRateMax;
unsigned long ScanStartMicros;
bool AnalogAsDigital = 0; //defaut us 'false'. Change to 'true' if you would like to use Analog pins as Digital
char gbuf[8];
char* gOutAliases[]={"S1", "O2", "O3", "O4", "O5"};
char* gInAliases[]={"I8", "I9", "I10","I11","I12","I3"};
char* gCharPins[]={"0","1","2","3","4","5","6","7","8","9","10","11","12","13"};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void setup() {
 //cli();//stop interrupts
 cm.Begin();
 cm.SetupSerialCommands();

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

/*
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
*/

//Set inital values
gScanRateMin = 999999;
gScanRateMax = 0;
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

 //cli();//stop interrupts

 // check for serial activity
  cm.Read();
  gScanRate = ElapsedTime(ScanStartMicros, micros());
  if (gScanRate < gScanRateMin) gScanRateMin = gScanRate;
  if (gScanRate > gScanRateMax) gScanRateMax = gScanRate;

 //sei();//allow interrupts
}

/*char GetSerialCommandTxt(char cmd, char arg1, char arg2)
{;;} */

bool GetCurState(char *iPinNum) //gets the current state of the specified PIN.
{
  int pin;
  bool PinState;

  pin = atoi(iPinNum);

    //Use the following to obtain the current state of all pins, even if it is an output.
    {
      PinState = (0!=(*portOutputRegister( digitalPinToPort(pin) ) & digitalPinToBitMask(pin)));
    }

  return PinState;
}

bool GetNextState(char *iPinNum) //gets the NEXT state of the specified pin number.
                                //Will return TRUE or FALSE or HIGH or LOW  :   Completed by Ryan Baas Date: 20170206
{
  bool NextPinState;
  NextPinState = !GetCurState(iPinNum);
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

void myDelay(int ix){
  for(int i=0; i<=ix; i++)
  {
    delayMicroseconds(1000);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////
//START SERIAL COMMAND HANDLERS
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

//END SERIAL COMMAND HANDLERS
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
