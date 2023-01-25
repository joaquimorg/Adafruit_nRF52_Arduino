

#ifndef _VARIANT_PINETIME40_
#define _VARIANT_PINETIME40_

/** Master clock frequency */
#define VARIANT_MCK       (64000000ul)

#define USE_LFXO      // Board uses 32khz crystal for LF
// define USE_LFRC    // Board uses RC for LF

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "WVariant.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// Number of pins defined in PinDescription array
#define PINS_COUNT           (24)
#define NUM_DIGITAL_PINS     (24)
#define NUM_ANALOG_INPUTS    (1)
#define NUM_ANALOG_OUTPUTS   (0)

// LEDs
#define PIN_LED1             (10)

#define LED_BUILTIN          PIN_LED1
#define LED_CONN             PIN_LED1

#define LED_RED              PIN_LED1
#define LED_BLUE             PIN_LED1

#define LED_STATE_ON         0         // State when LED is litted

/*
 * Buttons
 */
#define PIN_BUTTON1          (11)

/*
 * Analog pins
 */
#define PIN_A0               (0)
#define PIN_A1               (0xff)
#define PIN_A2               (0xff)
#define PIN_A3               (0xff)
#define PIN_A4               (0xff)
#define PIN_A5               (0xff)
#define PIN_A6               (0xff)
#define PIN_A7               (0xff)

static const uint8_t A0  = PIN_A0 ;
static const uint8_t A1  = PIN_A1 ;
static const uint8_t A2  = PIN_A2 ;
static const uint8_t A3  = PIN_A3 ;
static const uint8_t A4  = PIN_A4 ;
static const uint8_t A5  = PIN_A5 ;
static const uint8_t A6  = PIN_A6 ;
static const uint8_t A7  = PIN_A7 ;
#define ADC_RESOLUTION    14

// Other pins
#define PIN_AREF           (0xff)
#define PIN_NFC1           (0xff)
#define PIN_NFC2           (0xff)

static const uint8_t AREF = PIN_AREF;

/*
 * Serial interfaces
 */

#define PIN_SERIAL_RX      (13)
#define PIN_SERIAL_TX      (15)

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 1

#define PIN_SPI_MISO         (9)
#define PIN_SPI_MOSI         (0xff)
#define PIN_SPI_SCK          (6)

static const uint8_t SS   = 4 ;
static const uint8_t MOSI = PIN_SPI_MOSI ;
static const uint8_t MISO = PIN_SPI_MISO ;
static const uint8_t SCK  = PIN_SPI_SCK ;

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_SDA         (5)
#define PIN_WIRE_SCL         (7)

// QSPI Pins
#define PIN_QSPI_SCK         20
#define PIN_QSPI_CS          18
#define PIN_QSPI_IO0         19
#define PIN_QSPI_IO1         21
#define PIN_QSPI_IO2         22
#define PIN_QSPI_IO3         14

// On-board QSPI Flash
#define EXTERNAL_FLASH_DEVICES   XT25F32BDFIGT // XT25F32BDFIGT-S
#define EXTERNAL_FLASH_USE_QSPI

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#endif
