#ifndef _IO_PORT_h
#define _IO_PORT_h

#include <stdint.h>

/// <summary>
/// Class interface for IO ports.
/// </summary>
class IoPort
{
public:
	virtual uint8_t GetIoCount() const = 0;

public:
	virtual void SetMode(const uint16_t direction, const uint16_t pullUp) = 0;
	virtual void SetState(const uint16_t state) = 0;
	virtual uint16_t GetState() = 0;

	//virtual void DisableIRQ() = 0;
	virtual void SetIRQ(const uint16_t mask, const uint8_t mode) = 0;

	virtual void AttachInterrupt(void (*irqInterruptA)() = nullptr, void (*irqInterruptB)() = nullptr) = 0;
};

#endif