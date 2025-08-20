# Modern Model M

IBM Model M Keyboard conversion to combined USB/BLE device using [RetroBle](https://github.com/GitMoDu/RetroBLE) device framework.

- Original Portuguese PT-PT layout
- All keys working including _Pause/Break_
- Indicator lights _NumLock_ and _CapsLock_ working correctly
- _ScrollLock_ light used as On/Off light (dimmed when in BLE mode)

# Device
  - [Host nRF52840](https://github.com/GitMoDu/ModernModelM/tree/master/examples/ModernModelM)
  - [Slave Lights driver AVR328P](https://github.com/GitMoDu/ModernModelM/tree/master/examples/KeyboardLightsSlave)

# Dependencies
  - [RetroBle](https://github.com/GitMoDu/RetroBLE) - USB/BLE device framework.
  - [Template I2C](https://github.com/GitMoDu/TemplateI2C) - I2C host and driver abstraction.
  - [MCP23017_RT](https://github.com/RobTillaart/MCP23017_RT) - MCP23017 I2C driver
