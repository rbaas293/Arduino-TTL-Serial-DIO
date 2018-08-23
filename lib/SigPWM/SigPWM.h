#ifndef SIGPWM_H
#define SIGPWM_H

#include <Arduino.h>

class SigPWM
{
public:
SigPWM(const uint8_t iOutPin, const double iStepsPerRev, const uint8_t iDisablePin, const uint8_t iDirecPin, const uint8_t iFreq, const uint8_t iDuty);
~SigPWM(void);

int Conv2Steps(double iDegrees);
void SetFreq(uint8_t iFreq, uint8_t iPeriod_ms);
void SetDuty(uint8_t iPercent0_100);
//bool GreaterThan100();
protected:
uint8_t mOutPin, mDisablePin, mDirecPin, mFreq, mDuty, mDutymicros;
double mStepsPerRev; 
double mStepsPerDeg;


};

#endif
