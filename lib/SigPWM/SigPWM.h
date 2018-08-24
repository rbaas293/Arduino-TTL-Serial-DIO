#ifndef SIGPWM_H
#define SIGPWM_H

#include <Arduino.h>
#include <sdfat.h>
class SigPWM
{
public:
SigPWM(const uint8_t iOutPin, const double iStepsPerRev, const uint8_t iDisablePin, const uint8_t iDirecPin, const uint8_t iFreq, const uint8_t iDuty);
~SigPWM(void);

int Degree2Steps(double iDegrees);
void Move(double iDegrees);
void SetFreq(uint8_t iFreq, uint8_t iPeriod_ms); //choose one 
uint8_t GetFreq(void);
void SetDuty(uint8_t iPercent0_100);
void GetDuty(void);
void GetDutymicros(void)

//bool GreaterThan100();
protected:
uint8_t mOutPin, mDisablePin, mDirecPin, mFreq, mPeriod, mDuty, mDutymicros;
double mStepsPerRev;
double mStepsPerDeg;
double mPositonD;
unsigned long mStepCount;
};

#endif
