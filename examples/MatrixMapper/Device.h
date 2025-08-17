#ifndef _DEVICE_h
#define _DEVICE_h

#include <Arduino.h>

#if defined(ARDUINO_Seeed_XIAO_nRF52840_Sense) || defined(ARDUINO_Seeed_XIAO_nRF52840)
#include <variant.h>
#endif

namespace Device
{
	namespace Debug
	{
		static constexpr uint32_t SERIAL_BAUD_RATE = 921600;
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
			static constexpr uint8_t Address = 0x20;

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
			static constexpr uint8_t Address = 0x21;

			enum class Pin : uint8_t
			{
				RST = 7,
				IRQA = 8,
				IRQB = 9
			};
		}
	}
};

#endif
//#endif

