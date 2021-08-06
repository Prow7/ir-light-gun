/*!
 * @file SamcoBoard.h
 * @brief Build configuration for Arduino boards.
 *
 * @copyright Mike Lynch, 2021
 * @copyright GNU Lesser General Public License
 *
 * @author Mike Lynch
 * @version V1.0
 * @date 2021
 */

#ifndef _SAMCOBOARD_H_
#define _SAMCOBOARD_H_

// sadly even Arduino doesn't have ideal defines... very odd there is a generic __SAMD51__, but not __SAMD21__?
// I'll add that anyway along with the known ItsyBitsy defines that are the supported boards
#if defined(ADAFRUIT_ITSYBITSY_M0) || defined(ARDUINO_ITSYBITSY_M0) || defined(__SAMD21__) || defined(__SAMD21G18A__)
    // use SAMD21 peripherals
    #define SAMCO_SAMD21 1

    // DFRobot IR camera IIC clock
    // even with cheap clips and the full length IR cam cable 1MHz is fine
    #define DFROBOT_IR_IIC_CLOCK 1000000

    // timer configuration
    #define TIMER_PRESCALER_DIV 16
    #define TIMER_TC_CTRLA_PRESCALER_DIV TC_CTRLA_PRESCALER_DIV16
#elif defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS) || defined(ARDUINO_ITSYBITSY_M4) || defined(__SAMD51__) || defined(__SAMD51G19A__)
    // use SAMD51 peripherals
    #define SAMCO_SAMD51 1

    // DFRobot IR camera IIC clock
    // even with cheap clips and the full length IR cam cable 1MHz is fine
    #define DFROBOT_IR_IIC_CLOCK 1000000

    // timer configuration
    #define TIMER_PRESCALER_DIV 16
    #define TIMER_TC_CTRLA_PRESCALER_DIV TC_CTRLA_PRESCALER_DIV16
#elif defined(ARDUINO_AVR_ITSYBITSY32U4_5V) || defined(ARDUINO_AVR_MICRO) || defined(ARDUINO_AVR_PROMICRO) || defined(ARDUINO_AVR_LEONARDO)
    // ATmega32u4
    #define SAMCO_ATMEGA32U4 1

    // AVR has EEPROM
    #define SAMCO_EEPROM_ENABLE 1

    // DFRobot IR camera IIC clock
    // The ATmega32u4 datasheet says a maximum of 400kHz
    #define DFROBOT_IR_IIC_CLOCK 400000
#else
    // unknown board
    // this will use millis() for camera update timing instead of a hardware timer
    #define SAMCO_NO_HW_TIMER 1
    #define DFROBOT_IR_IIC_CLOCK 400000
#endif // determine SAMCO_xxx board

// specific ItsyBitsy board configuration
#if defined(ADAFRUIT_ITSYBITSY_M0) || defined(ARDUINO_ITSYBITSY_M0)
    // included Dot Star on ItsyBitsy M0
    #define DOTSTAR_DATAPIN 41
    #define DOTSTAR_CLOCKPIN 40
    #define DOTSTAR_ENABLE 1

    // use flash
    #define SAMCO_FLASH_ENABLE 1
#elif defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS) || defined(ARDUINO_ITSYBITSY_M4)
    // included Dot Star on ItsyBitsy M4
    #define DOTSTAR_DATAPIN 8
    #define DOTSTAR_CLOCKPIN 6
    #define DOTSTAR_ENABLE 1

    // use flash
    #define SAMCO_FLASH_ENABLE 1
#endif // ADAFRUIT_ITSYBITSY

// auto-detect flash presence if it is not yet enabled and no EEPROM
#if !defined(SAMCO_FLASH_ENABLE) && !defined(SAMCO_EEPROM_ENABLE)
#if defined(EXTERNAL_FLASH_USE_QSPI) || defined(EXTERNAL_FLASH_USE_SPI)
    #define SAMCO_FLASH_ENABLE 1
#endif // EXTERNAL_FLASH_USE
#endif // SAMCO_FLASH_ENABLE

#endif // _SAMCOBOARD_H_
