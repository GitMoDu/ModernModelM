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
private:
	void (*WakeInterrupt)() = nullptr;

	volatile bool IrqFired = false;

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

	bool Setup(void (*onWakeInterrupt)())
	{
		WakeInterrupt = onWakeInterrupt;

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
		// Disable wake interrupt.
		ModelMKeyboardHidMapped::DisableKeyInterrupt();

		// If interrupt already fired, do not enter sleep.
		if (IrqFired)
		{
			return false;
		}

		// Setup wake interrupt.
		IrqFired = false;
		ModelMKeyboardHidMapped::AttachAnyKeyInterrupt(WakeInterrupt);

		// Enter sleep if no interrupt fired during setup.
		return !IrqFired;
	}


	/// <summary>
	/// Device has just woken up from sleep, restore peripherals.
	/// </summary>
	void OnWakeUp() final
	{
		IrqFired = true;

		// Disable wake interrupt.
		ModelMKeyboardHidMapped::DisableKeyInterrupt();

		// Clear keyboard state and restore input mode.
		ModelMKeyboardHidMapped::ClearState();
		ModelMKeyboardHidMapped::SetInputModeAll();

		//TODO: Notify manager about wake event.
	}
};
#endif
