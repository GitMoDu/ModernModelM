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

	void OnBackReport(uint8_t report_id,
		uint8_t report_type,
		uint8_t const* buffer, uint16_t bufsize) final
	{
		if (report_type == 0
			&& report_id == (uint8_t)RetroBle::HidBackReport::IdEnum::KeyboardLights
			&& bufsize == 1)
		{
			SetState(buffer[0]);

			TS::Task::enableIfNot();
		}
	}

	bool Callback() final
	{
		TS::Task::disable();

		KeyboardIndicator.SetModifiers(NumLockOn, CapsLockOn, ScrollLockOn);

		return true;
	}

private:
	void SetState(const uint8_t lightsBitMask)
	{
		NumLockOn = (lightsBitMask & KEYBOARD_LED_NUMLOCK) > 0;
		CapsLockOn = (lightsBitMask & KEYBOARD_LED_CAPSLOCK) > 0;
		ScrollLockOn = (lightsBitMask & KEYBOARD_LED_SCROLLLOCK) > 0;
	}
};

#endif