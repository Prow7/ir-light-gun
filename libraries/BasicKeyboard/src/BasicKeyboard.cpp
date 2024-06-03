/*
 *  NOTE: This code is derived and stripped down from the standard Arduino
 *    Keyboard.h and Keyboard.cpp code. The copyright on that original code
 *    is as follows.
 *   
 *  Copyright (c) 2015, Arduino LLC
 *  Original code (pre-library): Copyright (c) 2011, Peter Barrett
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifdef USE_TINYUSB
#include <Adafruit_TinyUSB.h>
#else
#include <HID.h>
#endif

#include "BasicKeyboard.h"

#if defined(_USING_HID)
// HID keyboard report descriptor copied from the Arduino Keyabord library,
// found in Keyboard.cpp. It uses report ID 2.
static const uint8_t _hidRawKbReportDescriptor[] PROGMEM = {
    //  Keyboard
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 47
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)

    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)

    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)

    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x73,                    //   LOGICAL_MAXIMUM (115)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)

    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x73,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0,                          // END_COLLECTION
};

// BasicKeyboard instance
BasicKeyboard_ BasicKeyboard(2);
#endif // _USING_HID

BasicKeyboard_::BasicKeyboard_(uint8_t reportId) : _reportId(reportId), _autoReport(true)
{
    memset(&_keyReport, 0, sizeof(_keyReport));
#if defined(_USING_HID)
	static HIDSubDescriptor descriptor(_hidRawKbReportDescriptor, sizeof(_hidRawKbReportDescriptor));
	HID().AppendDescriptor(&descriptor);
#endif // _USING_HID
}
  
void BasicKeyboard_::report()
{
#if defined(_USING_HID)
    // the descriptor above, copied from the Arduino Keyboard library, use a fixed report ID value of 2
	HID().SendReport(2, &_keyReport, sizeof(_keyReport));
#endif // _USING_HID
#if defined(USE_TINYUSB)
    if (TinyUSBDevice.suspended())  {
        TinyUSBDevice.remoteWakeup();
    }
    while (!tud_hid_ready()) yield();
    tud_hid_keyboard_report(_reportId, _keyReport.modifiers, _keyReport.keys);
    yield();
#endif // USE_TINYUSB
}

bool BasicKeyboard_::prepareKeyPress(uint8_t key)
{
    if (key > KEY_RIGHTMETA || !key) {
        // invalid key code
        return false;
    }

    if (key >= KEY_LEFTCTRL) {
        _keyReport.modifiers |= (1 << (key & 7));
        return true;
    } else if (_keyReport.keys[0] != key && _keyReport.keys[1] != key && _keyReport.keys[2] != key &&
               _keyReport.keys[3] != key && _keyReport.keys[4] != key && _keyReport.keys[5] != key) {
        // key is not already pressed, assign it to an available slot
        for (unsigned int i = 0; i < sizeof(_keyReport.keys); ++i) {
            if (_keyReport.keys[i] == 0) {
                _keyReport.keys[i] = key;
                return true;
            }
        }
    }

    // no slot available
    return false;
}

bool BasicKeyboard_::press(uint8_t key)
{
    bool success = prepareKeyPress(key);
    if (success) {
        autoreport();
    }
    return success;
}

bool BasicKeyboard_::press(uint8_t modMask, uint8_t key)
{
    bool success = prepareKeyPress(key);
    if (success) {
        _keyReport.modifiers |= modMask;
        autoreport();
    }
    return success;
}

void BasicKeyboard_::pressModifers(uint8_t modMask)
{
    _keyReport.modifiers |= modMask;
    autoreport();
}

bool BasicKeyboard_::prepareKeyRelease(uint8_t key)
{
    if (key > KEY_RIGHTMETA || !key) {
        // invalid key code
        return false;
    }

    if (key >= KEY_LEFTCTRL) {
        _keyReport.modifiers &= ~(1 << (key & 7));
    } else {
        // check all positions in case the key is present more than once...
        // how could that happen? gallop2WhoKnows
        for (unsigned int i = 0; i < sizeof(_keyReport.keys); ++i) {
            if (_keyReport.keys[i] == key) {
                _keyReport.keys[i] = 0;
            }
        }
    }

    return true;
}

bool BasicKeyboard_::release(uint8_t key) 
{
    bool success = prepareKeyRelease(key);
    if (success) {
        autoreport();
    }
    return success;
}

bool BasicKeyboard_::release(uint8_t modMask, uint8_t key) 
{
    bool success = prepareKeyRelease(key);
    if (success) {
        _keyReport.modifiers &= ~modMask;
        autoreport();
    }
    return success;
}

void BasicKeyboard_::releaseModifers(uint8_t modMask)
{
    _keyReport.modifiers &= ~modMask;
    autoreport();
}

void BasicKeyboard_::releaseAll()
{
    memset(&_keyReport, 0, sizeof(_keyReport));
    autoreport();
}
