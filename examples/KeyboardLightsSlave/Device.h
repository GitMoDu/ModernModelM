// Device.h

#ifndef _DEVICE_h
#define _DEVICE_h


namespace Device
{
	namespace Led
	{
		static constexpr uint32_t AnimatePeriod = 8;

		static constexpr uint8_t BrightnessRange = 4;

#if defined(ARDUINO_AVR_ATTINYX5)
		enum class Pin : uint8_t
		{
			NumLock = PB1,
			CapsLock = PB3,
			ScrollLock = PB4
		};

		static constexpr bool OnState = LOW;
#elif defined(ARDUINO_ARCH_AVR)
		enum class Pin : uint8_t
		{
			NumLock = A0,
			CapsLock = A1,
			ScrollLock = A2
		};

		static constexpr bool OnState = HIGH;
#else
#error Platform not supported.
#endif
	}
}


#endif

