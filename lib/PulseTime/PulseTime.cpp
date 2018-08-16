#include "PulseTime.h"


TPGAnalogPulseTime::TPGAnalogPulseTime(const uint8_t iPin, const float i0msVoltage, const float i100msVoltage) //(input pin, start Voltage, end voltage)
{
	mPin = iPin;
	mPulseTime = 0;
	InitializeAverage();

	SetRange(i0msVoltage, i100msVoltage);
}

TPGAnalogPulseTime::~TPGAnalogPulseTime(void)
{
}

void TPGAnalogPulseTime::InitializeAverage(void)
{
	for (int i = 0; i < kRunAvgBufSize; i++)
	{
		mAtoDValues[i] = 0;
	}

	mAtoDSum = 0;
	mNextAtoDLoc = 0;
	mNumAtoDValues = 0;
}


// Sets the range so that the voltages specified are scalled to 0 to 100ms  
void TPGAnalogPulseTime::SetRange(const float i0msVoltage, const float i100msVoltage)
{
	float fTimePerVoltConv;
	// AtoD is 0 to 1023
	// Using 5V reference for analog means 0.0048828125 V/AD
	// distribute 100ms or 1000 0.1ms over selected voltage range = 1000 / (i100msVoltage - i0msVoltage) = ms/V
	// span = 0.0048828125 V/AD * 0.1ms/V = 0.1ms/AD
	// offset = i0msVoltage / 0.0048828125 V/AD = AD
	fTimePerVoltConv = 1000.0  / ( i100msVoltage - i0msVoltage );
	mConversionSpan = kVoltsPerAD *  fTimePerVoltConv;
	mConversionOffset = i0msVoltage / kVoltsPerAD;
}

void TPGAnalogPulseTime::Refresh(void)
{
	//Remove Oldest
	mAtoDSum = mAtoDSum - mAtoDValues[mNextAtoDLoc];

	// Read the current value
	mAtoDValues[mNextAtoDLoc] = analogRead(mPin);
	mAtoDSum = mAtoDSum + mAtoDValues[mNextAtoDLoc];

	// Adjust for next
	mNextAtoDLoc++;
	mNumAtoDValues++;
	if (mNextAtoDLoc >= kRunAvgBufSize) 
	{
		mNextAtoDLoc = 0;
	}
	if (mNumAtoDValues > kRunAvgBufSize) 
	{
		mNumAtoDValues = kRunAvgBufSize;
	}
}

int TPGAnalogPulseTime::GetPulseTimeIn0_1ms(void)
{
	int pulseTime;
	float fAvgAtoD;
	fAvgAtoD = mAtoDSum / mNumAtoDValues;
	pulseTime = ( fAvgAtoD  - mConversionOffset) * mConversionSpan ;
	if ( pulseTime <= 0 ) 
	{
		pulseTime = 1;
	}
	if ( pulseTime >= 1000 )
	{
		pulseTime = 1000;
	}
	return pulseTime;
}

TPGBasicPulseTime::TPGBasicPulseTime(void)
{
	mPulseTime = 300;
}

TPGBasicPulseTime::~TPGBasicPulseTime(void)
{
}

int TPGBasicPulseTime::GetPulseTimeIn0_1ms(void)
{
	return mPulseTime;
}

void TPGBasicPulseTime::SetPulseTimeIn0_1ms(const int iPulseTime0_1ms)
{
	mPulseTime = iPulseTime0_1ms;
 /*////////////////////////////////////////////
 // Use this block of code to print out each time SetPulseTimeIn0_1ms is called.
  Serial.println(" ");              
  Serial.print("SetPluseTime = ");
  Serial.println(mPulseTime);
 /////////////////////////////////////////// */
	if ( iPulseTime0_1ms > 1000 ) mPulseTime = 1000;
	if ( iPulseTime0_1ms < 1 ) mPulseTime = 1;
}

void TPGBasicPulseTime::Refresh(void)
{
	;
}
