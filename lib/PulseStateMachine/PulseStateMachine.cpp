#include "PulseStateMachine.h"


TPGPulseStateMachine::TPGPulseStateMachine(const uint8_t iPulsePin, const uint8_t iLEDPin, const int iResetTime0_1ms)
{
	mCurState = cPSMWaiting;
	mPulsePin = iPulsePin;
	mLEDPin = iLEDPin;
	mResetTime0_1ms = iResetTime0_1ms;
	mOnResetFunc = NULL;

}


TPGPulseStateMachine::~TPGPulseStateMachine(void)
{
}

void TPGPulseStateMachine::Setup(void)
{
	pinMode(mPulsePin, OUTPUT);
	digitalWrite(mPulsePin, LOW);
	pinMode(mLEDPin, OUTPUT);
	digitalWrite(mLEDPin, LOW);
}

TPGPulseStateMachine::ePSMState TPGPulseStateMachine::GetCurState(void)
{
	return mCurState;
}


void TPGPulseStateMachine::SetOnResetFunc( void (*iFunc)(void) )
{
	TPGPulseStateMachine::mOnResetFunc = iFunc;
}

void TPGPulseStateMachine::TimerTick0_1ms(void)
{
	switch (mCurState)
	{
		case cPSMWaiting:
			// waiting so do nothing
			break;
		case cPSMTriggered:
			// been triggered so lets start the pulse
			digitalWrite(mPulsePin, HIGH);
			mStartMicros = micros();
			digitalWrite(mLEDPin, HIGH);
			mCurState = cPSMPulseOutput;
			break;
		case cPSMPulseOutput:
			// if pulse done, turn off pulse and start reset
			mCurTimer--;
			if ( mCurTimer <= 0 ) 
			{
				digitalWrite(mPulsePin, LOW);
				mEndMicros = micros();
				mCurState = cPSMReseting;
				mCurTimer = mResetTime0_1ms;
				Serial.print("STM,");
				Serial.println(mStartMicros);
				Serial.print("ENM,");
				Serial.println(mEndMicros);
       /////////////////////////*///////////////////////////////////
        mPulseDuration = mEndMicros-mStartMicros;
        Serial.print("PD = ");                   //Added so we can compare the output pulse duration to the set duration.
        Serial.print(mPulseDuration/1000);   //us*(1ms/1000us) = ms
        Serial.println(" ms");
        ///////////////////////*////////////////////////////////////                  
			}
			break;
		case cPSMReseting:
			// if reset done, turn off LED and start waiting
			mCurTimer--;
			if ( mCurTimer <= 0 ) 
			{
				digitalWrite(mLEDPin, LOW);
				mCurState = cPSMWaiting;
				if (mOnResetFunc != NULL)
				{
					(*TPGPulseStateMachine::mOnResetFunc)();
				}
			}
			break;
		default:
			// if state gets messed up then reset it to waiting
			mCurState = cPSMWaiting;
			mCurTimer = 0;
			break;
	}
}

void TPGPulseStateMachine::Triggered(const int iPulseTime0_1ms)
{
	if ( mCurState == cPSMWaiting )
	{
		mCurTimer = iPulseTime0_1ms;
    //Serial.println();                     //for debugging 
    //Serial.print("iPulseTime0_1ms = ");
    //Serial.println(mCurTimer);
		mCurState = cPSMTriggered;
	}
}
