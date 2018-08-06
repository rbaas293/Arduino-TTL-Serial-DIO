/*
 By: Ryan Baas
 Date: 02-02-18

 
This code is designed to make a arduino into a simple Serial DIO Controller. It is usefull for any application requiring TTL digital logic.
04-16-18: VERSION 1.0
*/ 

//Included Libraries:
#include <SerialCommand.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Servo.h>
#define kSerialBaudRate 9600

/*
 Defined below are the easy to read identifiers for our pins.
*/
//PINS 0 & 1 TO BE USED FOR SERIAL RX AND TX
  
#define S0 0        //D0  Serial Rx
#define S1 1        //D1  Serial Tx


//DEFINE OUTPUT AND INPUT ALIASES


//DEFAULT: PINS 2 & 3 TO BE USED AS NORMAL OUTPUTS:
#define P2 2        //D2  MULTI-PURPOSE
#define P3 3        //D3  MULTI-PURPOSE
bool ExtInterrupts = true; //defaut us 'false'. Change to 'true' if you would like to use pins 2 and 3 as external interupts:


//DEFAULTS
#define O4 4        //D4  Output 
#define O5 5        //D5  Output 
#define O6 6        //D6  Output
#define O7 7        //D7  Output  
#define I8 8        //D8  Input
#define I9 9        //D9  Input
#define I10 10      //D10 Input
#define I11 11      //D11 PWM Output
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
volatile byte P2_State, P3_State;
char gAck[4];
char gNack[5];
char gbuf[8];
char* gOutAliases[]={"S1", "P2", "P3", "O4", "O5"};
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
 //DEFINE OUTPUTS
 if (ExtInterrupts == true) //If external interrupts are used, then define them as inputs instead of outputs.
 {
  //SET UP EXT INTERUPTS
  pinMode(P2, INPUT);
  pinMode(P3, INPUT);
  P2_State = LOW;
  P3_State = LOW;
  attachInterrupt(digitalPinToInterrupt(P2), ISR2, FALLING); // SYNTAX: attachInterrupt(digitalPinToInterrupt(pin), <ISR-FUNCTION>, mode)
  attachInterrupt(digitalPinToInterrupt(P3), ISR3, CHANGE);
 }
 else
 {
  pinMode(P2, OUTPUT);
  pinMode(P3, OUTPUT);
 }
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

 
 Serial.println("READY");
sei();//allow interrupts
digitalWrite(P2, LOW);digitalWrite(P3, LOW);digitalWrite(O4, LOW);digitalWrite(O5, LOW);digitalWrite(O5, LOW);digitalWrite(O6, LOW);digitalWrite(O7, LOW); //INITALIZE OUTPUTS
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
if (P2_State == 0)

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

void ISR2() //Sends [  STCHG,<PIN-NUMBER>,<NEW STATE>  ]
{
  P2_State = !P2_State;
  Serial.print("STCHG,2,"); //Send STCHG. AKA STATE CHANGE
  Serial.println(P2_State);
  
}

void ISR3() //Sends [  STCHG,<PIN-NUMBER>,<NEW STATE>  ]
{
  P3_State = !P3_State;
  Serial.print("STCHG,3,"); //Send STCHG. AKA STATE CHANGE
  Serial.println(P3_State);
}

void SetupSerialCommands(void)
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

void ConfigurePins(void)
{
  //Future use.
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
/*
 * Serial.println(bitRead(PORTD,3)); //Reads bit 3 of register PORTD which contains the current state (high/low) of pin 3.
 */
  result = gNack;

  //Load command arguments into variables: just a little change
  OutNum = gSerialCommands.next();


  CurState = GetCurState(OutNum);
/*  DEBUG CURRENT STATE
  Serial.println();
  Serial.print("CurState = ");
  Serial.println(CurState);     */
  //Print command back to user
  Serial.println(OutNum);


  //--------------------------
    //Find out what our next state is going to be.
    NextState = GetNextState(OutNum);
/*  DEBUG NEXT STATE
    Serial.print("NextState = ");
    Serial.println(NextState);
    */

    //Check to see if arguments were in fact acceptable:
    if (NextState != CurState && OutNum != I9 && OutNum != I8 )
    {
       
      digitalWrite(atoi(OutNum), GetNextState(OutNum));
       
       /*
       //TOGGLE Desired Output:
       switch (atoi(OutNum))
      {
        case 1:
        digitalWrite(S1, GetNextState(OutNum));
          break;
        case 2:
        digitalWrite(P2, GetNextState(OutNum));
          break;
        case 3:
        digitalWrite(P3, GetNextState(OutNum));
          break;
        case 4:
        digitalWrite(O4, GetNextState(OutNum));
          break;
        case 5:
        digitalWrite(O5, GetNextState(OutNum));
          break;
      }
      */
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
    {
      switch (atoi(OutNum))
      {
        case 1:
          Serial.println(result);
          break;
        case 2:
          digitalWrite(P2, HIGH);
          delay(long(PulseTime));
          digitalWrite(P2, LOW);
          result = gAck;
          break;
        case 3:
          digitalWrite(P3, HIGH);
          delay(long(PulseTime));
          digitalWrite(P3, LOW);
          break;
          result = gAck;
        case 4:
          digitalWrite(O4, HIGH);
          delay(long(PulseTime));
          digitalWrite(O4, LOW);
          result = gAck;
          break;
        case 5:
          digitalWrite(O5, HIGH);
          delay(long(PulseTime));
          digitalWrite(O5, LOW);
          result = gAck;
          break;
      }
      
    }
    //Print result:
    Serial.print(result);
   /* if (result == gAck)       //if ack then, print ack and display Current State of pin:
    {
      Serial.print(",");
      Serial.println(PULSETIME MESURMENT); //This can be used as a confirmation that the pin got set HIGH.
    }   */
}

void GetPinStates(void) //gets the current state of each output PIN and displays it to the user.
{                       //CALLED BY 'STAT'   :     Completed by Ryan Baas Date: 20170206

  char *PinNum, *result;
  int j;
  //char* PinStatestxt[4];
  bool PinState;

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


bool GetCurState(char *iPinNum) //gets the current state of the specified output PIN.
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
  /*switch (atoi(iPinNum))
      {
        case 1:
        pin= S1;
        PinState = (0!=(*portOutputRegister( digitalPinToPort(pin) ) & digitalPinToBitMask(pin)));
          break;
        case 2:
        pin= P2;
        PinState = (0!=(*portOutputRegister( digitalPinToPort(pin) ) & digitalPinToBitMask(pin)));
          break;
        case 3:
        pin= P3;
       PinState = (0!=(*portOutputRegister( digitalPinToPort(pin) ) & digitalPinToBitMask(pin)));
          break;
        case 4:
        pin= O4;
        PinState = (0!=(*portOutputRegister( digitalPinToPort(pin) ) & digitalPinToBitMask(pin)));
          break;
        case 5:
        pin= O5;
        PinState = (0!=(*portOutputRegister( digitalPinToPort(pin) ) & digitalPinToBitMask(pin)));
          break;
      }
      */
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



void CmdUnknown(const char *iCommand) //Called on anything sent that is not defined as a command (ex. 'TPGTRIG')
{
  Serial.print("UNK,");
  Serial.println(iCommand);
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

void DebugPrint(void) //FUNCTION FOR SERIAL DEBUGING
{
  //Your Debugging Statments Here:
  //Serial.println("Put your DEBUGGING code in the function 'DebugPrint' and it will be displayed here.");
for (int i = 7;i<=8;i++)
{
  Serial.println();
  Serial.println(GetCurState(i2str(i,gbuf)));
}

// Old Debugging
/*  int pin = S1;
  bool PinState = (0!=(*portOutputRegister( digitalPinToPort(S1) ) & digitalPinToBitMask(S1)));
  Serial.println("");
  Serial.print("OUT");
  Serial.print("1");
  Serial.print(" Current State: ");
  Serial.println(PinState);
  */
}

void CmdQueryScanRate(void) //Called using 'SRT?'
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
