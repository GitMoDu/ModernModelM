// Definitions.h

#ifndef _DEFINITIONS_h
#define _DEFINITIONS_h

#include <Arduino.h>
#include <RetroBle.h>

#if defined(ARDUINO_Seeed_XIAO_nRF52840_Sense) || defined(ARDUINO_Seeed_XIAO_nRF52840)
#include <variant.h>

namespace Device
{
#if defined(DEBUG)
	namespace Debug
	{
		static constexpr uint32_t SERIAL_BAUD_RATE = 921600;
	}
#endif
	static constexpr char Name[] = "ModelM Keyboard";

	namespace Version
	{
		static constexpr uint16_t Code = 0;
		static constexpr char Name[] = "0";
	}

	namespace BMS
	{
		using Pin = SeeedXIAOnRF52840::BMS::Pin;
		using Calibration = SeeedXIAOnRF52840::BMS::Bq25100Calibration;

		static constexpr uint8_t WakePin = (uint8_t)Pin::Charging;

		static constexpr uint8_t HistorySize = 50;
		static constexpr uint32_t UpdatePeriodMillis = 50;
	}

	namespace Led
	{
		using Pin = SeeedXIAOnRF52840::Led::Pin;
		static constexpr bool OnState = SeeedXIAOnRF52840::Led::OnState;
	}

	namespace I2C
	{
		enum class Pin : uint8_t
		{
			SDA = PIN_WIRE_SDA,	// AC24
			SDL = PIN_WIRE_SCL	// AA24
		};
	}

	namespace IoExpander
	{
		namespace Io1
		{
			static constexpr uint8_t Channel = 0;
			static constexpr uint8_t Address = 0x21;

			enum class Pin : uint8_t
			{
				RST = 0,
				IRQA = 1,
				IRQB = 2
			};
		}

		namespace Io2
		{
			static constexpr uint8_t Channel = 0;
			static constexpr uint8_t Address = 0x20;
			
			enum class Pin : uint8_t
			{
				RST = 7,
				IRQA = 8,
				IRQB = 9
			};
		}
	}

	namespace BLE
	{
		static constexpr uint32_t UpdatePeriodMillis = 15;

		static constexpr RetroBle::BleConfig::Appearance Appearance = RetroBle::BleConfig::Appearance::Keyboard;

		static constexpr uint32_t LONG_PRESS_POWER_OFF_PERIOD_MILLIS = 5000;
	}

	namespace USB
	{
		static constexpr uint32_t UpdatePeriodMillis = 5;

		//TODO:
		static constexpr uint16_t ProductId = 0;
	}

	namespace Keyboard
	{
	}

	namespace Unused
	{
		static constexpr uint8_t Pins[] = { D5, D6, D7, D8, D9, D10 };
	};
};

#else
#error Device is not supported for this project.
#endif

#endif

