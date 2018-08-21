/*
Define your pin ALIASES here.
*/
#ifndef PinDef_h
#define PinDef_h

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

#endif
