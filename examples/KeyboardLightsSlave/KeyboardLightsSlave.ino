
#define _TASK_OO_CALLBACKS
#define _TASK_SLEEP_ON_IDLE_RUN
#include <TScheduler.hpp>

#include <avr\power.h>
#include <avr\sleep.h>
#include <avr\wdt.h>

#include <Wire.h>

#include "Device.h"


#include "KeyboardLightsAnimator.h"
#include "KeyboardLightsI2cSlave.h"


// Process scheduler.
TS::Scheduler SchedulerBase{};
//

// Lights animator.
KeyboardLightsAnimator<Device::Led::BrightnessRange, Device::Led::Pin, Device::Led::OnState> LightsAnimator(SchedulerBase, Device::Led::AnimatePeriod);

KeyboardLights::I2cSlave I2cSlave(SchedulerBase, Wire, LightsAnimator);

uint32_t LastActivity = 0;

void Halt()
{
	while (true)
		;;
}

void setup()
{
	SetupLowPower();

	// Setup Lights and pins.
	LightsAnimator.Start();

	// Setup I2C Slave.
	if (!I2cSlave.Setup(OnI2cReceive, OnI2cRequest))
	{
		Halt();
	}
}

void loop()
{
	SchedulerBase.execute();
	if (SchedulerBase.getActiveTasks() == 0)
	{
		if ((millis() - LastActivity) > 20)
		{
			// Devices are stopped.
			LightsAnimator.Stop();

			ExecuteLowPowerMode();

			// Systems must be enabled manually after sleep.
			LightsAnimator.Start();

			LastActivity = millis();
		}
	}
	else
	{
		LastActivity = millis();
	}
}

void OnI2cReceive(int size)
{
	I2cSlave.OnReceiveInterrupt(size);
}

void OnI2cRequest()
{
	I2cSlave.OnRequestInterrupt();
}

/// <summary>
/// ~20uA @ 3.3V, wakes up with I2C on 1st try.
/// </summary>
void ExecuteLowPowerMode()
{
#if defined(ATTINYX5)
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	noInterrupts();						// timed sequence follows
	sleep_enable();

	sleep_bod_disable();				// turn off brown-out enable in software
	interrupts();						// guarantees next instruction executed
	sleep_cpu();						// sleep within 3 clock cycles of above
	sleep_disable();
	interrupts();
#endif
}

void SetupLowPower()
{
#if defined(ATTINYX5)

	ADCSRA &= ~bit(ADEN); // disable the ADC
	ACSR |= _BV(ACD); // Disable analog comparator, saves ~35uA TODO: validate

	power_timer1_disable();
	power_adc_disable();
	wdt_disable();
#endif

	pinMode(PB5, INPUT); // ATTiny RST.
}

/// <summary>
/// ~20uA @ 3.3V, wakes up with I2C on 1st try.
/// </summary>
void ExecuteLowPowerModeNew()
{
	// Disable interrupts while preparing to sleep.
	noInterrupts();	

#if defined(ATTINYX5)
	// Disable watchdog if enabled.
	MCUSR = 0;
	wdt_disable();

	// Full sleep, maximum power save, only wake on pin change.
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	// Turn off brown-out enable in software
	sleep_bod_disable();

	noInterrupts();		// Timed sequence follows.
	sleep_enable();		// Sets the Sleep Enable bit in the MCUCR Register (SE BIT).
	interrupts();		// Guarantees next instruction executed
	sleep_cpu();		// Sleep within 3 clock cycles of above.

	// ZZZzzzZZZ...

	sleep_disable();	// Clear Sleep bit.
#endif

	interrupts();		// Enable interrupts and get back to business.
}

void SetupLowPowerNew()
{
#if defined(ATTINYX5)
	ADCSRA &= ~bit(ADEN); // disable the ADC
	ACSR |= _BV(ACD); // Disable analog comparator, saves ~35uA TODO: validate

	power_timer1_disable();
	power_adc_disable();
#endif

	pinMode(PB5, INPUT); // ATTiny RST.
}
