#ifndef TPGPulseStateMachine_h
#define TPGPulseStateMachine_h
#include "Arduino.h"

class TPGPulseStateMachine
{
	public:
		enum ePSMState
		{
			cPSMWaiting,        // = 0 
			cPSMTriggered,      // = 1
			cPSMPulseOutput,    // = 2
			cPSMReseting        // = 3
		};

		TPGPulseStateMachine(const uint8_t iPulsePin, const uint8_t iLEDPin, const int iResetTime0_1ms);
		~TPGPulseStateMachine(void);
		void Setup(void);
		ePSMState GetCurState(void);
		void TimerTick0_1ms(void);
		void Triggered(const int iPulseTime0_1ms);
		void SetOnResetFunc( void (*iFunc)(void) );

	protected:
		ePSMState mCurState;        //ePSMState is a user defined variable type. 
		int mResetTime0_1ms;
		uint8_t mPulsePin;
		uint8_t mLEDPin;
		int mCurTimer;
		void (*mOnResetFunc)(void);
		unsigned long mStartMicros;
		unsigned long mEndMicros;
    unsigned long mPulseDuration;
};
#endif //TPGPulseStateMachine_h

