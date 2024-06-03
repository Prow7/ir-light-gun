/*!
 * @file AbsMouse5.cpp
 * @brief 5 button absolute mouse.
 * @n Based on AbsMouse Arduino library. Modified to be a 5 button device.
 */

#if defined(USE_TINYUSB)
#include <Adafruit_TinyUSB.h>
#elif defined(CFG_TUSB_MCU)
#error Incompatible USB stack. Use Arduino or Adafruit TinyUSB.
#else
#include <HID.h>
#endif

#include "AbsMouse5.h"

#if defined(_USING_HID)
// The Arduino Mouse library uses a fixed report ID value of 1
static const uint8_t _AbsMouse5HIDReportDescriptor[] PROGMEM = {
	0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
	0x09, 0x02,        // Usage (Mouse)
	0xA1, 0x01,        // Collection (Application)
	0x09, 0x01,        //   Usage (Pointer)
	0xA1, 0x00,        //   Collection (Physical)
	0x85, 0x01,        //     Report ID (1)
	0x05, 0x09,        //     Usage Page (Button)
	0x19, 0x01,        //     Usage Minimum (0x01)
	0x29, 0x05,        //     Usage Maximum (0x05)
	0x15, 0x00,        //     Logical Minimum (0)
	0x25, 0x01,        //     Logical Maximum (1)
	0x95, 0x05,        //     Report Count (5)
	0x75, 0x01,        //     Report Size (1)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x95, 0x01,        //     Report Count (1) (3 bit padding)
	0x75, 0x03,        //     Report Size (3)
	0x81, 0x03,        //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
	0x09, 0x30,        //     Usage (X)
	0x09, 0x31,        //     Usage (Y)
	0x16, 0x00, 0x00,  //     Logical Minimum (0)
	0x26, 0xFF, 0x7F,  //     Logical Maximum (32767)
	0x36, 0x00, 0x00,  //     Physical Minimum (0)
	0x46, 0xFF, 0x7F,  //     Physical Maximum (32767)
	0x75, 0x10,        //     Report Size (16)
	0x95, 0x02,        //     Report Count (2)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0xC0,              //   End Collection
	0xC0               // End Collection
};

// AbsMouse5 instance
AbsMouse5_ AbsMouse5(1);
#endif // _USING_HID

AbsMouse5_::AbsMouse5_(uint8_t reportId) : _reportId(reportId), _buttons(0), _x(0), _y(0), _width(32767l), _height(32767l), _autoReport(true)
{
#if defined(_USING_HID)
	static HIDSubDescriptor descriptor(_AbsMouse5HIDReportDescriptor, sizeof(_AbsMouse5HIDReportDescriptor));
	HID().AppendDescriptor(&descriptor);
#endif // _USING_HID
}

void AbsMouse5_::init(uint16_t width, uint16_t height, bool autoReport)
{
	_width = width;
	_height = height;
	_autoReport = autoReport;
}

void AbsMouse5_::report(void)
{
	uint8_t buffer[5];
	buffer[0] = _buttons;
	buffer[1] = (uint8_t)_x;
	buffer[2] = (uint8_t)(_x >> 8);
	buffer[3] = (uint8_t)_y;
	buffer[4] = (uint8_t)(_y >> 8);
#if defined(_USING_HID)
	// the descriptor above and Arduino Mouse library use a fixed report ID of 1
	HID().SendReport(1, buffer, 5);
#endif // _USING_HID
#if defined(USE_TINYUSB)
	if (TinyUSBDevice.suspended())  {
		TinyUSBDevice.remoteWakeup();
	}
	while (!tud_hid_ready()) yield();
	tud_hid_report(_reportId, buffer, 5);
	yield();
#endif // USE_TINYUSB
}

void AbsMouse5_::move(uint16_t x, uint16_t y)
{
	x = (uint16_t)((32767ul * ((uint32_t)x)) / _width);
	y = (uint16_t)((32767ul * ((uint32_t)y)) / _height);

	if(x != _x || y != _y) {
		_x = x;
		_y = y;
		autoreport();
	}
}

void AbsMouse5_::press(uint8_t button) 
{
	const uint8_t new_buttons = _buttons | button;
	if(new_buttons != _buttons) {
		_buttons = new_buttons;
		autoreport();
	}
}

void AbsMouse5_::release(uint8_t button)
{
	const uint8_t new_buttons = _buttons & ~button;
	if(new_buttons != _buttons) {
		_buttons = new_buttons;
		autoreport();
	}
}
