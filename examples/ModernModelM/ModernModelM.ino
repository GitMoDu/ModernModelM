/*
* Retro BLE Pad implementation for Model M keyboard PT-PT layout.
*
* Retro BLE features
*	- USB HID interface, takes precedence over any BLE connection.
*	- BLE interface with ~10ms update period.
*	- Battery level state.
*	- State LEDs.
*
* Dependencies:
*	- Core https://github.com/Seeed-Studio/OSHW-XIAO-Series
*	- BLE https://github.com/adafruit/Adafruit_nRF52_Arduino/blob/master/libraries/Bluefruit52Lib/src/bluefruit.h
*	- USB https://github.com/adafruit/Adafruit_TinyUSB_Arduino
*	- MCP23017 I2C Driver https://github.com/RobTillaart/MCP23017_RT
*/

#define DEBUG
#define ENABLE_COORDINATOR


#define _TASK_OO_CALLBACKS
#include <TScheduler.hpp>

#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include <bluefruit.h>

#include <TScheduler.hpp>


#include <RetroBle.h>

#include <IoExpanderMcp23017.h>

#include "ModelMKeyboardHidMap.h"
#include "ModelMSleepyKeyboard.h"

#include "KeyboardMapperTask.h"

#include "ModelMIndicator.h"

#include "Device.h"


// Process scheduler.
TS::Scheduler SchedulerBase;

// Driver for onboard battery manager.
Bq25100Driver<Device::BMS::UpdatePeriodMillis,
	Device::BMS::HistorySize,
	Device::BMS::Pin,
	Device::BMS::Calibration> BMS(SchedulerBase);

// USB driver.
UsbHidKeyboard UsbKeyboard{};
UsbPeripheral UsbDev{};

// Bluetooth driver.
BLEHidAdafruit BleKeyboard{};
BlePeripheral BleDev{};

// IO Expanders drivers.
IoExpanderMcp23017<Device::IoExpander::Io1::Pin> Io1(Device::IoExpander::Io1::Address, Wire);
IoExpanderMcp23017<Device::IoExpander::Io2::Pin> Io2(Device::IoExpander::Io2::Address, Wire);

// Keyboard driver.
using KeyboardDriverType = ModelMSleepyKeyboard<Device::IoExpander::Io1::Pin, Device::IoExpander::Io2::Pin>;
KeyboardDriverType KeyboardDriver(&Io1, &Io2,
	Device::Keyboard::KeyMap::ColumnMap, Device::Keyboard::KeyMap::ColumnMapSize,
	Device::Keyboard::KeyMap::RowMap, Device::Keyboard::KeyMap::RowMapSize);

// Keyboard Lights I2C slave driver.
KeyboardIndicator ModelMIndicator(Wire);

// Keyboard read and notify task.
KeyboardMapperTask<KeyboardDriverType, Device::BLE::LONG_PRESS_POWER_OFF_PERIOD_MILLIS>KeyboardMapper(
	SchedulerBase,
	KeyboardDriver,
	UsbKeyboard, BleKeyboard,
	Device::USB::UpdatePeriodMillis, Device::BLE::UpdatePeriodMillis);

// Coordinator task.
UsbBleCoordinator Coordinator(SchedulerBase, &BMS, &ModelMIndicator, &KeyboardMapper, UsbDev, BleDev);

void setup()
{
#if defined(DEBUG)
	Serial.begin(Device::Debug::SERIAL_BAUD_RATE);

#if !defined(ENABLE_COORDINATOR)
	// Blocking wait for connection when debug mode is enabled via IDE
	while (!Serial) delay(10);

	Serial.println(F("ModelM start"));
#endif

	Serial.flush();
#endif

	// Disable unused pins.
	PinSetup();

	// Battery management setup.
	BMS.Setup();
	BMS.Start();

	// Setup I2C.
	Wire.begin();
	Wire.setClock(Device::I2C0::Clock);

	// Setup IO expanders and keyboard driver.
	
	if (!Io1.Setup()
		|| !Io2.Setup()
		|| !KeyboardDriver.Setup())
	{
#if defined(DEBUG)
		Serial.println(F("Error starting KeyboardDriver."));
#endif
	}

#if defined(ENABLE_COORDINATOR)
	// BLE setup.
	BleDev.Setup(BleKeyboard,
		connect_callback, disconnect_callback,
		advertise_stop_callback, ble_event_callback,
		Device::BLE::Appearance,
		Device::BLE::ConnectionIntervalMin,
		Device::BLE::ConnectionIntervalMax,
		Device::Name,
		Device::Version::Name);

	// USB setup.
	UsbDev.Setup(Device::Name, Device::Version::Code,
		Device::USB::UpdatePeriodMillis,
		Device::USB::ProductId);
	UsbKeyboard.Setup(Device::Name, get_report_callback, set_report_callback);

	// Start the device coordinator.
	if (!Coordinator.Start())
	{
		Serial.println(F("Error starting coordinator."));
	}
#endif
}

#if !defined(ENABLE_COORDINATOR)
hid_keyboard_report_t Report{};
#endif

void loop()
{
#if defined(ENABLE_COORDINATOR)
	SchedulerBase.execute();
#endif
}

void PinSetup()
{
	for (uint8_t i = 0; i < sizeof(Device::Unused::Pins); i++)
	{
		const uint8_t pin = Device::Unused::Pins[i];
		pinMode(pin, INPUT);
		digitalWrite(pin, LOW);
	}
}

// Event callbacks.
void ble_event_callback(ble_evt_t* bleEvent)
{
	BleDev.OnBleEventInterrupt(bleEvent);
}

void advertise_stop_callback()
{
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
	BleDev.OnDisconnectInterrupt(conn_handle, reason);
}

void connect_callback(uint16_t conn_handle)
{
	BleDev.OnConnectInterrupt(conn_handle);
}

uint16_t get_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
	return UsbKeyboard.OnGetReportInterrupt(report_id, report_type, buffer, reqlen);
}

void set_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
	UsbKeyboard.OnSetReportInterrupt(report_id, report_type, buffer, bufsize);
}