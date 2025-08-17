// KeyboardLightsI2cSlave.h

#ifndef _KEYBOARD_LIGHTS_I2C_SLAVE_h
#define _KEYBOARD_LIGHTS_I2C_SLAVE_h

#include <TemplateAsyncI2cSlave.h>
#include <KeyboardLights.h>

namespace KeyboardLights
{
	class I2cSlave : public TemplateI2c::I2cSlave<Api::Address, Api::Id>
	{
	private:
		using Base = TemplateI2c::I2cSlave<Api::Address, Api::Id>;

	protected:
		using Base::Incoming;

	private:
		IKeyboardLights& Lights;

	public:
		I2cSlave(TS::Scheduler& scheduler, TwoWire& wire, IKeyboardLights& indicator)
			: Base(wire)
			, Lights(indicator)
		{
		}

		void Reset()
		{
			Base::ResetDevice();
		}

		void OnI2cReceived(const uint8_t size)
		{
			const uint8_t header = Incoming[(uint8_t)TemplateI2c::MessageDefinition::HeaderIndex];

			switch (header)
			{
			case Api::Requests::SetState::Header:
				Lights.SetState(
					GetModeFromCode(Incoming[(uint8_t)TemplateI2c::MessageDefinition::PayloadIndex]),
					GetNumLockFromCode(Incoming[(uint8_t)TemplateI2c::MessageDefinition::PayloadIndex]),
					GetCapsLockFromCode(Incoming[(uint8_t)TemplateI2c::MessageDefinition::PayloadIndex]));
				break;
			default:
				Base::OnSystemMessageReceived(size);
				break;
			}
		}

		void OnReceiveInterrupt(const uint8_t size)
		{
			OnI2cReceived(Base::ReadIncoming(size));
		}
	};
}
#endif

