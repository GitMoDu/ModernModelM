#ifndef _DEVICE_h
#define _DEVICE_h

#if defined(ARDUINO_Seeed_XIAO_nRF52840_Sense) || defined(ARDUINO_Seeed_XIAO_nRF52840)
#include <RetroBle.h>

#include "ModelMSleepyKeyboard.h"

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

		static constexpr uint8_t HistorySize = 200;
		static constexpr uint32_t UpdatePeriodMillis = 30;
	}

	namespace I2C0
	{
		enum class Pin : uint8_t
		{
			SDA = PIN_WIRE_SDA,	// AC24
			SDL = PIN_WIRE_SCL	// AA24
		};

		static constexpr uint32_t Clock = 400000;
		static constexpr uint8_t Channel = 0;
	}

	namespace Lights
	{
		static constexpr uint8_t Channel = I2C0::Channel;
		static constexpr uint8_t Address = KeyboardLights::Api::Address;
	}

	namespace IoExpander
	{
		namespace Io1
		{
			static constexpr uint8_t Channel = I2C0::Channel;
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
			static constexpr uint8_t Channel = I2C0::Channel;
			static constexpr uint8_t Address = 0x21;

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
		static constexpr uint32_t ConnectionIntervalMin = RetroBle::BleConfig::ConnectionIntervalRegular::Min;
		static constexpr uint32_t ConnectionIntervalMax = RetroBle::BleConfig::ConnectionIntervalRegular::Max;

		static constexpr uint32_t UpdatePeriodMillis = 20;

		static constexpr RetroBle::BleConfig::Appearance Appearance = RetroBle::BleConfig::Appearance::Keyboard;


		static constexpr uint32_t LONG_PRESS_POWER_OFF_PERIOD_MILLIS = 5000;
	}

	namespace USB
	{
		static constexpr uint32_t UpdatePeriodMillis = 20;

		static constexpr uint16_t ProductId = (uint16_t)RetroBle::Device::CustomProductIds::KeyboardModelM;
	}

	namespace Unused
	{
		static constexpr uint8_t Pins[] = { LED_BUILTIN, LED_RED, LED_GREEN, LED_BLUE };
	};

	namespace Keyboard
	{
		namespace KeyMap
		{
			static constexpr uint16_t ColumnMap[]
			{
				// Function
				MappedKey::GetCode<	2,	3,	HID_KEY_F1>(),
				MappedKey::GetCode<	2,	2,	HID_KEY_F2>(),
				MappedKey::GetCode<	3,	2,	HID_KEY_F3>(),
				MappedKey::GetCode<	4,	2,	HID_KEY_F4>(),
				MappedKey::GetCode<	4,	0,	HID_KEY_F5>(),
				MappedKey::GetCode<	4,	8,	HID_KEY_F6>(),
				MappedKey::GetCode<	3,	9,	HID_KEY_F7>(),
				MappedKey::GetCode<	2,	9,	HID_KEY_F8>(),
				MappedKey::GetCode<	2,	0,	HID_KEY_F9>(),
				MappedKey::GetCode<	1,	0,	HID_KEY_F10>(),
				MappedKey::GetCode<	1,	11,	HID_KEY_F11>(),
				MappedKey::GetCode<	1,	12,	HID_KEY_F12>(),

				// System.
				MappedKey::GetCode<	1,	14,	HID_KEY_PRINT_SCREEN>(),
				MappedKey::GetCode<	0,	14,	HID_KEY_SCROLL_LOCK>(),
				MappedKey::GetCode<	6,	15,	HID_KEY_PAUSE>(),// TODO: use internally only?

				// Arrow.
				MappedKey::GetCode<	4,	15,	HID_KEY_ARROW_UP>(),
				MappedKey::GetCode<	7,	11,	HID_KEY_ARROW_DOWN>(),
				MappedKey::GetCode<	7,	15,	HID_KEY_ARROW_LEFT>(),
				MappedKey::GetCode<	7,	12,	HID_KEY_ARROW_RIGHT>(),

				// Navigation.
				MappedKey::GetCode<	4,	4,	HID_KEY_ESCAPE>(),
				MappedKey::GetCode<	6,	0,	HID_KEY_ENTER>(),
				MappedKey::GetCode<	2,	12,	HID_KEY_INSERT>(),
				MappedKey::GetCode<	2,	11,	HID_KEY_DELETE>(),
				MappedKey::GetCode<	2,	15,	HID_KEY_HOME>(),
				MappedKey::GetCode<	1,	15,	HID_KEY_END>(),
				MappedKey::GetCode<	2,	13,	HID_KEY_PAGE_UP>(),
				MappedKey::GetCode<	1,	13,	HID_KEY_PAGE_DOWN>(),

				// Text handling.
				MappedKey::GetCode<	3,	4,	HID_KEY_TAB>(),
				MappedKey::GetCode<	3,	3,	HID_KEY_CAPS_LOCK>(),
				MappedKey::GetCode<	3,	0,	HID_KEY_BACKSPACE>(),
				MappedKey::GetCode<	7,	6,	HID_KEY_SPACE>(),

				// Modifier keys.
				MappedKey::GetCode<	2,	6,	HID_KEY_CONTROL_LEFT>(),
				MappedKey::GetCode<	6,	6,	HID_KEY_CONTROL_RIGHT>(),
				MappedKey::GetCode<	3,	5,	HID_KEY_SHIFT_LEFT>(),
				MappedKey::GetCode<	6,	5,	HID_KEY_SHIFT_RIGHT>(),
				MappedKey::GetCode<	4,	14,	HID_KEY_ALT_LEFT>(),
				MappedKey::GetCode<	7,	14,	HID_KEY_ALT_RIGHT>(), // Alt Gr.

				// Numpad.
				MappedKey::GetCode<	6,	11,	HID_KEY_NUM_LOCK>(),
				MappedKey::GetCode<	6,	12,	HID_KEY_KEYPAD_DIVIDE>(),
				MappedKey::GetCode<	6,	13,	HID_KEY_KEYPAD_MULTIPLY>(),
				MappedKey::GetCode<	7,	13,	HID_KEY_KEYPAD_SUBTRACT>(),
				MappedKey::GetCode<	0,	15,	HID_KEY_KEYPAD_ADD>(),
				MappedKey::GetCode<	5,	15, HID_KEY_KEYPAD_ENTER>(),
				MappedKey::GetCode<	4,	13,	HID_KEY_KEYPAD_DECIMAL>(),
				MappedKey::GetCode<	4,	12,	HID_KEY_KEYPAD_0>(),
				MappedKey::GetCode<	5,	11,	HID_KEY_KEYPAD_1>(),
				MappedKey::GetCode<	5,	12,	HID_KEY_KEYPAD_2>(),
				MappedKey::GetCode<	5,	13,	HID_KEY_KEYPAD_3>(),
				MappedKey::GetCode<	3,	11,	HID_KEY_KEYPAD_4>(),
				MappedKey::GetCode<	3,	12,	HID_KEY_KEYPAD_5>(),
				MappedKey::GetCode<	3,	13,	HID_KEY_KEYPAD_6>(),
				MappedKey::GetCode<	0,	11,	HID_KEY_KEYPAD_7>(),
				MappedKey::GetCode<	0,	12,	HID_KEY_KEYPAD_8>(),
				MappedKey::GetCode<	0,	13,	HID_KEY_KEYPAD_9>(),

				// Special Characters.
				MappedKey::GetCode<	2,	4,	HID_KEY_GRAVE>(), /* \  */
				MappedKey::GetCode<	2,	10,	HID_KEY_MINUS>(), // '
				MappedKey::GetCode<	2,	8,	HID_KEY_EQUAL>(), // «

				MappedKey::GetCode<	3,	10,	HID_KEY_BRACKET_LEFT>(), // +
				MappedKey::GetCode<	3,	8,	HID_KEY_BRACKET_RIGHT>(), // ´

				MappedKey::GetCode<	4,	10,	HID_KEY_APOSTROPHE>(), // º ª 
				MappedKey::GetCode<	6,	10,	HID_KEY_EUROPE_1>(), // ~

				MappedKey::GetCode<	4,	3,	HID_KEY_EUROPE_2>(), // <
				MappedKey::GetCode<	6,	8,	HID_KEY_COMMA>(), // ,
				MappedKey::GetCode<	6,	9,	HID_KEY_PERIOD>(), // .
				MappedKey::GetCode<	7,	10,	HID_KEY_SLASH>(), // -

				// Numbers.
				MappedKey::GetCode<	1,	10,	HID_KEY_0>(),
				MappedKey::GetCode<	1,	4,	HID_KEY_1>(),
				MappedKey::GetCode<	1,	3,	HID_KEY_2>(),
				MappedKey::GetCode<	1,	2,	HID_KEY_3>(),
				MappedKey::GetCode<	1,	1,	HID_KEY_4>(),
				MappedKey::GetCode<	2,	1,	HID_KEY_5>(),
				MappedKey::GetCode<	2,	7,	HID_KEY_6>(),
				MappedKey::GetCode<	1,	7,	HID_KEY_7>(),
				MappedKey::GetCode<	1,	8,	HID_KEY_8>(),
				MappedKey::GetCode<	1,	9,	HID_KEY_9>(),

				// Letters
				MappedKey::GetCode<	0,	4,	HID_KEY_Q>(),
				MappedKey::GetCode<	0,	3,	HID_KEY_W>(),
				MappedKey::GetCode<	0,	2,	HID_KEY_E>(),

				MappedKey::GetCode<	0,	1,	HID_KEY_R>(),
				MappedKey::GetCode<	3,	1,	HID_KEY_T>(),
				MappedKey::GetCode<	3,	7,	HID_KEY_Y>(),
				MappedKey::GetCode<	0,	7,	HID_KEY_U>(),
				MappedKey::GetCode<	0,	8,	HID_KEY_I>(),
				MappedKey::GetCode<	0,	9,	HID_KEY_O>(),
				MappedKey::GetCode<	0,	10,	HID_KEY_P>(),

				MappedKey::GetCode<	5,	4,	HID_KEY_A>(),
				MappedKey::GetCode<	5,	3,	HID_KEY_S>(),
				MappedKey::GetCode<	5,	2,	HID_KEY_D>(),
				MappedKey::GetCode<	5,	1,	HID_KEY_F>(),
				MappedKey::GetCode<	4,	1,	HID_KEY_G>(),
				MappedKey::GetCode<	4,	7,	HID_KEY_H>(),
				MappedKey::GetCode<	5,	7,	HID_KEY_J>(),
				MappedKey::GetCode<	5,	8,	HID_KEY_K>(),
				MappedKey::GetCode<	5,	9,	HID_KEY_L>(),
				MappedKey::GetCode<	5,	10,	HID_KEY_SEMICOLON>(), // Ç

				MappedKey::GetCode<	6,	4,	HID_KEY_Z>(),
				MappedKey::GetCode<	6,	3,	HID_KEY_X>(),
				MappedKey::GetCode<	6,	2,	HID_KEY_C>(),
				MappedKey::GetCode<	6,	1,	HID_KEY_V>(),
				MappedKey::GetCode<	7,	1,	HID_KEY_B>(),
				MappedKey::GetCode<	7,	6,	HID_KEY_N>(),
				MappedKey::GetCode<	6,	7,	HID_KEY_M>()
			};

			static constexpr uint16_t RowMap[]
			{
				// Function
				MappedKey::GetCode<	5,	2,	HID_KEY_F1>(),
				MappedKey::GetCode<	4,	2,	HID_KEY_F2>(),
				MappedKey::GetCode<	4,	3,	HID_KEY_F3>(),
				MappedKey::GetCode<	4,	4,	HID_KEY_F4>(),
				MappedKey::GetCode<	2,	4,	HID_KEY_F5>(),
				MappedKey::GetCode<	10,	4,	HID_KEY_F6>(),
				MappedKey::GetCode<	11,	3,	HID_KEY_F7>(),
				MappedKey::GetCode<	11,	2,	HID_KEY_F8>(),
				MappedKey::GetCode<	2,	2,	HID_KEY_F9>(),
				MappedKey::GetCode<	2,	1,	HID_KEY_F10>(),
				MappedKey::GetCode<	13,	1,	HID_KEY_F11>(),
				MappedKey::GetCode<	14,	1,	HID_KEY_F12>(),

				// System.
				MappedKey::GetCode<	0,	1,	HID_KEY_PRINT_SCREEN>(),
				MappedKey::GetCode<	0,	0,	HID_KEY_SCROLL_LOCK>(),
				MappedKey::GetCode<	1,	6,	HID_KEY_PAUSE>(),// TODO: use internally only?

				// Arrow.
				MappedKey::GetCode<	1,	4,	HID_KEY_ARROW_UP>(),
				MappedKey::GetCode<	13,	7,	HID_KEY_ARROW_DOWN>(),
				MappedKey::GetCode<	1,	7,	HID_KEY_ARROW_LEFT>(),
				MappedKey::GetCode<	14,	7 ,	HID_KEY_ARROW_RIGHT>(),

				// Navigation.
				MappedKey::GetCode<	6,	4,	HID_KEY_ESCAPE>(),
				MappedKey::GetCode<	2,	6,	HID_KEY_ENTER>(),
				MappedKey::GetCode<	14,	2,	HID_KEY_INSERT>(),
				MappedKey::GetCode<	13,	2,	HID_KEY_DELETE>(),
				MappedKey::GetCode<	1,	2,	HID_KEY_HOME>(),
				MappedKey::GetCode<	1,	1,	HID_KEY_END>(),
				MappedKey::GetCode<	15,	2,	HID_KEY_PAGE_UP>(),
				MappedKey::GetCode<	15,	1,	HID_KEY_PAGE_DOWN>(),

				// Text handling.
				MappedKey::GetCode<	6,	3,	HID_KEY_TAB>(),
				MappedKey::GetCode<	5,	3,	HID_KEY_CAPS_LOCK>(),
				MappedKey::GetCode<	2,	3,	HID_KEY_BACKSPACE>(),
				MappedKey::GetCode<	2,	7,	HID_KEY_SPACE>(),

				// Modifier keys.
				MappedKey::GetCode<	8,	2,	HID_KEY_CONTROL_LEFT>(),
				MappedKey::GetCode<	8,	6,	HID_KEY_CONTROL_RIGHT>(),
				MappedKey::GetCode<	7,	3,	HID_KEY_SHIFT_LEFT>(),
				MappedKey::GetCode<	7,	6,	HID_KEY_SHIFT_RIGHT>(),
				MappedKey::GetCode<	0,	4,	HID_KEY_ALT_LEFT>(),
				MappedKey::GetCode<	0,	7,	HID_KEY_ALT_RIGHT>(), // Alt Gr.

				// Numpad.
				MappedKey::GetCode<	13,	6,	HID_KEY_NUM_LOCK>(),
				MappedKey::GetCode<	14,	6,	HID_KEY_KEYPAD_DIVIDE>(),
				MappedKey::GetCode<	15,	6,	HID_KEY_KEYPAD_MULTIPLY>(),
				MappedKey::GetCode<	15,	7,	HID_KEY_KEYPAD_SUBTRACT>(),
				MappedKey::GetCode<	1,	0,	HID_KEY_KEYPAD_ADD>(),
				MappedKey::GetCode<	1,	5,	HID_KEY_KEYPAD_ENTER>(),
				MappedKey::GetCode<	15,	4,	HID_KEY_KEYPAD_DECIMAL>(),
				MappedKey::GetCode<	14,	4,	HID_KEY_KEYPAD_0>(),
				MappedKey::GetCode<	13,	5,	HID_KEY_KEYPAD_1>(),
				MappedKey::GetCode<	14,	5,	HID_KEY_KEYPAD_2>(),
				MappedKey::GetCode<	15,	5,	HID_KEY_KEYPAD_3>(),
				MappedKey::GetCode<	13,	3,	HID_KEY_KEYPAD_4>(),
				MappedKey::GetCode<	14,	3,	HID_KEY_KEYPAD_5>(),
				MappedKey::GetCode<	15,	3,	HID_KEY_KEYPAD_6>(),
				MappedKey::GetCode<	13,	0,	HID_KEY_KEYPAD_7>(),
				MappedKey::GetCode<	14,	0,	HID_KEY_KEYPAD_8>(),
				MappedKey::GetCode<	15,	0,	HID_KEY_KEYPAD_9>(),

				// Special Characters.
				MappedKey::GetCode<	6,	2,	HID_KEY_GRAVE>(), /* \  */
				MappedKey::GetCode<	12,	2,	HID_KEY_MINUS>(), // '
				MappedKey::GetCode<	10,	2,	HID_KEY_EQUAL>(), // «

				MappedKey::GetCode<	12,	3,	HID_KEY_BRACKET_LEFT>(), // +
				MappedKey::GetCode<	10,	3,	HID_KEY_BRACKET_RIGHT>(), // ´

				MappedKey::GetCode<	12,	4,	HID_KEY_APOSTROPHE>(), // º ª 
				MappedKey::GetCode<	12,	6,	HID_KEY_EUROPE_1>(), // ~

				MappedKey::GetCode<	5,	4,	HID_KEY_EUROPE_2>(), // <
				MappedKey::GetCode<	10,	6,	HID_KEY_COMMA>(), // ,
				MappedKey::GetCode<	11,	6,	HID_KEY_PERIOD>(), // .
				MappedKey::GetCode<	12,	7,	HID_KEY_SLASH>(), // -

				// Numbers.
				MappedKey::GetCode<	12,	1,	HID_KEY_0>(),
				MappedKey::GetCode<	6,	1,	HID_KEY_1>(),
				MappedKey::GetCode<	5,	1,	HID_KEY_2>(),
				MappedKey::GetCode<	4,	1,	HID_KEY_3>(),
				MappedKey::GetCode<	3,	1,	HID_KEY_4>(),
				MappedKey::GetCode<	3,	2,	HID_KEY_5>(),
				MappedKey::GetCode<	9,	2,	HID_KEY_6>(),
				MappedKey::GetCode<	9,	1,	HID_KEY_7>(),
				MappedKey::GetCode<	10,	1,	HID_KEY_8>(),
				MappedKey::GetCode<	11,	1,	HID_KEY_9>(),

				// Letters
				MappedKey::GetCode<	6,	0,	HID_KEY_Q>(),
				MappedKey::GetCode<	5,	0,	HID_KEY_W>(),
				MappedKey::GetCode<	4,	0,	HID_KEY_E>(),
				MappedKey::GetCode<	3,	0,	HID_KEY_R>(),
				MappedKey::GetCode<	3,	3,	HID_KEY_T>(),
				MappedKey::GetCode<	9,	3,	HID_KEY_Y>(),
				MappedKey::GetCode<	9,	0,	HID_KEY_U>(),
				MappedKey::GetCode<	10,	0,	HID_KEY_I>(),
				MappedKey::GetCode<	11,	0,	HID_KEY_O>(),
				MappedKey::GetCode<	12,	0,	HID_KEY_P>(),

				MappedKey::GetCode<	6,	5,	HID_KEY_A>(),
				MappedKey::GetCode<	5,	5,	HID_KEY_S>(),
				MappedKey::GetCode<	4,	5,	HID_KEY_D>(),
				MappedKey::GetCode<	3,	5,	HID_KEY_F>(),
				MappedKey::GetCode<	3,	4,	HID_KEY_G>(),
				MappedKey::GetCode<	9,	4,	HID_KEY_H>(),
				MappedKey::GetCode<	9,	5,	HID_KEY_J>(),
				MappedKey::GetCode<	10,	5,	HID_KEY_K>(),
				MappedKey::GetCode<	11,	5,	HID_KEY_L>(),
				MappedKey::GetCode<	12,	5,	HID_KEY_SEMICOLON>(), // Ç

				MappedKey::GetCode<	6,	6,	HID_KEY_Z>(),
				MappedKey::GetCode<	5,	6,	HID_KEY_X>(),
				MappedKey::GetCode<	4,	6,	HID_KEY_C>(),
				MappedKey::GetCode<	3,	6,	HID_KEY_V>(),
				MappedKey::GetCode<	3,	7,	HID_KEY_B>(),
				MappedKey::GetCode<	9,	7,	HID_KEY_N>(),
				MappedKey::GetCode<	9,	6,	HID_KEY_M>()
			};

			static constexpr uint16_t ColumnMapSize = sizeof(ColumnMap) / sizeof(uint16_t);
			static constexpr uint16_t RowMapSize = sizeof(RowMap) / sizeof(uint16_t);
		};
	};
};
#else
#error Device is not supported for this project.
#endif
#endif
