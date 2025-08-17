// KeyboardLightsAnimator.h

#ifndef _KEYBOARD_LIGHTS_ANIMATOR_TASK_h
#define _KEYBOARD_LIGHTS_ANIMATOR_TASK_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

#include <KeyboardLights.h>
#include "BrightnessAnimator.h"


/// <summary>
/// </summary>
/// <typeparam name="Pin">enum class ExamplePin{NumLock = 1, CapsLock = 2, ScrollLock = 3}</typeparam>
/// <typeparam name="onState"></typeparam>
template<uint8_t brightnessRange,
	typename Pin,
	bool onState = LOW>
class KeyboardLightsAnimator : public virtual KeyboardLights::IKeyboardLights, private TS::Task
{
private:
	static constexpr uint16_t BLE_SEARCHING_ANIMATE_PERIOD = 450;
	static constexpr uint16_t BLE_SEARCHING_LED_OFFSET = (BLE_SEARCHING_ANIMATE_PERIOD * 1) / 3;
	//static constexpr uint16_t BLE_SEARCHING_LED_THRESHOLD = ((uint32_t)BLE_SEARCHING_ANIMATE_PERIOD * 45) / 100;

private:
	uint32_t AnimationStart = 0;

private:
	KeyboardLights::ModeEnum Mode = KeyboardLights::ModeEnum::Off;
	bool NumLock = false;
	bool CapsLock = false;
	bool ScrollLock = false;

private:
	BrightnessAnimator<brightnessRange, Pin, onState> Driver;


public:
	KeyboardLightsAnimator(TS::Scheduler& scheduler,
		const uint32_t updatePeriod = 10)
		: TS::Task(updatePeriod, TASK_FOREVER, &scheduler, false)
		, Driver(scheduler, 2)
	{
	}

	void Start()
	{
		Driver.Start();
		/*pinMode((uint8_t)Pin::NumLock, OUTPUT);
		digitalWrite((uint8_t)Pin::NumLock, IoState(false));

		pinMode((uint8_t)Pin::CapsLock, OUTPUT);
		digitalWrite((uint8_t)Pin::CapsLock, IoState(false));

		pinMode((uint8_t)Pin::ScrollLock, OUTPUT);
		digitalWrite((uint8_t)Pin::ScrollLock, IoState(false));*/
	}

	void Stop()
	{
		Driver.Stop();
		//SetLed(false, false, false);
		//pinMode((uint8_t)Pin::NumLock, INPUT);
		//pinMode((uint8_t)Pin::CapsLock, INPUT);
		//pinMode((uint8_t)Pin::ScrollLock, INPUT);
	}

	void WakeUp()
	{
		Task::enable();
	}

	virtual void SetState(const KeyboardLights::ModeEnum mode, const bool numLock, const bool capsLock) final
	{
		if (Mode != mode)
		{
			AnimationStart = millis();
		}

		NumLock = numLock;
		CapsLock = capsLock;
		Mode = mode;

		TS::Task::enable();
	}

	virtual bool Callback() final
	{
		UpdateLedState();

		switch (Mode)
		{
		case KeyboardLights::ModeEnum::Ble:
		case KeyboardLights::ModeEnum::Usb:
		case KeyboardLights::ModeEnum::Searching:
			break;
		case KeyboardLights::ModeEnum::Off:
		default:
			TS::Task::disable();
			return false;
			break;
		}

		return true;
	}


	void UpdateLedState()
	{
		const uint32_t timestamp = millis();
		const uint32_t elapsed = timestamp - AnimationStart;

		uint8_t capsLock = 0;
		uint8_t numLock = 0;
		uint8_t scrollLock = 0;

		switch (Mode)
		{
		case KeyboardLights::ModeEnum::Ble:
			numLock = NumLock * UINT8_MAX;
			capsLock = CapsLock * UINT8_MAX;
			scrollLock = (UINT8_MAX / brightnessRange) + 1;
			break;
		case KeyboardLights::ModeEnum::Usb:
			numLock = NumLock * UINT8_MAX;
			capsLock = CapsLock * UINT8_MAX;
			scrollLock = UINT8_MAX - 1;
			break;
		case KeyboardLights::ModeEnum::Searching: // Phased animation using all three LEDs.
			numLock = GetSearchingBrightness(elapsed + (BLE_SEARCHING_LED_OFFSET * 2));
			capsLock = GetSearchingBrightness(elapsed + (BLE_SEARCHING_LED_OFFSET * 1));
			scrollLock = GetSearchingBrightness(elapsed + (BLE_SEARCHING_LED_OFFSET * 0));
			break;
		case KeyboardLights::ModeEnum::Off:
		default:
			break;
		}

		Driver.SetBrightness(numLock, capsLock, scrollLock);

		//SetLed(numLock, capsLock, scrollLock);
	}

	//void UpdateLedState2()
	//{
	//	const uint32_t timestamp = millis();
	//	const uint32_t elapsed = timestamp - AnimationStart;

	//	bool capsLock = false;
	//	bool numLock = false;
	//	bool scrollLock = false;

	//	switch (Mode)
	//	{
	//	case KeyboardLights::ModeEnum::Ble:
	//		numLock = NumLock;
	//		capsLock = CapsLock;
	//		scrollLock = true;
	//		break;
	//	case KeyboardLights::ModeEnum::Usb:
	//		numLock = NumLock;
	//		capsLock = CapsLock;
	//		scrollLock = true;
	//		break;
	//	case KeyboardLights::ModeEnum::Searching: // Phased animation using all three LEDs.
	//		numLock = ((elapsed + (BLE_SEARCHING_LED_OFFSET * 2)) % BLE_SEARCHING_ANIMATE_PERIOD) >= BLE_SEARCHING_LED_THRESHOLD;
	//		capsLock = ((elapsed + (BLE_SEARCHING_LED_OFFSET * 1)) % BLE_SEARCHING_ANIMATE_PERIOD) >= BLE_SEARCHING_LED_THRESHOLD;
	//		scrollLock = ((elapsed + (BLE_SEARCHING_LED_OFFSET * 0)) % BLE_SEARCHING_ANIMATE_PERIOD) >= BLE_SEARCHING_LED_THRESHOLD;
	//		break;
	//	case KeyboardLights::ModeEnum::Off:
	//	default:
	//		break;
	//	}

	//	SetLed(numLock, capsLock, scrollLock);
	//}

private:
	static constexpr uint8_t GetSearchingBrightness(const uint32_t elapsed)
	{
		return ((elapsed % BLE_SEARCHING_ANIMATE_PERIOD) * UINT8_MAX) / (BLE_SEARCHING_ANIMATE_PERIOD - 1);
	}

	void SetLed(const bool numLock, const bool capsLock, const bool scrollLock)
	{
		digitalWrite((uint8_t)Pin::NumLock, IoState(numLock));
		digitalWrite((uint8_t)Pin::CapsLock, IoState(capsLock));
		digitalWrite((uint8_t)Pin::ScrollLock, IoState(scrollLock));
	}

	static const bool IoState(const bool state)
	{
		if (onState)
		{
			return state;
		}
		else
		{
			return !state;
		}
	}
};
#endif