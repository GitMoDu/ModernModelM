// IoExpanderMcp23017.h

#ifndef _IO_EXPANDER_MCP23017_h
#define _IO_EXPANDER_MCP23017_h

#include <Wire.h>
#include <MCP23017.h>

#include "IoPort.h"

/// <summary>
/// Depends on https://github.com/blemasle/arduino-mcp23017
/// 
/// Implements IoPort for MCP23017 with 14 GPIOs.
/// Mapping PA6..0 .. PB0..6 
/// Pins A7 and B7 are disabled, abstracted away.
/// Requires Wire.begin() before setup.
/// </summary>
/// <typeparam name="Pin">Device pin mapping enum.</typeparam>
template<typename Pin>
class IoExpanderMcp23017 : public virtual IoPort
{
private:
	//static constexpr uint16_t TOTAL_IO_MASK = (uint16_t)~((uint16_t)1 << 14);
	static constexpr uint8_t PORT_IO_MASK = (uint8_t)~((uint8_t)1 << 7);
	static constexpr uint8_t UN_IO_MASK = ((uint8_t)1 << 7);

	static constexpr uint8_t INVERTED_CONFIG = 0;

public:
	static constexpr uint8_t IoCount = 14;

private:
	MCP23017 Driver;

public:
	IoExpanderMcp23017(const uint8_t i2cAddress)
		: IoPort()
		, Driver(i2cAddress)
	{}

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


	void Setup()
	{
		Driver.init();
	}
	//void Setup(void (*onIoInterrupt)(), const uint8_t interruptPin1, const uint8_t interruptPin2)


public:
	virtual const uint8_t GetIoCount() final
	{
		return IoCount;
	}

	virtual void SetMode(const uint16_t direction, const uint16_t pullUp) final
	{
		// *1 = Pin is configured as an input.
		// *0 = Pin is configured as an output.
		Driver.portMode(MCP23017Port::A, GetSliceA(~direction) | UN_IO_MASK, INVERTED_CONFIG);
		Driver.portMode(MCP23017Port::B, GetSliceB(~direction) | UN_IO_MASK, INVERTED_CONFIG);
	}

	virtual void SetState(const uint16_t state) final
	{
		Driver.write(((uint16_t)(GetSliceA(state) & ~UN_IO_MASK)) | ((uint16_t)(GetSliceB(state) & ~UN_IO_MASK) << 8));
	}

	virtual const uint16_t GetState() final
	{
		return GetCode(Driver.read());
	}

private:
	static constexpr uint8_t GetReversed(const uint8_t code)
	{
		return (((code >> 0) & 1) << 7) |
			(((code >> 1) & 1) << 6) |
			(((code >> 2) & 1) << 5) |
			(((code >> 3) & 1) << 4) |
			(((code >> 4) & 1) << 3) |
			(((code >> 5) & 1) << 2) |
			(((code >> 6) & 1) << 1) |
			(((code >> 7) & 1) << 0);
	}

	static constexpr uint8_t GetSliceA(const uint16_t code)
	{
		return (GetReversed((uint8_t)code) >> 1) & PORT_IO_MASK;
	}

	static constexpr uint8_t GetSliceB(const uint16_t code)
	{
		return ((GetReversed(code) >> 6) & 1)
			| ((code >> 7) & PORT_IO_MASK);
	}

	static constexpr uint16_t GetCode(const uint16_t portAB)
	{
		return GetCode(portAB, portAB >> 8);
	}

	static constexpr uint16_t GetCode(const uint8_t portA, const uint8_t portB)
	{
		return ((uint16_t)(GetReversed(portA) >> 1) & PORT_IO_MASK)
			| ((uint16_t)(portB & PORT_IO_MASK) << 7);
	}
};
#endif