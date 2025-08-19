#ifndef _KEYBOARDMODIFIERLIGHTSTASK_h
#define _KEYBOARDMODIFIERLIGHTSTASK_h

#include <RetroBle.h>

class KeyboardModifierLightsTask : public RetroBle::HidBackReport::IListener, private TS::Task
{
private:
	IKeyboardIndicator& KeyboardIndicator;

private:
	volatile bool NumLockOn = false;
	volatile bool CapsLockOn = false;
	volatile bool ScrollLockOn = false;

public:
	KeyboardModifierLightsTask(TS::Scheduler& scheduler,
		IKeyboardIndicator& keyboardIndicator)
		: TS::Task(TASK_IMMEDIATE, TASK_FOREVER, &scheduler, false)
		, KeyboardIndicator(keyboardIndicator)
	{
	}

	void OnBackReport(RetroBle::HidBackReport::SourceEnum backReportSource,
		uint8_t report_id,
		hid_report_type_t report_type,
		uint8_t const* buffer, uint16_t bufsize) final
	{
		if (report_id == (uint8_t)RetroBle::HidBackReport::IdEnum::KeyboardLights
			&& bufsize == sizeof(uint8_t))
		{
			const uint8_t ledState = buffer[0]; // bitmask for NumLock, CapsLock, ScrollLock.
			NumLockOn = ledState & KEYBOARD_LED_NUMLOCK;
			CapsLockOn = ledState & KEYBOARD_LED_CAPSLOCK;
			ScrollLockOn = ledState & KEYBOARD_LED_SCROLLLOCK;

			TS::Task::enableIfNot();
		}
	}

	bool Callback() final
	{
		TS::Task::disable();

		KeyboardIndicator.SetModifiers(NumLockOn, CapsLockOn, ScrollLockOn);

		return true;
	}
};

#endif