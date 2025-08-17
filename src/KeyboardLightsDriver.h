// KeyboardLightsDriver.h

#ifndef _KEYBOARD_LIGHTS_DRIVER_h
#define _KEYBOARD_LIGHTS_DRIVER_h

#include <KeyboardLights.h>
#include <TemplateI2cDriver.h>

namespace KeyboardLights
{
	class I2cDriver
		: public TemplateI2c::I2cDriver<Api::Address, Api::Id>
	{
	private:
		using Base = TemplateI2c::I2cDriver<Api::Address, Api::Id>;

	public:
		I2cDriver(TwoWire& wire)
			: Base(wire)
		{
		}

		const bool SetState(const ModeEnum mode, const bool numLock, const bool capsLock)
		{
			const uint8_t code = GetCodeFromState(mode, numLock, capsLock);

			return Base::SendMessage(Api::Requests::SetState::Header, &code, Api::Requests::SetState::PayloadSize);
		}
	};
}
#endif

