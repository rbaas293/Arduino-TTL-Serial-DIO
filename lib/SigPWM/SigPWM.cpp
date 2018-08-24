#include <SigPWM.h>


SigPWM(const uint8_t iPin, const uint8_t iFreq, const uint8_t iDuty, const double iStepsPerRev)
{
  mOutPin = iOutPin;
  mDisablePin = iDisablePin
  mDirecPin = iDirecPin
  mFreq = iFreq;
  mPeriod = 1/iFreq;
  mDuty = iDuty;
  mDutymicros = iDuty*mPeriod;
  mStepsPerRev = iStepsPerRev;

}

~SigPWM(void){

}

int Degree2Steps(double iDegrees){
//Converts input Degrees into Steps.
//This Depends on the Steps Per Rev of the motor. this example will use a 2000 Steps/Rev.
// StepsPerDeg = {# of Steps to go 360 Degrees} / {Degrees Per Rev (I would hope this is 360)}
mStepsPerDeg = mStepsPerRev/360;
return iDegrees*mStepsPerDeg;

}

void Move(double iDegrees)
{
  int iSteps = Degree2Steps(iDegrees)
  for (int i = 0; i <= atoi(iSteps); i += 1)  // goes from 0 degrees to 180 degrees
    {
    digitalWrite(atoi(mPin), HIGH);
    delayMicroseconds(mDutyMicros); // if = 1 ... then, Approximately 1% duty cycle @ 10KHz
    digitalWrite(atoi(mPin), LOW);
    delayMicroseconds(100 - mDutyMicros); // if = 100-1
    }

    Serial.println("DONE");

}

void SetFreq(uint8_t iFreq, uint8_t iPeriod_us){

  if (iFreq == NULL){
    mFreq = 1/iPeriod_ms;
    #ifdef DEBUG
      Serial.println("mFreq = %i set from Period.", mFreq)
    #endif
  }
  else{
    mFreq = iFreq;
    SetDuty(mDuty); //Needed to update mD
  }
}

uint8_t GetFreq(void){
  return mFreq;
}



void SetDuty(uint8_t iPercent0_100){

  mDuty = iPercent0_100;
  mDutymicros = mDuty*(1/mFeq);
}
