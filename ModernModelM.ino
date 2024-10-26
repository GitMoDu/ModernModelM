/*
* Retro BLE Pad implementation for Model M keyboard PT-PT layout.
*
* Retro BLE features
*	- USB HID interface, takes precedence over any BLE connection.
*	- BLE interface with ~12ms update period.
*	- Battery level state.
*	- State LEDs.
*
* Dependencies:
*	- Core https://github.com/Seeed-Studio/OSHW-XIAO-Series
*	- BLE https://github.com/adafruit/Adafruit_nRF52_Arduino/blob/master/libraries/Bluefruit52Lib/src/bluefruit.h
*	- USB https://github.com/adafruit/Adafruit_TinyUSB_Arduino
*	- MCP23017 I2C Driver https://github.com/blemasle/arduino-mcp23017
*/

#define DEBUG


#define _TASK_OO_CALLBACKS
#include <TScheduler.hpp>

#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include <bluefruit.h>

#include <TScheduler.hpp>


#include <Arduino.h>
#include <RetroBle.h>

#include "Device.h"


#include "IoExpanderMcp23017.h"

#include "ModelMDriver.h"
#include "ModelMSleepyKeyboard.h"

#include "KeyboardMapperTask.h"


// Process scheduler.
TS::Scheduler SchedulerBase;
// 

//// HARDWARE DRIVERS
// Driver for onboard RGB LED.
LedDriver<Device::Led::Pin, Device::Led::OnState> Led{};

// Driver for onboard battery manager.
Bq25100Driver<Device::BMS::UpdatePeriodMillis,
	Device::BMS::HistorySize,
	Device::BMS::Pin,
	Device::BMS::Calibration> BMS(SchedulerBase);

//// USB driver.
//UsbHidKeyboard UsbKeyboard{};
//UsbPeripheral UsbDev{};
//
//// Bluetooth driver.
//BLEHidAdafruit BleKeyboard{};
//BlePeripheral BleDev{};

// IO Expanders drivers.
IoExpanderMcp23017<Device::IoExpander::Io1::Pin> Io1(Device::IoExpander::Io1::Address);
IoExpanderMcp23017<Device::IoExpander::Io2::Pin> Io2(Device::IoExpander::Io2::Address);

// Keyboard driver.
ModelMSleepyKeyboard KeyboardDriver(&Io1, &Io2);
//

//// SOFTWARE TASKS
// Keyboard read and notify task.
//KeyboardMapperTask<Device::BLE::LONG_PRESS_POWER_OFF_PERIOD_MILLIS>KeyboardMapper(
//	SchedulerBase,
//	KeyboardDriver,
//	UsbKeyboard, BleKeyboard,
//	Device::USB::UpdatePeriodMillis, Device::BLE::UpdatePeriodMillis);

// LED animation task.
LedAnimatorTask PadLights(SchedulerBase, &Led);

// Coordinator task.
//UsbBleCoordinator Coordinator(SchedulerBase, &BMS, PadLights, nullptr, UsbDev, BleDev);
//

void setup()
{
#if defined(DEBUG)
	Serial.begin(Device::Debug::SERIAL_BAUD_RATE);

	// Blocking wait for connection when debug mode is enabled via IDE
	while (!Serial) delay(10);

	Serial.println(F("ModelM start"));
	Serial.flush();
#endif

	// Disable unused pins.
	PinSetup();

	// Battery management pin setup.
	BMS.Setup();

	// Onboard LED pin setup.
	Led.Setup();

	// Setup I2C.
	Wire.begin();
	Wire.setClock(400000);


	// Setup IO expanders and keyboard driver.
	Io1.Setup();
	Io2.Setup();
	if (!KeyboardDriver.Setup())
	{
		Serial.println(F("Error starting KeyboardDriver."));
	}

	//// BLE setup.
	//BleDev.Setup(BleKeyboard,
	//	connect_callback, disconnect_callback,
	//	advertise_stop_callback, ble_event_callback,
	//	Device::BLE::Appearance,
	//	Device::Name,
	//	Device::Version::Name);

	//// USB setup.
	//UsbDev.Setup(Device::Name, Device::Version::Code,
	//	Device::USB::UpdatePeriodMillis,
	//	Device::USB::ProductId);
	//UsbKeyboard.Setup(Device::Name, get_report_callback, set_report_callback);

	//// Start the device coordinator.
	//if (!Coordinator.Start())
	//{
	//	Serial.println(F("Error starting coordinator."));
	//}
}

void ReadFast(hid_keyboard_report_t& report)
{
	uint32_t start = micros();
	KeyboardDriver.GetFastReport(report);
	uint32_t end = micros();

	Serial.print(F("Keyboard fast read took "));
	Serial.print(end - start);
	Serial.println(F(" us"));
}

void ReadFull(hid_keyboard_report_t& report)
{
	uint32_t start = micros();
	KeyboardDriver.GetFullReport(report);
	uint32_t end = micros();

	Serial.print(F("Keyboard full read took "));
	Serial.print(end - start);
	Serial.println(F(" us"));
}

void loop()
{
	hid_keyboard_report_t report;

	//ReadFast(report);
	ReadFull(report);

	KeyboardDriver.LogState();

	delay(10);
	//SchedulerBase.execute();
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
//void ble_event_callback(ble_evt_t* bleEvent)
//{
//	BleDev.OnBleEventInterrupt(bleEvent);
//}
//
//void advertise_stop_callback()
//{
//}
//
//void disconnect_callback(uint16_t conn_handle, uint8_t reason)
//{
//	BleDev.OnDisconnectInterrupt(conn_handle, reason);
//}
//
//void connect_callback(uint16_t conn_handle)
//{
//	BleDev.OnConnectInterrupt(conn_handle);
//}
//
//uint16_t get_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
//{
//	return UsbKeyboard.OnGetReportInterrupt(report_id, report_type, buffer, reqlen);
//}
//
//void set_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
//{
//	UsbKeyboard.OnSetReportInterrupt(report_id, report_type, buffer, bufsize);
//}