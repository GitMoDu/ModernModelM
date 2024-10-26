// ModelMSleepyKeyboard.h

#ifndef _MODELMSLEEPYKEYBOARD_h
#define _MODELMSLEEPYKEYBOARD_h

#include <RetroBle.h>
#include "ModelMDriver.h"

class ModelMSleepyKeyboard : public virtual BatteryManager::ISleep, public ModelMDriver
{
public:
	ModelMSleepyKeyboard(IoPort* io1, IoPort* io2)
		: BatteryManager::ISleep()
		, ModelMDriver(io1, io2)
	{
	}

public:
	/// <summary>
	/// Sleep peripheral and setup wake event.
	/// </summary>
	/// <returns>False if sleep isn't possible right now.</returns>
	virtual const bool WakeOnInterrupt() final
	{
		return true;
	}

	/// <summary>
	/// Device has just woken up from sleep, restore peripheral.
	/// </summary>
	virtual void OnWakeUp() final
	{ }
};

#endif

