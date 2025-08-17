/*
* Model M Matrix live reader and logger for  mapping discovery.
*
* Dependencies:
*	- Core https://github.com/Seeed-Studio/OSHW-XIAO-Series
*	- MCP23017 I2C Driver https://github.com/RobTillaart/MCP23017_RT
*/

#define DEBUG

#define _TASK_OO_CALLBACKS
#include <TScheduler.hpp>

#if defined(ARDUINO_Seeed_XIAO_nRF52840_Sense) || defined(ARDUINO_Seeed_XIAO_nRF52840)
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#endif

#include "Device.h"

#include <IoExpanderMcp23017.h>

#include "ModelMDebugDriver.h"

// IO Expanders drivers.
IoExpanderMcp23017<Device::IoExpander::Io1::Pin> Io1(Device::IoExpander::Io1::Address, Wire);
IoExpanderMcp23017<Device::IoExpander::Io2::Pin> Io2(Device::IoExpander::Io2::Address, Wire);

// Keyboard driver.
ModelMDebugDriver KeyboardDriver(&Io1, &Io2);

void halt()
{
	while (true)
		;;
}

void bootError()
{
	Serial.println(F("Boot error, stopped."));
	halt();
}

void setup()
{
	Serial.begin(Device::Debug::SERIAL_BAUD_RATE);
	while (!Serial)
		;;

	Serial.println(F("Matrix Mapper start"));
	Serial.flush();

	// Setup I2C.
	Wire.begin();
	Wire.setClock(400000);

	// Setup IO expanders and keyboard driver.
	if (!Io1.Setup()
		|| !Io2.Setup())
	{
		Serial.println(F("Error starting Io Drivers."));
		bootError();
	}

	if (!KeyboardDriver.Setup())
	{
		Serial.println(F("Error starting KeyboardDriver."));
		bootError();
	}

	KeyboardDriver.LogDurations();
	Serial.flush();
}


// Repeatedly log and read the keyboard through both methods
//  of driving the rows and columns.
void loop()
{
	KeyboardDriver.LogColumns();
	KeyboardDriver.LogRows();
	KeyboardDriver.SetInputModeAll();

	delay(200);
}