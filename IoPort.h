// IoPort.h

#ifndef _IO_PORT_h
#define _IO_PORT_h

#include <stdint.h>

/// <summary>
/// Class interface for IO ports.
/// </summary>
class IoPort
{
public:
	virtual const uint8_t GetIoCount() { return 0; }

public:
	virtual void SetMode(const uint16_t direction, const uint16_t pullUp) {}
	virtual void SetState(const uint16_t state) {}
	virtual const uint16_t GetState() { return 0; }

	//TODO:
	//virtual void SetForInterrupt() {}

	//TODO:
	//virtual void RestorePins() {}
};

#endif