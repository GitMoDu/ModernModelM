// KeyboardLights.h

#ifndef _KEYBOARDLIGHTS_h
#define _KEYBOARDLIGHTS_h

#include <TemplateI2c.h>

namespace KeyboardLights
{
	enum class ModeEnum : uint8_t
	{
		Off = 0,
		Ble = 1,
		Usb = 2,
		Searching = 3,
		ModeEnumCount
	};

	struct IKeyboardLights
	{
		virtual void SetState(const ModeEnum mode, const bool numLock, const bool capsLock) {}
	};

	static constexpr ModeEnum GetModeFromCode(const uint8_t code)
	{
		return (ModeEnum)(code & 0b00000011);
	}

	static constexpr bool GetCapsLockFromCode(const uint8_t code)
	{
		return (code >> 6) & 1;
	}

	static constexpr bool GetNumLockFromCode(const uint8_t code)
	{
		return (code >> 7) & 1;
	}

	static constexpr uint8_t GetCodeFromState(const ModeEnum mode, const bool numLock, const bool capsLock)
	{
		return ((numLock * 1) << 7)
			| ((capsLock * 1) << 6)
			| (uint8_t)mode;
	}

	namespace Api
	{
		static constexpr uint32_t Id = 834312801;
		static constexpr uint8_t Address = 55;

		struct Requests : TemplateI2c::Api::Requests
		{
			/// <summary>
			/// Payload is state code.
			/// </summary>
			struct SetState : public TemplateI2c::Api::Request<Requests::UserHeaderStart, 1> {};
		};
	}
}
#endif