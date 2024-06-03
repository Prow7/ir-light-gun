/**
 * Basic USB HID keyboard library. This implements a basic USB HID keyboard with the keys
 * defined in the "Keyboard/Keypad Page (0x07)" section in the HID Usage Tables for USB document.
 * https://usb.org/document-library/hid-usage-tables-15
 * 
 * See usb_hid_keys.h for the key code and modifier flag defines.
 * 
 * When using the Arduino USB stack, a BasicKeyboard object will be included and the keybaord
 * added to HID() using report ID 2. This is the same report ID the Arduino Keyboard uses.
 *  
 * When using TinyUSB, include a TUD_HID_REPORT_DESC_KEYBOARD(RID_KEYBOARD)
 * in the descriptor report.
 * 
 * In your main sketch declare the keyboard instance with the reort ID:
 *  BasicKeyboard_ Keyboard(RID_KEYBOARD);
 * 
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

#ifndef _BASICKEYBOARD_H_
#define _BASICKEYBOARD_H_

#include <stdint.h>
#include "usb_hid_keys.h"

// USB HID key report structure: up to 6 keys and modifiers
typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport_t;

class BasicKeyboard_
{
private:
    const uint8_t _reportId;
    KeyReport_t _keyReport;
    bool _autoReport;
    
    /// @brief Call report() if auto report is enabled.
    inline void autoreport() {
        if(_autoReport) {
            report();
        }
    }

    /// @brief Update the report data with a key press.
    /// @param keyCode Key code.
    /// @return True for success.
    /// False if the key code is invalid or 6 keys are already pressed.
    bool prepareKeyPress(uint8_t keyCode);

    /// @brief Release a key from the report data.
    /// @param keyCode Key code.
    /// @return True if the key code is valid.
    bool prepareKeyRelease(uint8_t keyCode);

public:
    /// @brief Constructor.
    /// @param[in] reportId TinyUSB report ID. Ignored when using Arduino HID.
    BasicKeyboard_(uint8_t reportId);

    /// @brief Set automatic report when keys are pressed or released.
    /// @param autoReport True to call report() any time a key is pressed or released.
    /// False to require explicit call to report().
    void setAutoReport(bool autoReport) { _autoReport = autoReport; }

    /// @brief Send a USB report.
    void report();

    /// @brief Press a key.
    /// @param keyCode Key code.
    /// @return True if the key is pressed.
    /// False if the key code is invalid or 6 keys are already pressed.
    bool press(uint8_t keyCode);

    /// @brief Press modifers and a key.
    /// @details If the key code is invalid then the modifier keys will not change.
    /// @param modMask Modifier bitmask of keys to press. This value will be bitwise OR'd
    /// to the current modifier mask. Bits set to 1 will press.
    /// @param keyCode Key code.
    /// @return True if the key is pressed.
    /// False if the key code is invalid or 6 keys are already pressed.
    bool press(uint8_t modMask, uint8_t keyCode);

    /// @brief Press modifier keys.
    /// @param modMask Modifier bitmask of keys to press. This value will be bitwise OR'd
    /// to the current modifier mask. Bits set to 1 will press.
    void pressModifers(uint8_t modMask);

    /// @brief Release a key.
    /// @param keyCode Key code.
    /// @return True if the key code is valid.
    bool release(uint8_t keyCode);

    /// @brief Release modifers and a key.
    /// @details If the key code is invalid then the modifier keys will not change.
    /// @param modMask Modifier bitmask of keys to release. Bits set to 1 will release.
    /// 0 will keep the existing value.
    /// @param keyCode Key code.
    /// @return True if the key code is valid.
    bool release(uint8_t modMask, uint8_t keyCode);

    /// @brief Release modifier keys.
    /// @param modMask Modifier bitmask of keys to release. Bits set to 1 will release.
    /// 0 will keep the existing value.
    void releaseModifers(uint8_t modMask);

    /// @brief Release all keys.
    void releaseAll();

    /// @brief Check if a key code is valid.
    /// @param keyCode Key code.
    /// @return True if the key code is valid.
    static bool isKeyCodeValid(uint8_t keyCode) { return (keyCode != 0) && (keyCode <= KEY_RIGHTMETA); }
};

extern BasicKeyboard_ BasicKeyboard;

#endif // _BASICKEYBOARD_H_
