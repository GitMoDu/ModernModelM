#ifndef _KEYBOARD_MAPPER_TASK_h
#define _KEYBOARD_MAPPER_TASK_h

#include <RetroBle.h>
#include <VirtualPad.h>

template<typename KeyboardType,
	const uint32_t PowerDownHoldDuration>
class KeyboardMapperTask : public HidKeyboardTask
{
private:
	KeyboardType& Driver;
	VirtualPad::ButtonParser::ActionTimed EscapeHold{};

public:
	KeyboardMapperTask(TS::Scheduler& scheduler,
		KeyboardType& driver,
		UsbHidKeyboard& usbKeyboard,
		BLEHidAdafruit& bleKeyboard,
		const uint32_t usbUpdatePeriod = 5,
		const uint32_t bleUpdatePeriod = 15)
		: HidKeyboardTask(scheduler, usbKeyboard, bleKeyboard, usbUpdatePeriod, bleUpdatePeriod)
		, Driver(driver)
	{}

public:
	virtual bool IsPowerDownRequested() const final
	{
		return EscapeHold.ActionDown(PowerDownHoldDuration);
	}

	/// <summary>
	/// Sleep peripheral and setup wake event.
	/// </summary>
	/// <returns>False if sleep isn't possible right now.</returns>
	virtual bool WakeOnInterrupt() final
	{
		return Driver.WakeOnInterrupt();
	}

	/// <summary>
	/// Device has just woken up from sleep, restore peripheral.
	/// </summary>
	virtual void OnWakeUp() final
	{
		EscapeHold.Clear();
		Driver.OnWakeUp();
		OnWakeInterrupt();
	}

protected:
	virtual void UpdateState(hid_keyboard_report_t& hidReport) final
	{
		Driver.GetReport(hidReport);

		EscapeHold.Parse(millis(), IsKeyCodeActive(hidReport));
	}

private:
	static const bool IsKeyCodeActive(hid_keyboard_report_t& hidReport)
	{
		for (uint_fast8_t i = 0; i < sizeof(hid_keyboard_report_t::keycode); i++)
		{
			if (hidReport.keycode[i] == HID_KEY_ESCAPE)
			{
				return true;
			}
		}

		return false;
	}
};

#endif

