#ifndef TPGAnalogPulseTime_h
#define TPGAnalogPulseTime_h
#include "Arduino.h"

#define kRunAvgBufSize 10
// Using 5V reference for 10bit analog means 0.0048828125 V/AD
#define kVoltsPerAD 0.0048828125

class TPGBasicPulseTime
{
	public:
		TPGBasicPulseTime(void);
		~TPGBasicPulseTime(void);
		virtual int GetPulseTimeIn0_1ms(void);
		virtual void Refresh(void);
		void SetPulseTimeIn0_1ms(const int iPulseTime0_1ms);

	protected:
		int mPulseTime;
};

class TPGAnalogPulseTime : public TPGBasicPulseTime
{
	public:
		TPGAnalogPulseTime(const uint8_t iPin, const float i0msVoltage, const float i100msVoltage);
		~TPGAnalogPulseTime(void);
		virtual int GetPulseTimeIn0_1ms(void);
		virtual void Refresh(void);
		void InitializeAverage(void);

	protected:
		uint8_t mPin;
		float mConversionSpan;		// in ms/AD
		float mConversionOffset;	// in ms
		int mAtoDValues[kRunAvgBufSize];		// used for running average
		int mAtoDSum;								// sum of A to D values in the array
		uint8_t mNextAtoDLoc;					// next location in array for new value, also points to oldest
		uint8_t mNumAtoDValues;					// number of AtoD values in array

		void SetRange(const float i0msVoltage, const float i100msVoltage);


};

#endif // TPGAnalogPulseTime_h

