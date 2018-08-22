#ifndef SIGPWM_H
#define SIGPWM_H

#include <Arduino.h>

Class SigPWM
{
Public:
SigPWM(const uint8_t iOutPin, const uint8_t iDisablePin, const uint8_t iDirecPin, const uint8_t iFreq, const uint8_t iDuty);
~SigPWM(void);

int Conv2Steps(double iDegrees);
void

Protected:
uint8_t mOutPin, mDisablePin, mDirecPin, mFreq, mDuty;
double mStepsPerRev;


}
