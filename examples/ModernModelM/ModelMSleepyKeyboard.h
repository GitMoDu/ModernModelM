#ifndef _MODELM_SLEEPY_KEYBOARD_h
#define _MODELM_SLEEPY_KEYBOARD_h

#include <RetroBle.h>
#include <ModelMKeyboardHidMap.h>

/// <summary>
/// </summary>
/// <typeparam name="PinIo1">Enum class with RST, IRQA and IRQB pins.</typeparam>
/// <typeparam name="PinIo2">Enum class with RST, IRQA and IRQB pins.</typeparam>
template<typename PinIo1,
	typename PinIo2>
class ModelMSleepyKeyboard : public BatteryManager::ISleep, public ModelMKeyboardHidMapped
{
public:
	ModelMSleepyKeyboard(IoPort* io1, IoPort* io2,
		const uint16_t* columnMap,
		const uint8_t columnMapSize,
		const uint16_t* rowMap,
		const uint8_t rowMapSize)
		: BatteryManager::ISleep()
		, ModelMKeyboardHidMapped(io1, io2, columnMap, columnMapSize, rowMap, rowMapSize)
	{
	}

	virtual bool Setup() final
	{
		SetupPins();

		if (ModelMKeyboardHidMapped::Setup())
		{
			return true;
		}

		return false;
	}

public:
	/// <summary>
	/// Sleep peripheral and setup wake event.
	/// </summary>
	/// <returns>False if sleep isn't possible right now.</returns>
	bool WakeOnInterrupt() final
	{
		return false;
	}

	/// <summary>
	/// Device has just woken up from sleep, restore peripheral.
	/// </summary>
	void OnWakeUp() final
	{
	}

private:
	void SetupPins()
	{
		pinMode((uint8_t)PinIo1::RST, INPUT);
		pinMode((uint8_t)PinIo1::IRQA, INPUT);
		pinMode((uint8_t)PinIo1::IRQB, INPUT);

		pinMode((uint8_t)PinIo2::RST, INPUT);
		pinMode((uint8_t)PinIo2::IRQA, INPUT);
		pinMode((uint8_t)PinIo2::IRQB, INPUT);
	}
};
#endif
