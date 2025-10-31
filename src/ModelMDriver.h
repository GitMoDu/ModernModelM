#ifndef _MODEL_M_DRIVER_h
#define _MODEL_M_DRIVER_h

#include <Arduino.h>
#include "IoPort.h"

/// <summary>
/// Driver for custom board with 2 inline 14 IO ports.
/// Mapping
///		IO1: 0-4 Rows 0-4
///		IO1: 5-6 Columns 15-16
///		IO1: 7-9 Rows 5-7
///		IO2: 0-14 Columns 0-14
/// </summary>
class ModelMDriver
{
public:
	static constexpr uint8_t RowCount = 8;
	static constexpr uint8_t ColumnCount = 16;

	static constexpr uint8_t DriverCount = 2;
	static constexpr uint8_t DriverIoCount = 14;

private:
	static constexpr uint16_t PORT_IO_MASK = (uint16_t)~(((uint16_t)1 << 14) | ((uint16_t)1 << 15));

private:
	IoPort* Io1;
	IoPort* Io2;

protected:
	uint8_t RowState[ColumnCount]{};
	uint16_t ColumnState[RowCount]{};

public:
	ModelMDriver(IoPort* io1, IoPort* io2)
		: Io1(io1)
		, Io2(io2)
	{
	}

	virtual bool Setup()
	{
		if (Io1 != nullptr && Io2 != nullptr
			&& Io1->GetIoCount() == DriverIoCount
			&& Io2->GetIoCount() == DriverIoCount)
		{
			SetMode(0, 0, 0, 0);
			SetState(0, 0);

			// Disable interrupts by default.
			SetIRQ(0, 0, 0);

			// Io2 IRQs are not used.
			Io2->AttachInterrupt(nullptr, nullptr);

			return true;
		}

		return false;
	}

	void SetInputModeAll()
	{
		// Restore input mode.
		SetMode(0, 0, 0, 0);
		SetState(0, 0);
	}

protected:
	void DisableKeyInterrupt()
	{
		// Disable IRQs.
		SetIRQ(0, 0, 0);

		// Disable interrupt handler by setting null callbacks.
		Io1->AttachInterrupt(nullptr, nullptr);
		// Io2 IRQs are not used.
	}

	void AttachAnyKeyInterrupt(void (*irqInterrupt)())
	{
		// Set all columns to output and rows as input pull-up.
		SetMode(UINT16_MAX, 0, 0, UINT8_MAX);
		SetState(0, 0);

		// Enable IRQs on all rows with CHANGE mode (triggers on any state change).
		// Mode 0 would disable, so we need a non-zero mode value.
		// The MCP23017 hardware detects changes from previous state.
		SetIRQ(0, 0b11111111, CHANGE);

		// Attach the interrupt handler to both IRQA and INTB of Io1 (rows are on Io1).
		Io1->AttachInterrupt(irqInterrupt, irqInterrupt);
		// Io2 IRQs are not used.
	}

	void ClearState()
	{
		for (size_t i = 0; i < ColumnCount; i++)
		{
			RowState[i] = 0;
		}
		for (size_t i = 0; i < RowCount; i++)
		{
			ColumnState[i] = 0;
		}
	}

	void ReadByColumnDrive()
	{
		// Set all columns to output and rows as input pull-up.
		SetMode(UINT16_MAX, 0, 0, UINT8_MAX);
		SetState(0, 0);

		for (uint_fast8_t i = 0; i < ColumnCount; i++)
		{
			const uint16_t drive = (uint16_t(1) << i);

			// Drive a single column to low.
			SetState(~drive, 0);

			// Read the first 8 channels for this column.
			RowState[i] = ~GetRowState();
		}
	}

	void ReadByRowDrive()
	{
		// Set all rows to output and columns as input pull-up.
		SetMode(0, UINT16_MAX, UINT8_MAX, 0);
		SetState(0, 0);

		for (uint_fast8_t i = 0; i < RowCount; i++)
		{
			const uint8_t drive = ((uint8_t)1 << i);

			// Drive a single row to low.
			SetState(0, ~drive);

			// Read the 16 channels for this row.
			ColumnState[i] = ~GetColumnState();
		}
	}

	const bool ColumnMatch(const uint8_t columnIndex, const uint8_t bitIndex) const
	{
		return (ColumnState[columnIndex] >> bitIndex) & 1;
	}

	const bool RowMatch(const uint8_t rowIndex, const uint8_t bitIndex) const
	{
		return (RowState[rowIndex] >> bitIndex) & 1;
	}

private:
	void SetMode(const uint16_t columnDirection, const uint16_t columnPullUp, const uint8_t rowDirection, const uint8_t rowPullUp)
	{
		Io1->SetMode(GetSlice1(columnDirection, rowDirection), GetSlice1(columnPullUp, rowPullUp));
		Io2->SetMode(GetSlice2(columnDirection), GetSlice2(columnPullUp));
	}

	void SetState(const uint16_t columnState, const uint8_t rowState)
	{
		Io1->SetState(GetSlice1(columnState, rowState));
		Io2->SetState(GetSlice2(columnState));
	}

	void SetIRQ(const uint16_t columnMask, const uint8_t rowMask, const uint8_t mode)
	{
		Io1->SetIRQ(GetSlice1(columnMask, rowMask), mode);
		Io2->SetIRQ(GetSlice2(columnMask), mode);
	}

	const uint8_t GetRowState()
	{
		const uint16_t port1State = Io1->GetState();

		const uint8_t lowSide = port1State & 0b11111;         // bits 0-4 (Rows 0-4)
		const uint8_t highSide = (port1State >> 7) & 0b111;   // bits 7-9 (Rows 5-7)

		return lowSide | (highSide << 5);
	}

	const uint16_t GetColumnState()
	{
		const uint16_t port1State = Io1->GetState();
		const uint16_t port2State = Io2->GetState();

		const uint8_t port1Columns = (port1State >> 5) & 0b11;
		return port2State | (uint16_t(port1Columns) << 14);
	}

private:
	static constexpr uint16_t GetSlice2(const uint16_t column)
	{
		return (column >> 2) & PORT_IO_MASK;
	}

	static constexpr uint16_t GetSlice1(const uint16_t column, const uint8_t row)
	{
		return (uint16_t(row & 0b11111)
			| (uint16_t((row >> 5) & 0b111) << 7)
			| (uint16_t(column & 0b11) << 5)) & PORT_IO_MASK;
	}
};
#endif