#ifndef TPGTrigger_h
#define TPGTrigger_h
#include "Arduino.h"

class TPGBasicTrigger
{
public:
	TPGBasicTrigger(void);
	~TPGBasicTrigger(void);

	void Setup(void);
	virtual void Refresh(void);
	virtual void ClearTrigger(void);
	virtual bool Triggered(void);
	void SetTrigger(bool iTriggered);

protected:
	int mCurState;
	bool mTriggered;
};




class TPGDigitalTrigger : public TPGBasicTrigger
{
public:
	enum eTriggerType
	{
		eTriggerRisingEdge,   //=0
		eTriggerFallingEdge   //=1
	};

	TPGDigitalTrigger(const uint8_t iPin, const eTriggerType iType);
	~TPGDigitalTrigger(void);

	void Setup(void);
	void Refresh(void);

protected:
	uint8_t mPin;
	eTriggerType mType;
	int mPrevState;
};

#endif // TPGTrigger_h
