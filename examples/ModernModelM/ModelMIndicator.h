#ifndef _MODELM_INDICATOR_h
#define _MODELM_INDICATOR_h

#include <RetroBle.h>
#include <KeyboardLightsDriver.h>

class IKeyboardIndicator
{
public:
	virtual void SetModifiers(const bool numLock, const bool capsLock, const bool scrollLock) = 0;
};

class KeyboardIndicator : public IIndicator, public IKeyboardIndicator
{
private:
	KeyboardLights::I2cDriver KeyboardDriver;

private:
	IIndicator::StateEnum IndicatorState;
	bool Charging = false;

	bool NumLock = false;
	bool CapsLock = false;
	bool ScrollLock = false;

public:
	KeyboardIndicator(TwoWire& wire)
		: IIndicator()
		, IKeyboardIndicator()
		, KeyboardDriver(wire)
	{
	}

public:
	void SetModifiers(const bool numLock, const bool capsLock, const bool scrollLock) final
	{
		NumLock = numLock;
		CapsLock = capsLock;
		ScrollLock = scrollLock;

		UpdateState();
	}

	void SetDrawMode(const IIndicator::StateEnum indicatorState, const bool charging) final
	{
		IndicatorState = indicatorState;
		Charging = charging;

		UpdateState();
	}

private:
	void UpdateState()
	{
		KeyboardDriver.SetState(GetModeFromIndicator(IndicatorState), NumLock, CapsLock);
	}

private:
	static KeyboardLights::ModeEnum GetModeFromIndicator(IIndicator::StateEnum indicatorState)
	{
		switch (indicatorState)
		{
		case IIndicator::StateEnum::Ble:
			return KeyboardLights::ModeEnum::Ble;
			break;
		case IIndicator::StateEnum::Usb:
			return KeyboardLights::ModeEnum::Usb;
			break;
		case IIndicator::StateEnum::Searching:
			return KeyboardLights::ModeEnum::Searching;
			break;
		case IIndicator::StateEnum::Off:
		default:
			return KeyboardLights::ModeEnum::Off;
			break;
		}
	}
};
#endif

