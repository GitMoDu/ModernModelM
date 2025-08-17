#ifndef _IO_EXPANDER_MCP23017_h
#define _IO_EXPANDER_MCP23017_h

#include <Wire.h>
#include <MCP23017.h>

#include "IoPort.h"

/// <summary>
/// Depends on https://github.com/RobTillaart/MCP23017_RT
/// 
/// Implements IoPort for MCP23017 with 14 GPIOs.
/// Mapping 
///		PA0..6 -> 0..6
///		PB0..6 -> 7..14
/// Pins A7 and B7 are disabled, abstracted away.
/// Requires Wire.begin() before setup.
/// </summary>
/// <typeparam name="Pin">Device pin mapping enum.</typeparam>
template<typename Pin>
class IoExpanderMcp23017 : public IoPort
{
private:
	static constexpr uint16_t IO_MASK = 0b0011111111111111;
	static constexpr uint8_t PORT_IO_MASK = 0b01111111;
	static constexpr uint8_t UN_IO_MASK = ~PORT_IO_MASK;

public:
	static constexpr uint8_t IoCount = 14;

private:
	MCP23017 Driver;

public:
	IoExpanderMcp23017(const uint8_t i2cAddress, TwoWire& wire)
		: IoPort()
		, Driver(i2cAddress, &wire)
	{
	}

	////TODO:
	//virtual void SetForInterrupt() final
	//{
	//	//attachInterrupt(digitalPinToInterrupt(InterruptPin), INPUT_)
	//}

	////TODO:
	//virtual void RestorePins() final
	//{
	//	//detachInterrupt(InterruptPin);
	//}


	bool Setup()
	{
		if (Driver.begin(true))
		{
			// Make the PCB layout make sense.
			Driver.reverse16ByteOrder(true);

			return true;
		}

		return false;
	}
	//bool Setup(void (*onIoInterrupt)(), const uint8_t interruptPin1, const uint8_t interruptPin2)


public:
	uint8_t GetIoCount() const final
	{
		return IoCount;
	}

	void SetMode(const uint16_t direction, const uint16_t pullUp) final
	{
		// Direction is inverted in logic: 1 - Pin as input; 0 - Pin as output.
		Driver.pinMode16(uint16_t(UN_IO_MASK | GetSliceA(~direction))
			| uint16_t(UN_IO_MASK | GetSliceB(~direction) << 8));
		Driver.setPullup16(uint16_t(0 | GetSliceA(pullUp))
			| uint16_t(UN_IO_MASK | GetSliceB(pullUp) << 8));
	}

	void SetState(const uint16_t state) final
	{
		Driver.write16(uint16_t(~UN_IO_MASK & GetSliceA(state))
			| (uint16_t(~UN_IO_MASK & GetSliceB(state)) << 8));
	}

	uint16_t GetState() final
	{
		const uint16_t portAB = Driver.read16();

		return GetCode(portAB);
	}

private:
	// Bits 0 to 6.
	static constexpr uint8_t GetSliceA(const uint16_t code)
	{
		return code & PORT_IO_MASK;
	}

	// Bits 7 to 13.
	static constexpr uint8_t GetSliceB(const uint16_t code)
	{
		return (((code >> 7) & 0b1) | ((uint8_t(code >> 8) << 1))) & PORT_IO_MASK;
	}

	// Converts the 16-bit portAB value to a 14-bit code.
	static constexpr uint16_t GetCode(const uint16_t portAB)
	{
		return GetCode(portAB, portAB >> 8);
	}

	// Converts the 7-bit portA and 7-bit portB values to a 14-bit code.
	static constexpr uint16_t GetCode(const uint8_t portA, const uint8_t portB)
	{
		return uint16_t(portA & PORT_IO_MASK)
			| (uint16_t(portB & PORT_IO_MASK) << 7);
	}
};
#endif