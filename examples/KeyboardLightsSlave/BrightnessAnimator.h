// BrightnessAnimator.h

#ifndef _BRIGHTNESSANIMATOR_h
#define _BRIGHTNESSANIMATOR_h

#define _TASK_OO_CALLBACKS
#include <TSchedulerDeclarations.hpp>

#include <Arduino.h>

template<uint8_t BrightnessRange,
	typename Pin,
	bool onState = LOW>
class BrightnessAnimator : public virtual KeyboardLights::IKeyboardLights, private TS::Task
{
private:
	struct ThresholdStruct
	{
		uint8_t NumLock = 0;
		uint8_t CapsLock = 0;
		uint8_t ScrollLock = 0;
	};

	ThresholdStruct Current{};
	ThresholdStruct Next{};


private:
	uint8_t Frame = 0;

public:
	BrightnessAnimator(TS::Scheduler& scheduler,
		const uint32_t updatePeriod = 1)
		: TS::Task(updatePeriod, TASK_FOREVER, &scheduler, false)
	{
	}

	void Stop()
	{
		pinMode((uint8_t)Pin::NumLock, INPUT);
		pinMode((uint8_t)Pin::CapsLock, INPUT);
		pinMode((uint8_t)Pin::ScrollLock, INPUT);
		SetLed(false, false, false);
	}

	void Start()
	{
		SetLed(false, false, false);
		pinMode((uint8_t)Pin::NumLock, OUTPUT);
		pinMode((uint8_t)Pin::CapsLock, OUTPUT);
		pinMode((uint8_t)Pin::ScrollLock, OUTPUT);
	}

	virtual bool Callback() final
	{
		Step(Frame);
		Frame++;
		if (Frame >= BrightnessRange)
		{
			Frame = 0;
			Current.NumLock = Next.NumLock;
			Current.CapsLock = Next.CapsLock;
			Current.ScrollLock = Next.ScrollLock;
		}

		return true;
	}


	void SetBrightness(const uint8_t numLock, const uint8_t capsLock, const uint8_t scrollLock)
	{
		if (numLock == 0
			&& capsLock == 0
			&& scrollLock == 0)
		{
			SetLed(false, false, false);
			TS::Task::disable();
		}
		else
		{
			Next.NumLock = GetThreshold(numLock);
			Next.CapsLock = GetThreshold(capsLock);
			Next.ScrollLock = GetThreshold(scrollLock);
			TS::Task::enableIfNot();
		}
	}

	void Step(const uint8_t frame)
	{
		SetLed(frame >= Current.NumLock,
			frame >= Current.CapsLock,
			frame >= Current.ScrollLock);
	}

private:
	static constexpr uint8_t GetThreshold(const uint8_t brightness)
	{
		return ((uint16_t)(UINT8_MAX - brightness) * BrightnessRange) / UINT8_MAX;
	}

	virtual void SetLed(const bool numLock, const bool capsLock, const bool scrollLock) final
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

