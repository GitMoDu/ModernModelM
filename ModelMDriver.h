// ModelMDriver.h

#ifndef _MODEL_M_DRIVER_h
#define _MODEL_M_DRIVER_h

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

#include "IoPort.h"
#include "KeyMap.h"


namespace ModelM
{
	namespace Mapping
	{
		static constexpr uint8_t DriverCount = 2;
		static constexpr uint8_t DriverIoCount = 14;

		static constexpr uint8_t RowCount = 8;
		static constexpr uint8_t ColumnCount = 16;
		static constexpr uint8_t BufferedIoCount = (DriverCount * DriverIoCount) - ColumnCount - RowCount;
	};

	static constexpr uint8_t N_KEY_ROLLOVER = sizeof(hid_keyboard_report_t::keycode);
}

class IKeyboardListener
{
public:
	virtual void OnKeyboardInterrupt() {}
};

/// <summary>
/// Depends on TinyUsb for HID definitions.
/// </summary>
class ModelMDriver
{
private:
	static constexpr uint16_t PORT_IO_MASK = (uint16_t)~(((uint16_t)1 << 14) | ((uint16_t)1 << 15));

private:
	IoPort* Io1;
	IoPort* Io2;

private:
	IKeyboardListener* Listener = nullptr;

private:
	uint8_t RowState[ModelM::Mapping::ColumnCount]{};
	uint16_t ColumnState[ModelM::Mapping::RowCount]{};

public:
	ModelMDriver(IoPort* io1, IoPort* io2)
		: Io1(io1)
		, Io2(io2)
	{}

	void SetListener(IKeyboardListener* listener = nullptr)
	{
		Listener = listener;
	}

	const bool Setup()
	{
		if (Io1 != nullptr && Io2 != nullptr
			&& Io1->GetIoCount() == ModelM::Mapping::DriverIoCount
			&& Io2->GetIoCount() == ModelM::Mapping::DriverIoCount)
		{
			SetMode(0, 0, 0, 0);
			SetState(0, 0);

			return true;
		}

		return false;
	}

	void Clear(hid_keyboard_report_t& report)
	{
		for (uint8_t i = 0; i < ModelM::N_KEY_ROLLOVER; i++)
		{
			report.keycode[i] = HID_KEY_NONE;
		}
	}

	void ClearState()
	{
		for (size_t i = 0; i < ModelM::Mapping::ColumnCount; i++)
		{
			RowState[i] = 0;
		}
		for (size_t i = 0; i < ModelM::Mapping::RowCount; i++)
		{
			ColumnState[i] = 0;
		}
	}

	void GetFastReport(hid_keyboard_report_t& report)
	{
		ReadByColumns();

		// Restore input mode.
		SetMode(0, 0, 0, 0);
		SetState(0, 0);

		Clear(report);

		ParseColumnsToKeys(report);
	}

	void GetFullReport(hid_keyboard_report_t& report)
	{
		ReadByColumns();
		ReadByRows();

		// Restore input mode.
		SetMode(0, 0, 0, 0);
		SetState(0, 0);

		Clear(report);

		ParseColumnsToKeys(report);
		ParseRowsToKeys(report);
	}

#if defined(DEBUG)
	void LogState()
	{
		Serial.println(F("Row State"));
		for (size_t i = 0; i < ModelM::Mapping::ColumnCount; i++)
		{
			Serial.print('\t');
			Serial.print('[');
			for (size_t j = 0; j < ModelM::Mapping::RowCount; j++)
			{
				Serial.print((uint16_t)((RowState[i] >> j) & 1));
			}
			Serial.println(']');
		}
		Serial.println();

		Serial.println(F("Column State"));
		for (size_t i = 0; i < ModelM::Mapping::RowCount; i++)
		{
			Serial.print('\t');
			Serial.print('[');
			for (size_t j = 0; j < ModelM::Mapping::ColumnCount; j++)
			{
				Serial.print((uint8_t)((ColumnState[i] >> j) & 1));
			}
			Serial.println(']');
		}
	}
#endif

private:
	void SetMode(const uint16_t columnDirection, const uint16_t columnPullUp, const uint8_t rowDirection, const uint8_t rowPullUp, const uint8_t buffer = 0)
	{
		Io1->SetMode(GetSlice1(columnDirection), GetSlice1(columnPullUp));
		Io2->SetMode(GetSlice2(columnDirection, rowDirection), GetSlice2(columnPullUp, rowPullUp, buffer));
	}

	void SetState(const uint16_t columnState, const uint8_t rowState)
	{
		Io1->SetState(GetSlice1(columnState));
		Io2->SetState(GetSlice2(columnState, rowState));
	}

	const uint8_t GetRowState()
	{
		const uint16_t port2State = Io2->GetState();

		return (port2State >> 2) & UINT8_MAX;
	}

	const uint16_t GetColumnState()
	{
		const uint16_t port1State = Io1->GetState();
		const uint16_t port2State = Io2->GetState();

		return (port1State & PORT_IO_MASK) | ((port2State & 0x03) << 14);
	}

	void ReadByRows()
	{
		SetMode(UINT16_MAX, 0, UINT8_MAX, UINT8_MAX);
		SetState(0, 0);

		// Set all columns to output and rows as input pull-up.
		SetMode(UINT16_MAX, 0, 0, UINT8_MAX);

		for (uint_fast8_t i = 0; i < ModelM::Mapping::ColumnCount; i++)
		{
			const uint16_t drive = ((uint16_t)1 << i);

			// Drive a single colum to high.
			SetState(drive, 0);

			// Read the first 8 channels for this column.
			RowState[i] = GetRowState();
		}
	}

	void ReadByColumns()
	{
		SetMode(UINT16_MAX, UINT16_MAX, UINT8_MAX, 0);
		SetState(0, 0);

		// Set all rows to output and columns as input pull-up.
		SetMode(0, UINT16_MAX, UINT8_MAX, 0);

		for (uint_fast8_t i = 0; i < ModelM::Mapping::RowCount; i++)
		{
			const uint8_t drive = ((uint8_t)1 << i);

			// Drive a single row to high
			SetState(0, drive);

			// Read the 16 channels for this row.
			ColumnState[i] = GetColumnState();
		}
	}

	/// <summary>
	/// Add all keys by column.
	/// </summary>
	/// <param name="report"></param>
	void ParseColumnsToKeys(hid_keyboard_report_t& report)
	{
		static constexpr uint8_t columnMapSize = sizeof(ModelM::ColumnMap) / sizeof(ModelM::Key);

		for (uint_fast8_t i = 0; i < columnMapSize; i++)
		{
			if (ColumnMatch(ModelM::ColumnMap[i].Item, ModelM::ColumnMap[i].Index))
			{
				if (!AddKey(report, ModelM::ColumnMap[i].KeyCode))
				{
					// N-Key rollover?
				}
			}
		}
	}

	/// <summary>
	/// Add all keys by column.
	/// </summary>
	/// <param name="nKey"></param>
	/// <returns></returns>
	void ParseRowsToKeys(hid_keyboard_report_t& report)
	{
		static constexpr uint8_t rowMapSize = sizeof(ModelM::RowMap) / sizeof(ModelM::Key);

		for (uint_fast8_t i = 0; i < rowMapSize; i++)
		{
			if (RowMatch(ModelM::RowMap[i].Item, ModelM::RowMap[i].Index))
			{
				if (!AddKey(report, ModelM::RowMap[i].KeyCode))
				{
					// N-Key rollover?
				}
			}
		}
	}

	static const bool AddKey(hid_keyboard_report_t& report, const uint8_t key)
	{
		if (key == HID_KEY_NONE)
		{
			return false;
		}

		switch (key)
		{
		case HID_KEY_CONTROL_LEFT:
			report.modifier |= KEYBOARD_MODIFIER_LEFTCTRL;
			return true;
		case HID_KEY_SHIFT_LEFT:
			report.modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
			return true;
		case HID_KEY_ALT_LEFT:
			report.modifier |= KEYBOARD_MODIFIER_LEFTALT;
			return true;
		case HID_KEY_GUI_LEFT:
			report.modifier |= KEYBOARD_MODIFIER_LEFTGUI;
			return true;
		case HID_KEY_CONTROL_RIGHT:
			report.modifier |= KEYBOARD_MODIFIER_RIGHTSHIFT;
			return true;
		case HID_KEY_SHIFT_RIGHT:
			report.modifier |= KEYBOARD_MODIFIER_RIGHTSHIFT;
			return true;
		case HID_KEY_ALT_RIGHT:
			report.modifier |= KEYBOARD_MODIFIER_RIGHTALT;
			return true;
		case HID_KEY_GUI_RIGHT:
			report.modifier |= KEYBOARD_MODIFIER_RIGHTGUI;
			return true;
		default:
			// Regular, non-modifier key.
			for (uint8_t i = 0; i < ModelM::N_KEY_ROLLOVER; i++)
			{
				if (report.keycode[i] == HID_KEY_NONE)
				{
					report.keycode[i] = key;
					return true;
				}
			}
			break;
		}

		return false;
	}

private:
	const bool ColumnMatch(const uint8_t columnIndex, const uint8_t bitIndex) const
	{
		return (ColumnState[columnIndex] >> bitIndex) & 1;
	}

	const bool RowMatch(const uint8_t rowIndex, const uint8_t bitIndex) const
	{
		return (RowState[rowIndex] >> bitIndex) & 1;
	}

private:
	static constexpr uint16_t GetSlice1(const uint16_t column)
	{
		return column & PORT_IO_MASK;
	}

	static constexpr uint16_t GetSlice2(const uint16_t column, const uint8_t row, const uint8_t buffer = 0)
	{
		return (((column >> 14) & 0x03) | (((uint16_t)row) << 2)) & PORT_IO_MASK;
	}
};
#endif