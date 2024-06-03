# BasicKeyboard library

Basic USB HID keyboard library. This implements a basic USB HID keyboard with the keys defined in the "Keyboard/Keypad Page (0x07)" section in the [HID Usage Tables for USB](https://usb.org/document-library/hid-usage-tables-15) document.

The BasicKeyboard uses the standard 6 key with 8 modifier flags HID keyboard report.

The defined USB HID key codes and modifier flags must be used. There is no ASCII translation provided like what is found in the Arduino keyboard library.

The library supports the Arduino and TinyUSB USB stacks.

The rollover error state is not reported. Up to 6 keys can be pressed simultaneously, not including the modifier keys. The 8 modifier keys (CTRL, SHIFT, etc.) are reported as a separate bitmask byte.

# Usage

Copy the BasicKeyboard folder to your "Arduino/libraries" folder.

    #include <BasicKeyboard.h>
	 
See `BasicKeyboard.h` for details. The key codes and modifier key flags are defined in `usb_hid_keys.h`.

## Using Arduino USB
When using the Arduino USB stack, include the `BasicKeyboard.h` and a `BasicKeyboard` object will be included in your sketch. The BasicKeyboard uses the Arduino HID() object and adds a keyboard using report ID 2, the same as the Arduino Keyboard object. Do not use both BasicKeyboard and the Arduino Keyboard library in the same sketch.

## Using TinyUSB
TinyUSB requires creating an instance of the BasicKeyboard object with the report ID for the keyboard. The keyboard is not automatically included in the USB HID report descriptor. The TinyUSB `TUD_HID_REPORT_DESC_KEYBOARD()` macro can be used for the keyboard report descriptor.

#### Example using the Adafruit USB HID
```cpp
#include <Adafruit_TinyUSB.h> 
#include <BasicKeyboard.h>

// keyboard report ID
constexpr uint8_t HID_RID_KEYBOARD = 1;

// HID report descriptor using TinyUSB's template
uint8_t const hidReportDesc[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_RID_KEYBOARD))
};

// USB HID instance
Adafruit_USBD_HID usbHid;

// BasicKeyboard instance
BasicKeyboard_ BasicKeyboard(HID_RID_KEYBOARD);

void setup() {
    usbHid.setReportDescriptor(hidReportDesc, sizeof(hidReportDesc));
    usbHid.begin();
}
```
