/*
 By: Ryan Baas
 Date: 02-02-18


This code is designed to make a arduino into a simple Serial DIO Controller. It is usefull for any application requiring TTL digital logic.
04-16-18: VERSION 1.0
*/

//Included Libraries:
#include <Arduino.h>
#include <SerialCommand.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Servo.h>
#include <"D:\Users\ryanbaas\OneDrive - Signalysis, Inc\Documents\mygit\public-github\Arduino\Arduino-TTL-Serial-DIO\lib\Commands.h">
#include <"D:\Users\ryanbaas\OneDrive - Signalysis, Inc\Documents\mygit\public-github\Arduino\Arduino-TTL-Serial-DIO\lib\ISRs.h">
#define kSerialBaudRate 9600

/*
 Defined below are the easy to read identifiers for our pins.
*/
//PINS 0 & 1 TO BE USED FOR SERIAL RX AND TX

#define S0 0        //D0  Serial Rx
#define S1 1        //D1  Serial Tx


//DEFINE OUTPUT AND INPUT ALIASES

//DEFAULT: PINS 2 & 3 TO BE USED AS NORMAL OUTPUTS:
#define O2 2        //D2  MULTI-PURPOSE
#define O3 3        //D3  MULTI-PURPOSE
bool AnalogAsDigital = False; //defaut us 'false'. Change to 'true' if you would like to use Analog pins as Digital:


//DEFAULTS
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


//Analoge Pins:
  //CAN BE REFERED TO BY 'A_' WHERE '_' IS A THE ANALOGE INPUT NUMBER
  // DEFINE THERE ALIASES HERE

//Initilize Objects:
SerialCommand gSerialCommands;

//Set Up Functions/Methods:
char GetCurState(int iPinNum);    //gets the CURRENT state of the specified pin number.
bool GetNextState(char *iPinNum); //gets the NEXT state of the specified pin number.
char *i2str(int i, char *buf);    //int to string fuction, cuz sting(int) would not work.

//Set Up Variables and Arrays
unsigned long gScanRate;
unsigned long gScanRateMin;
unsigned long gScanRateMax;
unsigned long ScanStartMicros;
volatile byte O2_State, O3_State;
char gAck[4];
char gNack[5];
char gbuf[8];
char* gOutAliases[]={"S1", "O2", "O3", "O4", "O5"};
char* gInAliases[]={"I8", "I9", "I10","I11","I12","I3"};
char* gCharPins[]={"0","1","2","3","4","5","6","7","8","9","10","11","12","13"};

void setup() {
// put your setup code here, to run once:
cli();//stop interrupts
//DEFAULT CONFIGURATION

 //Set Up Objects:
 SetupSerialCommands();
 //Set inital values
 gScanRateMin = 999999;
 gScanRateMax = 0;
 strcpy(gAck, "ACK");
 strcpy(gNack, "NACK");
 //DEFINE Anolog Pins
 /*if (AnalogAsDigital == true) //If this flag is set, then define A0-A5 as Digital instead of Analog.
 {
  //SET UP EXT INTERUPTS
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  O2_State = LOW;
  O3_State = LOW;
  attachInterrupt(digitalPinToInterrupt(O2), ISR2, FALLING); // SYNTAX: attachInterrupt(digitalPinToInterrupt(pin), <ISR-FUNCTION>, mode)
  attachInterrupt(digitalPinToInterrupt(O3), ISR3, CHANGE);
 }
 else
 {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
 }
 */
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
 pinMode(I13,INPUT

PCintPort::attachInterrupt(I8, ISR8, CHANGE);
PCintPort::attachInterrupt(I9, ISR9, CHANGE);
PCintPort::attachInterrupt(I10, ISR10, CHANGE);
PCintPort::attachInterrupt(I11, ISR11, CHANGE);
PCintPort::attachInterrupt(I12, ISR12, CHANGE);
PCintPort::attachInterrupt(I13, ISR13, CHANGE);


 Serial.println("READY");
sei();//allow interrupts
digitalWrite(O2, LOW);digitalWrite(O3, LOW);digitalWrite(O4, LOW);digitalWrite(O5, LOW);digitalWrite(O5, LOW);digitalWrite(O6, LOW);digitalWrite(O7, LOW); //INITALIZE OUTPUTS
}

void loop() {
  // put your main code here, to run repeatedly:
ScanStartMicros = micros();

//Future Use
 /* has a new command been set?
    if ( (gTriggerManual.Triggered() == true) || (gTriggerPLC.Triggered() == true) || (gTriggerUSB.Triggered() == true) )
    {
      pulseLenIn0_1Ms = gActivePulseTime->GetPulseTimeIn0_1ms();
      gRunMode.GetRunModeText3(strRunMode);
      gDiagLedMode = false;
      Serial.print(" MOD,");
      Serial.println(strRunMode);
      Serial.print("TPT,");
      Serial.println( pulseLenIn0_1Ms / 10.0);

      gPulseStateMachine.Triggered(pulseLenIn0_1Ms);
    }
  }
*/
if (O2_State == 0)

 // check for serial activity
  gSerialCommands.readSerial();
  gScanRate = ElapsedTime(ScanStartMicros, micros());
  if (gScanRate < gScanRateMin) gScanRateMin = gScanRate;
  if (gScanRate > gScanRateMax) gScanRateMax = gScanRate;
}

/*char GetSerialCommandTxt(char cmd, char arg1, char arg2)
{

}
*/

/*void SetupSerialCommands(void)
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
}*/


bool GetCurState(char *iPinNum) //gets the current state of the specified PIN.
{
  /* What Made This Possible...
   * Serial.println(bitRead(PORTD,3)); //Reads bit 3 of register PORTD which contains the current state (high/low) of pin 3.
   */
  int pin;
  bool PinState;

  pin = atoi(iPinNum);

  //Serial.println();
  //Serial.print(I9);
  //Serial.print("   ");
  //Serial.println(I8);
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

/* Calculates the time elapsed between the start time and the current time
 Assumes the start should always be before the current and then compensates
 if the milli or micro clock has rolled over    */
unsigned long ElapsedTime(unsigned long iStartTime, unsigned long iCurTime)
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
