#include <SigPWM.h>


SigPWM(const uint8_t iPin, const uint8_t iFreq, const uint8_t iDuty, const double iStepsPerRev)
{
  mOutPin = iOutPin;
  mDisablePin = iDisablePin
  mDirecPin = iDirecPin
  mFreq = iFreq;
  mDuty = iDuty;
  mDutymicros = iDuty*(1/mFeq);
  mStepsPerRev = iStepsPerRev;

}

~SigPWM(void){

}

int Conv2Steps(double iDegrees){
//Converts input Degrees into Steps.
//This Depends on the Steps Per Rev of the motor. this example will use a 2000 Steps/Rev.
// StepsPerRev = {# of Steps to go 360 Degrees} / {Degrees Per Rev (I would hope this is 360)}
mStepsPerDeg = mStepsPerRev/360;
return iDegrees*mStepsPerRev;

}

void SetFreq(uint8_t iFreq, uint8_t iPeriod_ms){

  if (iFreq == NULL){
    mFreq = 1/iPeriod_ms;
    #ifdef DEBUG
      Serial.print("mFreq = %i set from Period.", mFreq)
    #endif
  }
}

void SetDuty(uint8_t iPercent0_100){

  mDuty = iPercent0_100;
  mDutymicros = mDuty*(1/mFeq);
}
