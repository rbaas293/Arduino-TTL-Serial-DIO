#include "ISRs.h"

bool O2_State;
bool O3_State;
void ISR2() //Sends [  STCHG,<PIN-NUMBER>,<NEW STATE>  ]
{
  O2_State = !O2_State;
  Serial.print("STCHG,2,"); //Send STCHG. AKA STATE CHANGE
  Serial.println(O2_State);
}

void ISR3() //Sends [  STCHG,<PIN-NUMBER>,<NEW STATE>  ]
{
  O3_State = !O3_State;
  Serial.print("STCHG,3,"); //Send STCHG. AKA STATE CHANGE
  Serial.println(O3_State);
}

void ISR4()
{
// Placeholder

}
void ISR8()
{
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(pinTick));
    if(trigger == RISING)
      // Do something
    else if(trigger == FALLING)
      // Do something
    else
      // Wrong usage (trigger == CHANGE)

}
void ISR9()
{
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(pinTick));
    if(trigger == RISING)
      // Do something
    else if(trigger == FALLING)
      // Do something
    else
      // Wrong usage (trigger == CHANGE)

}
void ISR10()
{
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(pinTick));
    if(trigger == RISING)
      // Do something
    else if(trigger == FALLING)
      // Do something
    else
      // Wrong usage (trigger == CHANGE)

}
void ISR11()
{
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(pinTick));
    if(trigger == RISING)
      // Do something
    else if(trigger == FALLING)
      // Do something
    else
      // Wrong usage (trigger == CHANGE)

}
void ISR12()
{
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(pinTick));
    if(trigger == RISING)
      // Do something
    else if(trigger == FALLING)
      // Do something
    else
      // Wrong usage (trigger == CHANGE)

}
void ISR13()
{
    uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(pinTick));
    if(trigger == RISING)
      // Do something
    else if(trigger == FALLING)
      // Do something
    else
      // Wrong usage (trigger == CHANGE) 
}
