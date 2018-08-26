#ifndef SIGPWM_H
#define SIGPWM_H

#include <Arduino.h>
#include <SdFat.h>
#include <EEPROMex.h>
class SigPWM
{
public:
SigPWM(const uint8_t iOutPin, double iStepsPerRev, const uint8_t iDisablePin, const uint8_t iDirecPin, uint8_t iFreq, uint8_t iDuty);
~SigPWM(void);

void Save2EEPROM(void);
void ReadEEPROM(void);
int Degree2Steps(double iDegrees);
void Move(double iDegrees);
void SetDirection(bool iDirection);
bool GetCurDirection(void);
void SetFreq(uint8_t iFreq, uint8_t iPeriod_ms); //choose one
uint8_t GetFreq(void);
uint8_t GetPeriod(void);
void SetDuty(uint8_t iPercent0_100);
uint8_t GetDuty(void);
uint8_t GetDutymicros(void);

void SetStepsPerRev(uint8_t iStepsPerRev);
double GetStepsPerRev(void);
double GetStepsPerDeg(void);
unsigned long GetCurStepCount(void);
double GetCurDegs(void);
void SetHome(void);
void SetSaveState(bool i);

//bool GreaterThan100();
protected:
uint8_t mOutPin, mDisablePin, mDirecPin, mFreq, mPeriod_us, mDuty, mDutymicros, mStepsPerRev;
bool mDirection; //low = clockwise High = counter-clockkwise?? need to check.
bool mSaveState;
double mStepsPerDeg;
double mCurPositionDeg;
unsigned long mStepCount; //Gloabl Step count
//long AbsPosition;
};



#endif
