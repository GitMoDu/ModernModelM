#ifndef _MODELM_KEYBOARD_HID_MAP_h
#define _MODELM_KEYBOARD_HID_MAP_h

#include <RetroBle.h>
#include "MappedKey.h"
#include "ModelMDriver.h"

class ModelMKeyboardHidMapped : public ModelMDriver
{
private:
	const uint16_t* ColumnMap;
	const uint16_t* RowMap;

	const uint8_t ColumnMapSize;
	const uint8_t RowMapSize;

private:
	static constexpr uint8_t FAST_ROLLOVER = 4;
	static constexpr uint8_t FULL_ROLLOVER = 10;
	static constexpr uint8_t N_KEY_ROLLOVER = FULL_ROLLOVER;

	static constexpr uint8_t HID_ROLLOVER = sizeof(hid_keyboard_report_t::keycode);

private:
public:
	/// <summary>
	/// constexpr equivalent of min(HID_ROLLOVER, N_KEY_ROLLOVER).
	/// </summary>
	/// <returns>Effective N-Key rollover.</returns>
	static constexpr uint8_t GetRollover()
	{
		return ((HID_ROLLOVER <= N_KEY_ROLLOVER) * (HID_ROLLOVER)) | ((HID_ROLLOVER > N_KEY_ROLLOVER) * (N_KEY_ROLLOVER));
	}

public:
	ModelMKeyboardHidMapped(IoPort* io1, IoPort* io2,
		const uint16_t* columnMap,
		const uint8_t columnMapSize,
		const uint16_t* rowMap,
		const uint8_t rowMapSize)
		: ModelMDriver(io1, io2)
		, ColumnMap(columnMap)
		, RowMap(rowMap)
		, ColumnMapSize(columnMapSize)
		, RowMapSize(rowMapSize)
	{
	}

public:
	void GetReport(hid_keyboard_report_t& report)
	{
		GetFastReport(report);
	}

private:
	void GetFastReport(hid_keyboard_report_t& report)
	{
		ReadByColumnDrive();
		SetInputModeAll();

		Clear(report);
		ParseRowsToKeys(report);
	}

	void GetFullReport(hid_keyboard_report_t& report)
	{
		ReadByColumnDrive();
		ReadByRowDrive();
		SetInputModeAll();

		Clear(report);
		ParseColumnsToKeys(report);
		ParseRowsToKeys(report);
	}

private:
	/// <summary>
	/// Add all keys by column.
	/// </summary>
	/// <param name="report"></param>
	void ParseColumnsToKeys(hid_keyboard_report_t& report)
	{
		for (uint_fast8_t i = 0; i < ColumnMapSize; i++)
		{
			const uint16_t code = ColumnMap[i];

			if (ColumnMatch(MappedKey::Item(code), MappedKey::Index(code)))
			{
				AddKeyCode(report, MappedKey::KeyCode(code));
			}
		}
	}

	/// <summary>
	/// Add all keys by row.
	/// </summary>
	/// <param name="report"></param>
	void ParseRowsToKeys(hid_keyboard_report_t& report)
	{
		for (uint_fast8_t i = 0; i < RowMapSize; i++)
		{
			const uint16_t code = RowMap[i];

			if (RowMatch(MappedKey::Item(code), MappedKey::Index(code)))
			{
				AddKeyCode(report, MappedKey::KeyCode(code));
			}
		}
	}

	static void Clear(hid_keyboard_report_t& report)
	{
		for (uint8_t i = 0; i < sizeof(hid_keyboard_report_t::keycode); i++)
		{
			report.keycode[i] = HID_KEY_NONE;
		}
		report.reserved = 0;
		report.modifier = 0;
	}

	/// <summary>
	/// Assumes keycodes are added orderly.
	/// </summary>
	/// <param name="report"></param>
	/// <param name="key"></param>
	static void AddKeyCode(hid_keyboard_report_t& report, const uint8_t key)
	{
		if (key != HID_KEY_NONE)
		{
			switch (key)
			{
			case HID_KEY_CONTROL_LEFT:
				report.modifier |= KEYBOARD_MODIFIER_LEFTCTRL;
				break;
			case HID_KEY_SHIFT_LEFT:
				report.modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
				break;
			case HID_KEY_ALT_LEFT:
				report.modifier |= KEYBOARD_MODIFIER_LEFTALT;
				break;
			case HID_KEY_GUI_LEFT:
				report.modifier |= KEYBOARD_MODIFIER_LEFTGUI;
				break;
			case HID_KEY_CONTROL_RIGHT:
				report.modifier |= KEYBOARD_MODIFIER_RIGHTCTRL;
				break;
			case HID_KEY_SHIFT_RIGHT:
				report.modifier |= KEYBOARD_MODIFIER_RIGHTSHIFT;
				break;
			case HID_KEY_ALT_RIGHT:
				report.modifier |= KEYBOARD_MODIFIER_RIGHTALT;
				break;
			case HID_KEY_GUI_RIGHT:
				report.modifier |= KEYBOARD_MODIFIER_RIGHTGUI;
				break;
			default:// Regular, non-modifier key.
				// Search for the first free spot in the report keycodes.
				for (uint_fast8_t i = 0; i < GetRollover(); i++)
				{
					if (report.keycode[i] == HID_KEY_NONE)
					{
						// Search for existing keycode.
						for (uint_fast8_t j = 0; j < i; j++)
						{
							if (report.keycode[j] == key)
							{
								// Key code already present, don't add.
								return;
							}
						}

						// Add keycode.
						report.keycode[i] = key;
						break;
					}
				}
				break;
			}
		}
	}
};

#endif