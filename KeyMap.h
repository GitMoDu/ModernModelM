// KeyMap.h

#ifndef _KEY_MAP_h
#define _KEY_MAP_h

#include <Adafruit_TinyUSB.h>


/// <summary>
/// Depends on TinyUsb for HID definitions.
/// </summary>
namespace ModelM
{
	struct Key
	{
		uint8_t Item;
		uint8_t Index;
		uint8_t KeyCode;
	};

	static constexpr Key ColumnMap[]
	{
		{.Item = 0, .Index = 0, .KeyCode = HID_KEY_ESCAPE},
		{.Item = 15, .Index = 7, .KeyCode = HID_KEY_ENTER}
	};


	static constexpr Key RowMap[]
	{
		{.Item = 0, .Index = 0, .KeyCode = HID_KEY_ESCAPE},
		{.Item = 4, .Index = 13, .KeyCode = HID_KEY_ENTER}
	};
}


#endif

