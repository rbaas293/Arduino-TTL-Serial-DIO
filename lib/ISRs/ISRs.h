#ifndef ISRs_h
#define ISRs_h

#include "Arduino.h"

bool O2_State; 
bool O3_State;

void ISR2(); //Sends [  STCHG,<PIN-NUMBER>,<NEW STATE>  ]
void ISR3(); //Sends [  STCHG,<PsIN-NUMBER>,<NEW STATE>  ]
void ISR4();
void ISR8();
void ISR9();
void ISR10();
void ISR11();
void ISR12();
void ISR13();



#endif
