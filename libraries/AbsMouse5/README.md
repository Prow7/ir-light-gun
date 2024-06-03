# AbsMouse5
Basic 5 button (no scroll wheel) absolute positioning USB HID mouse. Supports Arduino and TinyUSB stacks.

Based on the AbsMouse Arduino library. Modified to be a 5 button device and support TinyUSB.

## Arduino USB
Include `AbsMouse5.h` and an `AbsMouse5` object will be included in your sketch. The AbsMouse5 uses the Arduino HID() object and adds a mouse using report ID 1, the same as the Arduino Mouse object. Do not use both AbsMouse5 and the Arduino Mouse library in the same sketch.
```c++
#include <AbsMouse5.h>

void setup() {
    AbsMouse5.init(1920, 1080);
}

void loop() {
    // do some stuff
    AbsMouse5.move(500, 200);
    AbsMouse5.press(MOUSE_BTN_LEFT);
    AbsMouse5.release(MOUSE_BTN_LEFT);
    // do some other stuff
}
```
## Using TinyUSB
TinyUSB requires creating an instance of the AbsMouse5 object with the report ID for the mouse. The mouse is not automatically included in the USB HID report descriptor. The AbsMouse5 `TUD_HID_REPORT_DESC_ABSMOUSE5_BASIC()` macro must be used for the mouse report descriptor.

#### TinyUSB Example with Adafruit USB HID
```c++
#include <Adafruit_TinyUSB.h>
#include <AbsMouse5.h>

// mouse report ID
constexpr uint8_t HID_RID_MOUSE = 1;

// HID report descriptor using AbsMouse5 template
uint8_t const hidReportDesc[] = {
    TUD_HID_REPORT_DESC_ABSMOUSE5_BASIC(HID_REPORT_ID(HID_RID_MOUSE))
};

// USB HID instance
Adafruit_USBD_HID usbHid;

// AbsMouse5 instance
AbsMouse5_ AbsMouse5(HID_RID_MOUSE);

void setup() {
    usbHid.setReportDescriptor(hidReportDesc, sizeof(hidReportDesc));
    usbHid.begin();
    AbsMouse5.init(1920, 1080);
}

void loop() {
    // do some stuff
    AbsMouse5.move(500, 200);
    AbsMouse5.press(MOUSE_BTN_LEFT);
    AbsMouse5.release(MOUSE_BTN_LEFT);
    // do some other stuff
}
```
