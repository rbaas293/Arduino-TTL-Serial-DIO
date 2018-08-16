#include "RunMode.h"
# define kModeNormDebounceMillis 250
# define kModeOpenDebounceMillis 2000

TPGRunMode::TPGRunMode(const uint8_t iMSBPin, const uint8_t iLSBPin )
{
	mMode = cUnknown;
	mMSBPin = iMSBPin;
	mLSBPin = iLSBPin;
	pinMode(iMSBPin, INPUT);
	pinMode(iLSBPin, INPUT);
	mOldModeValue = -1;
	mNewModeValue = -1;
}

TPGRunMode::~TPGRunMode(void)
{
}

TPGRunMode::eRunMode TPGRunMode::GetRunMode(void)
{
	return mMode;
}

// Return the 3 character mnemonic for the current run mode
void TPGRunMode::GetRunModeText3(char *oModeMnemonic)
{
	switch (mMode)
	{
		case cManual: 
			strcpy(oModeMnemonic, "MAN");
			break;
		case cPLC:
			strcpy(oModeMnemonic, "PLC");
			break;
		case cUSB:
			strcpy(oModeMnemonic, "USB");
			break;
		case cDiagnostic:
			strcpy(oModeMnemonic, "DIA");
			break;
		default:
			strcpy(oModeMnemonic, "N_A");
			break;
	}
}

// Updates the internal variables based on the current setting of the mode switch 
// Return:	false if the state did not change
//			    true if the state chaned
bool TPGRunMode::Refresh(void)
{
	int ValueMSB, ValueLSB, ModeValue;
	unsigned long CurrentMillis;
	bool StateChanged;

	StateChanged = false;
	ValueMSB = digitalRead(mMSBPin);
	ValueLSB = digitalRead(mLSBPin);
	ModeValue = (ValueMSB * 2) + ValueLSB;

	// apply a  "debounce" to make sure we are not in between switch changes. The actual switch is break before make.
	if (ModeValue == mOldModeValue)
	{
		mNewModeValue = -1;
		mModeChangeMillis = 0;
	}
	else
	{
		// mode has started to change
		if (ModeValue != mNewModeValue)
		{
			// mode is still changing so reset "debounce"
			mNewModeValue = ModeValue;
			mModeChangeMillis = millis();
			mDebounceMillis = kModeNormDebounceMillis;
			if (mNewModeValue == 0)
			{
				// because a slow switch change will cause an open, lets wait longer when recording an open
				mDebounceMillis = kModeOpenDebounceMillis;
			}
		}
		else
		{
			// same mode since last call so lets check time
			CurrentMillis = millis();
			if ((CurrentMillis < mModeChangeMillis) || ((CurrentMillis - mModeChangeMillis) > mDebounceMillis))
			{
				// clock rolled over or it has been more than 500 msec, so we have a new mode
				switch(mNewModeValue)
				{
					case 0:
						mMode = cDiagnostic;
						break;
					case 1:
						mMode = cUSB;
						break;
					case 2:
						mMode = cManual;
						break;
					case 3:
						mMode = cPLC;
						break;
				}
				mOldModeValue = mNewModeValue;
				mNewModeValue = -1;
				mModeChangeMillis = 0;
				StateChanged = true;
			}
		}
	}
	return StateChanged;
}
