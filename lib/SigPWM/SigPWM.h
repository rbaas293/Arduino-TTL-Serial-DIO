#ifndef SIGPWM_H
#define SIGPWM_H

#include <Arduino.h>

//#include <SdFat.h>

#include <EEPROMex.h>
class SigPWM
{
public:
SigPWM(const int iOutPin, double iStepsPerRev, const int iDisablePin, const int iDirecPin, int iFreq, int iDuty);
~SigPWM(void);

void Save2EEPROM(void);
void ReadEEPROM(void);
int Degree2Steps(double iDegrees);
void Move(double iDegrees);
void MoveTo(double iDegrees);
void SetDirection(bool iDirection);
bool GetCurDirection(void);
void SetFreq(int iFreq, int iPeriod_ms); //choose one
int GetFreq(void);
double GetPeriod(void);
void SetDuty(int iPercent0_100);
double GetDuty(void);
double GetDutymicros(void);

void SetStepsPerRev(int iStepsPerRev);
double GetStepsPerRev(void);
double GetStepsPerDeg(void);
unsigned long GetCurStepCount(void);
double GetCurDegs(void);
void SetHome(void);
void SetSaveState(bool i);

//bool GreaterThan100();
protected:
int mOutPin, mDisablePin, mDirecPin, mFreq, mStepsPerRev;
bool mDirection; //low = clockwise High = counter-clockkwise?? need to check.
bool mSaveState;
double mDuty;
double mDutymicros;
double mStepsPerDeg;
double mCurPositionDeg;
double mPeriod_us;
unsigned long mStepCount; //Gloabl Step count
long mAbsPositionDeg;
long mAbsPositionStep;
};



#endif
