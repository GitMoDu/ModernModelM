

#define DEBUG_TEMPLATE_I2C

#define SERIAL_BAUD_RATE 115200


#include <Wire.h>

#include <KeyboardLightsDriver.h>

KeyboardLights::I2cDriver Driver(Wire);

void setup()
{
	Serial.begin(SERIAL_BAUD_RATE);
	while (!Serial)
		;
	Serial.println(F("KeyboardLights I2C Slave Tester"));
	Serial.println();
	Serial.println(F("Serial Tester:"));
	Serial.println(F("\tn - NumLock Toggle"));
	Serial.println(F("\tc - CapsLock Toggle"));
	Serial.println(F("\to - Lights Off"));
	Serial.println(F("\tb - BLE Connected"));
	Serial.println(F("\ts - BLE Searching"));
	Serial.println(F("\tu - USB Connected"));
	Serial.println();

	Wire.begin();
	Wire.setClock((uint32_t)400000);
}

KeyboardLights::ModeEnum Mode = KeyboardLights::ModeEnum::Off;
bool NumLock = false;
bool CapsLock = false;


void loop()
{
	if (Serial.available())
	{
		const int8_t in = Serial.read();

		switch (in)
		{
		case 'n':
		case 'N':
			NumLock = !NumLock;
			Driver.SetState(Mode, NumLock, CapsLock);
			Serial.println(F("NumLock Toggle"));
			break;
		case 'c':
		case 'C':
			CapsLock = !CapsLock;
			Driver.SetState(Mode, NumLock, CapsLock);
			Serial.println(F("CapsLock Toggle"));
			break;
		case 'o':
		case 'O':
			Mode = KeyboardLights::ModeEnum::Off;
			Driver.SetState(Mode, NumLock, CapsLock);
			Serial.println(F("Lights Off"));
			break;
		case 'b':
		case 'B':
			Mode = KeyboardLights::ModeEnum::Ble;
			Driver.SetState(Mode, NumLock, CapsLock);+
			Serial.println(F("BLE Connected"));
			break;
		case 'u':
		case 'U':
			Mode = KeyboardLights::ModeEnum::Usb;
			Driver.SetState(Mode, NumLock, CapsLock);
			Serial.println(F("USB Connected"));
			break;
		case 's':
		case 'SC':
			Mode = KeyboardLights::ModeEnum::Searching;
			Driver.SetState(Mode, NumLock, CapsLock);
			Serial.println(F("BLE Searching"));
			break;
		default:
			break;
		}
	}
}
