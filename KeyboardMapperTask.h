// KeyboardMapperTask.h

#ifndef _KEYBOARD_MAPPER_TASK_h
#define _KEYBOARD_MAPPER_TASK_h

#include <RetroBle.h>

template<const uint32_t PowerDownHoldDuration>
class KeyboardMapperTask : public HidKeyboardTask
{
private:
	ModelMSleepyKeyboard& Driver;
	//ButtonParser::ActionTimed EscapeHold{};

public:
	KeyboardMapperTask(TS::Scheduler& scheduler,
		ModelMSleepyKeyboard& driver,
		UsbHidKeyboard& usbKeyboard,
		BLEHidAdafruit& bleKeyboard,
		const uint32_t usbUpdatePeriod = 5,
		const uint32_t bleUpdatePeriod = 15)
		: HidKeyboardTask(scheduler, usbKeyboard, bleKeyboard, usbUpdatePeriod, bleUpdatePeriod)
		, Driver(driver)
	{}

public:
	virtual const bool IsPowerDownRequested() final
	{
		return true;
		//return EscapeHold.ActionDown(PowerDownHoldDuration);
	}

	/// <summary>
	/// Sleep peripheral and setup wake event.
	/// </summary>
	/// <returns>False if sleep isn't possible right now.</returns>
	virtual const bool WakeOnInterrupt() final
	{
		return Driver.WakeOnInterrupt();
	}

	/// <summary>
	/// Device has just woken up from sleep, restore peripheral.
	/// </summary>
	virtual void OnWakeUp() final
	{
		//EscapeHold.Clear();
		//Driver.GetHidReport();
		Driver.OnWakeUp();
	}

protected:
	virtual void UpdateState(hid_gamepad_report_t& hidReport) final
	{
		//Source.Read();

		//EscapeHold.Parse(millis(), Source.Start());

	
	}
};

#endif

