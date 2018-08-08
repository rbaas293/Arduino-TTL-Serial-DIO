#include "ISRs.h"

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
