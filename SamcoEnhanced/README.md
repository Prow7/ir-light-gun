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

Note that SAMCO PCBs only offically support the ItsyBitsy M0 or M4. The ItsyBitsy RP2040 should be pin compatible but hasn't been confirmed.

The sketch is configured with buttons for a SAMCO GunCon 2 PCB.

The ATmega32U4 is no longer officially supported.

## IR Emitter setup
The IR emitters must be arranged with 2 emitters on opposite edges of your screen/monitor forming a rectangle or square. For example, you can use 2 Wii sensor bars; one on top of your screen and one below.

## Arduino Board Dependencies
Be sure to install the appropriate boards in the Adruino Boards Manager.
#### RP2040
- [Earle F. Philhower Arduino-Pico](https://github.com/earlephilhower/arduino-pico)
#### SAMD
- Arduino SAMD Boards
- Adafruit SAMD Boards (ItsyBitsy M0, M4, or other Adafruit SAMD board)

## Arduino Library Dependencies
Be sure to have the following libraries installed depending on the board you are using (or just install them all).
- Adafruit DotStar (for ItsyBitsy M0 and M4)
- Adafruit NeoPixel (for ItsyBitsy RP2040)
- Adafruit SPI Flash (for ItsyBitsy M0 and M4)
- Adafruit TinyUSB (included for SAMD and RP2040 boards. Required for ItsyBitsy RP2040, can also be used for SAMD)

## Included libraries
Copy all folders under [libraries](../libraries/) to your Arduino `libraries` folder.
- AbsMouse5 - 5 button absolute positioning USB HID mouse device
- BasicKeyboard - Basic USB HID keyboard device
- DFRobotIRPositionEx - Modified DFRobot IR Positioning Camera library
- LightgunButtons - Library to handle the physical buttons
- SamcoPositionEnhanced - Slightly modified SAMCO positioning maths

## Compiling and Configuration Options
If you are using an ItsyBitsy M0 or M4 then I recommend you set the Optimize option to -O3 (or faster). If you are using an ItsyBitsy RP2040 then I recommend the -O3 Optimize option. If you are using an ItsyBitsy M4 (or any other SAMD51 board) then set the CPU Speed to 120 MHz standard. If you are using an ItsyBitsy RP2040 then set the CPU Speed to 125 or 133 MHz. There is no need for overclocking. The ItsyBitsy RP2040 must use the Adafruit TinyUSB stack. The SAMD devices can use either Arduino or TinyUSB USB stack.

> Note: The RP2040 boards tend to default to the `Pico SDK` USB stack so you will see a build error until you switch to TinyUSB.

## Operation
The light gun operates as a mouse until the button/combination is pressed to enter pause mode. The Arduino serial monitor (or any serial terminal) can be used to see information while the gun is paused and during the calibration procedure.

Note that the buttons in pause mode (and to enter pause mode) activate when the last button of the combination releases. This is used to detect and differentiate button combinations vs a single button press.

The mouse position updates at 209Hz so it is extremely responsive.

## Run modes
The gun has the following modes of operation:
1. Normal - The mouse position updates from each frame from the IR positioning camera (no averaging)
2. Averaging - The position is calculated from a 2 frame moving average (current + previous position)
3. Averaging2 - The position is calculated from a weighted average of the current frame and 2 previous frames
4. Processing - Test mode for use with the Processing sketch (this mode is prevented from being assigned to a profile)

The averaging modes are subtle but do reduce the motion jitter a bit without adding much if any noticeable lag.

## Processing mode
The Processing mode is intended for use with the SAMCO Processing sketch. Download Processing from processing.org and find the 4IR Processing sketch from the SAMCO project. The Processing sketch lets you visually see the IR points as seen by the camera. This is very useful aligning the camera when building your light gun and for testing that the camera tracks all 4 points properly. I suppose if you don't want to install Processing then you can just open your favourite serial terminal program and watch the numbers scroll by.

## IR camera sensitivity
The IR camera sensitivity can be adjusted. It is recommended to adjust the sensitivity as high as possible. If the IR sensitivity is too low then the pointer precision can suffer. However, too high of a sensitivity can cause the camera to pick up unwanted reflections that will cause the pointer to jump around. It is impossible to know which setting will work best since it is dependent on the specific setup. It depends on how bright the IR emitters are, the distance, camera lens, and if shiny surfaces may cause reflections.

A sign that the IR sensitivity is too low is if the pointer moves in noticeable coarse steps, as if it has a low resolution to it. If you have the sensitivity level set to max and you notice this then the IR emitters may not be bright enough.

A sign that the IR sensitivity is too high is if the pointer jumps around erratically. If this happens only while aiming at certain areas of the screen then this is a good indication a reflection is being detected by the camera. If the sensitivity is at max, step it down to high or minimum. Obviously the best solution is to eliminate the reflective surface. The Processing sketch can help daignose this problem since it will visually display the 4 IR points.

## Profiles
The sketch is configured with 8 profiles available. Each profile has its own calibration data, run mode, and IR camera sensitivity settings. Each profile can be selected from pause mode by assigning a unique button or combination.

## Default Buttons
- Trigger: Left mouse button
- A: Right mouse button
- B: Middle mouse button
- Start: 1 key
- Select: 5 key
- Up/Down/Left/Right: Keyboard arrow keys
- Reload: Enter pause mode
- Pedal: Mouse button 5

## Default Buttons in Pause mode
- A, B, Start, Select, Up, Down, Left, Right: select a profile
- Start + Down: Normal gun mode (averaging disabled)
- Start + Up: Normal gun with averaging, switch between the 2 averaging modes (use serial monitor to see the setting)
- Start + A: Processing mode for use with the Processing sketch
- B + Down: Decrease IR camera sensitivity (use serial monitor to see the setting)
- B + Up: Increase IR camera sensitivity (use serial monitor to see the setting)
- Reload: Exit pause mode
- Trigger: Begin calibration
- Start + Select: save settings to non-volatile memory (EEPROM or Flash depending on the board configuration)

## How to calibrate
1. Press **Reload** to enter pause mode.
2. Press a button to select a profile unless you want to calibration the current profile.
3. Pull the **Trigger** to begin calibration.
4. Shoot the pointer at center of the screen and hold the trigger down for 1/3 of a second while keeping a steady aim.
5. The mouse should lock to the vertical axis. Use the **A**/**B** buttons (can be held down) to adjust the mouse vertical range. **A** will increase and **B** will decrease. Track the pointer at the top and bottom edges of the screen while adjusting.
6. Pull the **Trigger** for horizontal calibration.
7. The mouse should lock to the horizontal axis. Use the **A**/**B** buttons (can be held down) to adjust the mouse horizontal range. **A** will increase and **B** will decrease. Track the pointer at the left and right edges of the screen while adjusting.
8. Pull the **Trigger** to finish and return to run mode. Values will apply to the currently selected profile in memory.
9. Recommended: After confirming the calibration is good, enter pause mode and press Start and Select to save the calibration to non-volatile memory.
10. Optional: Open serial monitor and update the `xCenter`, `yCenter`, `xScale`, and `yScale` values in the profile data array in the sketch (no need with step 9).
 
Calibration can be cancelled during any step by pressing **Reload** or **Start** or **Select**. The gun will return to pause mode if you cancel the calibration.

### Advanced calibration
- During center calibration, press **A** to skip this step and proceed to the vertical calibration
- During vertical calibration, tap **Up** or **Down** to manually fine tune the vertical offset
- During horizontal calibration, tap **Left** or **Right** to manually fine tune the horizontal offset

## Saving settings to non-volatile memory
The calibration data and profile settings can be saved in non-volatile memory. The currently selected profile is saved as the default for when the light gun is plugged in.

For ItsyBitsy M0 and M4 boards the external on-board SPI flash memory is used. For ATmega32U4 the EEPROM is used.

## Sketch Configuration
The sketch is configured for a SAMCO 2.0 (GunCon 2) build. If you are using a SAMCO 2.0 PCB or your build matches the SAMCO 2.0 button assignment then the sketch will work as is. If you are using a different set of buttons then the sketch will have to be modified.

### Define Buttons
Find the `LightgunButtons::ButtonDesc` array and define all of the buttons. The order of the buttons in the array represent a bit position. Define enum constants in the `ButtonMask_e` enum with the button bit mask values. There is also a `ButtonIndex_e` that defines the index positions in the array, but it is not currently not used except to define the bit mask values. See the `Desc_t` structure in the `LightgunButtons` for details on the structure.

At the time of this writing, the button pin values will have to be modified if you are using an ItsyBitsy RP2040 with a SAMCO 2.0 PCB. The sketch has the Arduino pin numbers but the RP2040 must use the physical GPIO pins. It is a bit odd but the library does not provide mapping from the Arduino pin label printed on the top of the PCB to the physical GPIO number printed on the bottom of the PCB (for example, it defines D7 as 7). You will have to modify the pin values with the physical GPIO numbers as seen on the bottom of the PCB (or refer to the pinouts from the Adafruit documentation). For example, the trigger is defined with pin 7 but this is GPIO 6 for the RP2040. The A0 through A3 pins are correctly defined from the library.

### Behaviour buttons
Below the button definitions are a bunch of constants that configure the buttons to control the gun. For example, enter and exit pause mode, and changing various settings. See the comments above each value for details.

Most button behaviours can be assigned a combination. If the comment says a button combination is used then it activates when the last button of the combination releases.

### Other constants
- `IRSeen0Color` colour for the RGB LED when no IR points are seen
- `CalModeColor` colour for the RGB LED while calibrating

### Profiles
There is a `ProfileCount` constant that defines the number of profiles. The `profileData` array has the default values for the profile. If there are no settings saved in non-volatile memory, then these values are used. There is no need to change these values if settings are saved to non-volatile memory.

Find the `profileDesc` array to configure each profile. The profile descriptor allows you to specify:
- A button/combination to select the profile while in pause mode
- Colour used to light up an RGB LED (ItsyBitsy M0 and M4 include a DotStar) when paused
- A text description for the button/combination
- An optional text label or brief description
