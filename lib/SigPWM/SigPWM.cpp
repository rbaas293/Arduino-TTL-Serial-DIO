#include <SigPWM.h>


SigPWM::SigPWM(const uint8_t iOutPin, double iStepsPerRev, const uint8_t iDisablePin, uint8_t iDirecPin, uint8_t iFreq, uint8_t iDuty)
{
  mOutPin = iOutPin;
  mDisablePin = iDisablePin;
  mDirecPin = iDirecPin;
  mDirection = 0;
  mFreq = iFreq;
  mPeriod_us = (10^6)*(1/iFreq);
  mDuty = iDuty;
  mDutymicros = (10^-2)*iDuty*mPeriod_us;
  mStepsPerRev = iStepsPerRev;
  mCurPositionDeg = 0;
}

SigPWM::~SigPWM(void){

}

void SigPWM::Save2EEPROM(void){
//Save all member variables to EEPROM ino order to save state incase a reset occurs
int address=0;
EEPROM.writeDouble(address, mCurPositionDeg);
address += sizeof(mCurPositionDeg);
EEPROM.updateLong(address, mStepCount);
address += sizeof(mStepCount);

EEPROM.updateDouble(address, mStepsPerRev);
address += sizeof(mStepsPerRev);
EEPROM.update(address, mFreq);
address += sizeof(mFreq);
EEPROM.update(address, mDuty);
address += sizeof(mDuty);
}

void SigPWM::ReadEEPROM(void){
// Read all back into the stack.

;; //later will prolly need to make this go in main cpp

}

int SigPWM::Degree2Steps(double iDegrees){
//Converts input Degrees into Steps.
//This Depends on the Steps Per Rev of the motor. this example will use a 2000 Steps/Rev.
// StepsPerDeg = {# of Steps to go 360 Degrees} / {Degrees Per Rev (I would hope this is 360)}
mStepsPerDeg = mStepsPerRev/360;
//mDegPerStep = 1/mStepsPerDeg;
return iDegrees*mStepsPerDeg;

}

void SigPWM::Move(double iDegrees) // goes to the specified degrees between 0 and 360
{
  int iSteps = Degree2Steps(iDegrees);
  #ifdef DEBUG
    Serial.print("DBUG: iDegrees = ");
    Serial.println(iDegrees);
    #endif
  for (int i = 0; i <= iSteps; i += 1)  // goes from 0 degrees to 180 degrees
    {
    digitalWrite(mOutPin, HIGH);
    delayMicroseconds(mDutymicros); // if = 1 ... then, Approximately 1% duty cycle @ 10KHz
    digitalWrite(mOutPin, LOW);
    delayMicroseconds(mPeriod_us - mDutymicros); // if = 100-1
    mStepCount++;
    mCurPositionDeg += (1/mStepsPerDeg);
    }


    Serial.print("New Position is ");
    Serial.println(mCurPositionDeg);
    Serial.println("DONE");

}

void SigPWM::SetDirection(bool iDirection){
    mDirection = iDirection;
    digitalWrite(mDirecPin, mDirection);
}

bool SigPWM::GetCurDirection(void){
    return mDirection;
}

void SigPWM::SetFreq(uint8_t iFreq, uint8_t iPeriod_us){

  if (iFreq == -1){
    mFreq = 1/iPeriod_us;
    #ifdef DEBUG
      Serial.println("mFreq = %i set from Period.", mFreq);
    #endif
  }
  else{
    mFreq = iFreq;
    mPeriod_us = 1/iFreq;
    SetDuty(mDuty); //Needed to update mDutymicros
  }
}

uint8_t SigPWM::GetFreq(void){
  return mFreq;
}
<<<<<<< HEAD
unsigned int SigPWM::GetPeriod(void){
=======
uint8_t SigPWM::GetPeriod(void){
>>>>>>> b4baf963926d5dd029753b88d28ed6dbc0a709ad
  return mPeriod_us;
}

void SigPWM::SetDuty(uint8_t iPercent0_100){

  mDuty = iPercent0_100;
  mDutymicros = mDuty*mPeriod_us;
}

uint8_t SigPWM::GetDuty(void){
  return mDuty;
}

uint8_t SigPWM::GetDutymicros(void){
  return  mDutymicros;
}

void SigPWM::SetStepsPerRev(uint8_t iStepsPerRev){
    mStepsPerRev = iStepsPerRev;
}

double SigPWM::GetStepsPerRev(void){
    return mStepsPerRev;
}

double SigPWM::GetStepsPerDeg(void){
    return mStepsPerDeg;
}

unsigned long SigPWM::GetCurStepCount(void){
    return mStepCount; // in steps
}

double SigPWM::GetCurDegs(void){
    return mCurPositionDeg;
}

void SigPWM::SetHome(void){
// Resets the Position of to 0 and makes the current location the home. cannot be called while moving.
mCurPositionDeg = 0;
mStepCount = 0;
}

void SigPWM::SetSaveState(bool i){
mSaveState = i;
}
