#include "Trigger.h"


TPGDigitalTrigger::TPGDigitalTrigger(const uint8_t iPin, const eTriggerType iType)
{
	mPin = iPin;
	mType = iType;
	mPrevState = 0;
}


TPGDigitalTrigger::~TPGDigitalTrigger(void)
{
}

void TPGDigitalTrigger::Setup( void )
{
	pinMode(mPin, INPUT);
	mCurState = digitalRead(mPin);
	mPrevState = mCurState;
}

void TPGDigitalTrigger::Refresh(void)
{
	mCurState = digitalRead(mPin);

	// state changed
	if (mCurState != mPrevState) 
	{
		// may not have been cleared yet since last trigger 
		if (mTriggered == false)
		{
			// if the change match the type then set triggered
			if (mType == eTriggerRisingEdge) 
			{
				if ((mPrevState == LOW) && (mCurState == HIGH))
				{
					mTriggered = true;
					//Serial.println("Risingedge Triggered");
				}
			}
			else
			{
				if ((mPrevState == HIGH) && (mCurState == LOW))
				{
					mTriggered = true;
					//Serial.println("Fallingedge Triggered");
				}
			}
		}
		mPrevState = mCurState;
	}
}



TPGBasicTrigger::TPGBasicTrigger(void)
{
	mTriggered = false;
	mCurState = 0;
}

TPGBasicTrigger::~TPGBasicTrigger(void)
{
}

void TPGBasicTrigger::Setup(void)
{
}

void TPGBasicTrigger::Refresh(void)
{
}

void TPGBasicTrigger::SetTrigger(bool iTriggered)
{
	mTriggered = iTriggered;
}

void TPGBasicTrigger::ClearTrigger(void)
{
	mTriggered = false;
}

bool TPGBasicTrigger::Triggered(void)
{
	return mTriggered;
}
