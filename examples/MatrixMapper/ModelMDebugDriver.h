#ifndef _MODEL_M_DEBUG_DRIVER_h
#define _MODEL_M_DEBUG_DRIVER_h

#include <ModelMDriver.h>

class ModelMDebugDriver : public ModelMDriver
{
public:
	ModelMDebugDriver(IoPort* io1, IoPort* io2)
		: ModelMDriver(io1, io2)
	{
	}

	void LogDurations()
	{
		uint32_t start, end;
		start = micros();
		ReadByRowDrive();
		end = micros();
		SetInputModeAll();
		Serial.print(F("Row drive read took "));
		Serial.print(end - start);
		Serial.println(F(" us"));

		start = micros();
		ReadByColumnDrive();
		end = micros();
		SetInputModeAll();
		Serial.print(F("Column drive read took "));
		Serial.print(end - start);
		Serial.println(F(" us"));

		start = micros();
		ReadByColumnDrive();
		ReadByRowDrive();
		SetInputModeAll();
		end = micros();

		Serial.print(F("Full read took "));
		Serial.print(end - start);
		Serial.println(F(" us"));
	}

	void LogLiveState()
	{
		ReadByColumnDrive();
		ReadByRowDrive();
		SetInputModeAll();

		LogColumnState();
		LogRowState();
	}

	void LogRows()
	{
		ReadByRowDrive();
		SetInputModeAll();

		Serial.println(F("Colum Match"));
		bool found = false;
		for (uint_fast8_t i = 0; i < RowCount; i++)
		{
			for (uint_fast8_t j = 0; j < ColumnCount; j++)
			{
				if (ColumnMatch(i, j))
				{
					found = true;
					Serial.print(F("\t("));
					Serial.print(i);
					Serial.print(',');
					Serial.print(j);
					Serial.println(')');
				}
			}
		}

		if (!found)
		{
			Serial.print(F("\tNone"));
		}
		Serial.println();
	}

	void LogColumns()
	{
		ReadByColumnDrive();
		SetInputModeAll();

		Serial.println(F("Row Match"));
		bool found = false;
		for (uint_fast8_t i = 0; i < ColumnCount; i++)
		{
			for (uint_fast8_t j = 0; j < RowCount; j++)
			{
				if (RowMatch(i, j))
				{
					found = true;
					Serial.print(F("\t("));
					Serial.print(i);
					Serial.print(',');
					Serial.print(j);
					Serial.println(')');
				}
			}
		}
		

		if (!found)
		{
			Serial.print(F("\tNone"));
		}
		Serial.println();
	}

	void LogColumnState()
	{
		Serial.println(F("Column State"));
		for (size_t i = 0; i < RowCount; i++)
		{
			Serial.print('\t');
			Serial.print('[');
			for (size_t j = 0; j < ColumnCount; j++)
			{
				Serial.print((uint8_t)((ColumnState[i] >> j) & 1));
			}
			Serial.println(']');
		}
	}

	void LogRowState()
	{
		Serial.println(F("Row State"));
		for (size_t i = 0; i < ColumnCount; i++)
		{
			Serial.print('\t');
			Serial.print('[');
			for (size_t j = 0; j < RowCount; j++)
			{
				Serial.print((uint16_t)((RowState[i] >> j) & 1));
			}
			Serial.println(']');
		}
	}
};

#endif

