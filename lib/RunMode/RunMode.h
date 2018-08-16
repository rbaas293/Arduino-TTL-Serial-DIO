#ifndef TPGRunMode_h
#define TPGRunMode_h
#include "Arduino.h"

class TPGRunMode
{
public:
	// listed in the order they are on the knob from left to right
	enum eRunMode
	{
		cPLC = 3,
		cManual = 2,
		cDiagnostic = 0,
		cUSB = 1,
		cUnknown = 4
	};

	TPGRunMode( const uint8_t iMSBPin, const uint8_t iLSBPin );
	~TPGRunMode(void);
	eRunMode GetRunMode(void);
	void GetRunModeText3(char *oModeMnemonic);
	bool Refresh(void);


protected:
	uint8_t mMSBPin;
	uint8_t mLSBPin;
	eRunMode mMode;
	unsigned long mModeChangeMillis;
	int mOldModeValue;
	int mNewModeValue;
	unsigned int mDebounceMillis;
};
#endif // TPGRunMode_h
