#include <SigPWM.h>


SigPWM(const uint8_t iPin, const uint8_t iFreq, const uint8_t iDuty, const double iStepsPerRev)
{
  mOutPin = iOutPin;
  mDisablePin = iDisablePin
  mDirecPin = iDirecPin
  mFreq = iFreq;
  mDuty = iDuty;
  mStepsPerRev = iStepsPerRev;
}

~SigPWM(void){

}

int Conv2Steps(double iDegrees){
//Converts input Degrees into Steps.
//This Depends on the Steps Per Rev of the motor. this example will use a 2000 Steps/Rev.
// StepsPerRev = {# of Steps to go 360 Degrees} / {Degrees Per Rev (I would hope this is 360)}

Return iDegrees*mStepsPerRev;

}
