# SAMCO Prow Enhanced - Arduino Powered Light Gun

Based on the 4IR Beta "Big Code Update" SAMCO project from https://github.com/samuelballantyne/IR-Light-Gun

## Enhancements
- Increased precision for maths and mouse pointer position
- Glitch-free DFRobot positioning (DFRobotIRPositionEx library)
- IR camera sensitivity adjustment (DFRobotIRPositionEx library)
- Faster IIC clock option for IR camera (DFRobotIRPositionEx library)
- Optional averaging modes can be enabled to slightly reduce mouse position jitter
- Enhanced button debouncing and handling (LightgunButtons library)
- Modified AbsMouse to be a 5 button device (AbsMouse5 library)
- Multiple calibration profiles
- Save settings and calibration profiles to flash memory (SAMD) or EEPROM (ATmega32U4)
- Built in Processing mode for use with the SAMCO Processing sketch

## Requirements
- Adafruit ItsyBitsy M0, M4, RP2040 (or any SAMD21, SAMD51, or RP2040 dev board)
- DFRobot IR Positioning Camera (SEN0158)
- 4 IR LED emitters
- Arduino development environment

With minor modifications it should work with any SAMD21, SAMD51, or RP2040 boards. See the SAMCO project for build details: https://github.com/samuelballantyne/IR-Light-Gun

The ATmega32U4 is no longer officially supported.

## Installation
1. Ensure you have the Arduino development environment installed with support for your board.
2. If you use an ItsyBitsy M0 or M4 then install the **Adafruit DotStar** and **Adafruit SPIFlash** libraries in the Library Manager. If you use an ItsyBitsy RP2040 then install **Adafruit NeoPixel** and **Adafruit TinyUSB**.
3. Copy all the folders under [libraries](libraries/) into your Arduino libraries folder.
4. Copy the [SamcoEnhanced](SamcoEnhanced/) folder to your Arduino sketch folder.
5. Open the [SamcoEnhanced](SamcoEnhanced/) sketch.

## Compiling and Configuration Options
If you are using an ItsyBitsy M0 or M4 then I recommend you set the Optimize option to -O3 (or faster). If you are using an ItsyBitsy RP2040 then I recommend the -O3 Optimize option. If you are using an ItsyBitsy M4 (or any other SAMD51 board) then set the CPU Speed to 120 MHz standard. If you are using an ItsyBitsy RP2040 then set the CPU Speed to 125 or 133 MHz. There is no need for overclocking. The ItsyBitsy RP2040 must use the Adafruit TinyUSB stack. The SAMD devices can use either Arduino or TinyUSB USB stack.

> Note: The RP2040 boards tend to default to the `Pico SDK` USB stack so you will see a build error until you switch to TinyUSB.

## Sketch Configuration
The sketch is configured for a SAMCO 2.0 (GunCon 2) build. If you are using a SAMCO 2.0 PCB or your build matches the SAMCO 2.0 button assignment then the sketch will work as is. If you use the ItsyBitsy RP2040 with the SAMCO 2.0 PCB or a different set of buttons then the sketch will have to be modified.

## Additional information
See the [README](SamcoEnhanced/README.md) file in the sketch folder for details on operation and configuration. Also see the README files in [libraries](libraries/) for more information on library functionality.