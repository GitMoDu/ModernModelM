#ifndef _MAPPED_KEY_h
#define _MAPPED_KEY_h

#include <stdint.h>

/// <summary>
/// Encodes coordinate and key code value in a uint16_t value.
/// Coordinates range (x,y) [0;15]
/// </summary>
namespace MappedKey
{
	static constexpr uint8_t IMax = 15;
	static constexpr uint8_t CodeSize = sizeof(uint16_t);

	template<uint8_t item, uint8_t index, uint8_t keyCode>
	static constexpr uint16_t GetCode()
	{
		return (uint16_t)(item & IMax)
			| ((uint16_t)(index & IMax) << 4)
			| ((uint16_t)(keyCode) << 8);
	}

	static constexpr uint8_t Item(const uint16_t code)
	{
		return (uint8_t)code & IMax;
	}

	static constexpr uint8_t Index(const uint16_t code)
	{
		return ((uint8_t)code >> 4) & IMax;
	}

	static constexpr uint8_t KeyCode(const uint16_t code)
	{
		return (uint8_t)(code >> 8);
	}
};

#endif