/*!
 * @file AbsMouse5.h
 * @brief Basic 5 button absolute mouse (no scroll wheel). Supports Arduino and TinyUSB stacks.
 * @n Based on the AbsMouse Arduino library. Modified to be a 5 button device.
 * @n For TinyUSB, include a TUD_HID_REPORT_DESC_ABSMOUSE5_BASIC(RID) in the descriptor report.
 */

#ifndef _ABSMOUSE5_H_
#define _ABSMOUSE5_H_

#include <stdint.h>

// Mouse button bit mask defines.
// TinyUSB can also use the MOUSE_BUTTON_ values from hid_mouse_button_bm_t enum in hid.h.
#define MOUSE_BTN_LEFT 0x01
#define MOUSE_BTN_RIGHT 0x02
#define MOUSE_BTN_MIDDLE 0x04
#define MOUSE_BTN_4 0x08
#define MOUSE_BTN_BACKWARD 0x08
#define MOUSE_BTN_5 0x10
#define MOUSE_BTN_FORWARD 0x10

// TinyUSB report descriptor macro for a basic 5 button absolute mouse (no scroll wheel)
#if defined(USE_TINYUSB)
#define TUD_HID_REPORT_DESC_ABSMOUSE5_BASIC(...) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP      )                   ,\
  HID_USAGE      ( HID_USAGE_DESKTOP_MOUSE     )                   ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION  )                   ,\
    /* Report ID if any */\
    __VA_ARGS__ \
    HID_USAGE      ( HID_USAGE_DESKTOP_POINTER )                   ,\
    HID_COLLECTION ( HID_COLLECTION_PHYSICAL   )                   ,\
      HID_USAGE_PAGE  ( HID_USAGE_PAGE_BUTTON  )                   ,\
        HID_USAGE_MIN   ( 1                                      ) ,\
        HID_USAGE_MAX   ( 5                                      ) ,\
        HID_LOGICAL_MIN ( 0                                      ) ,\
        HID_LOGICAL_MAX ( 1                                      ) ,\
        /* Left, Right, Middle, Backward, Forward buttons */ \
        HID_REPORT_COUNT( 5                                      ) ,\
        HID_REPORT_SIZE ( 1                                      ) ,\
        HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
        /* 3 bit padding */ \
        HID_REPORT_COUNT( 1                                      ) ,\
        HID_REPORT_SIZE ( 3                                      ) ,\
        HID_INPUT       ( HID_CONSTANT                           ) ,\
      HID_USAGE_PAGE  ( HID_USAGE_PAGE_DESKTOP )                   ,\
        /* X, Y absolute position [0, 32767] */ \
        HID_USAGE       ( HID_USAGE_DESKTOP_X                    ) ,\
        HID_USAGE       ( HID_USAGE_DESKTOP_Y                    ) ,\
        HID_LOGICAL_MIN   ( 0x00                                ) ,\
        HID_LOGICAL_MAX_N ( 0x7FFF, 2                           ) ,\
        HID_PHYSICAL_MIN  ( 0x00                                ) ,\
        HID_PHYSICAL_MAX_N( 0x7FFF, 2                           ) ,\
        HID_REPORT_SIZE  ( 16                                  ) ,\
        HID_REPORT_COUNT ( 2                                   ) ,\
        HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
    HID_COLLECTION_END                                            , \
  HID_COLLECTION_END
#endif // USE_TINYUSB

// 5 button absolute mouse
class AbsMouse5_
{
private:
	const uint8_t _reportId;
	uint8_t _buttons;
	uint16_t _x;
	uint16_t _y;
	uint32_t _width;
	uint32_t _height;
	bool _autoReport;

	/// @brief Call report() if auto report is enabled.
	inline void autoreport() {
		if(_autoReport) {
			report();
		}
	}

public:
	/// @brief Constructor.
	/// @param[in] reportId TinyUSB report ID. Ignored when using Arduino HID.
	AbsMouse5_(uint8_t reportId);

	/// @brief Initialize the absolute mouse.
	/// @param widthMax Maximum width value.
	/// @param heightMax Maximum height value. 
	/// @param autoReport True to call report() any time the mouse state changes.
	/// False to require explicit call to report().
	void init(uint16_t widthMax = 32767, uint16_t heightMax = 32767, bool autoReport = true);
	
	/// @brief Send a USB report.
	void report();

	/// @brief Move the mouse X and Y positions.
	/// @param x X position
	/// @param y Y position
	void move(uint16_t x, uint16_t y);

	/// @brief Move the mouse X position.
	void movex(uint16_t x) { move(x, _y); }
	
	/// @brief Move the mouse Y position.
	void movey(uint16_t y) { move(_x, y); }
	
	/// @brief Press mouse button(s).
	/// @param b Button mask.
	void press(uint8_t b = MOUSE_BTN_LEFT);

	/// @brief Release mouse button(s).
	/// @param b Button mask.
	void release(uint8_t b = MOUSE_BTN_LEFT);

	/// @brief Release all mouse buttons.
	void releaseAll() { release(0x1f); }
};

// global singleton
extern AbsMouse5_ AbsMouse5;

#endif // _ABSMOUSE5_H_
