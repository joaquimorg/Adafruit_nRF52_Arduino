#include "variant.h"

#include "wiring_constants.h"
#include "wiring_digital.h"
#include "nrf.h"

const uint32_t g_ADigitalPinMap[] = {
  
  0,  // 32.768KHz –XL1
  1,  // 32.768KHz –XL1
  2,  // SPI-SCK, LCD_SCK OUT
  3,  // SPI-MOSI, LCD_SDI OUT
  4,  // SPI-MISO IN
  5,  // SPI-CE# (SPI-NOR) OUT
  6,  // BMA421-SDA, HRS3300-SDA, TP-SDA I/O
  7,  // BMA421-SCL, HRS3300-SCL, TP-SCL OUT
  8,  // BMA421-INT IN
  9,  // LCD_DET OUT
  10, // TP_RESET OUT
  11, // TXD Debug ?
  12, // CHARGE INDICATION IN
  13, // PUSH BUTTON_IN IN
  14, // LCD_BACKLIGHT_LOW OUT
  15, // PUSH BUTTON_OUT OUT
  16, // VIBRATOR OUT OUT
  17, // RXD Debug ?
  18, // LCD_RS OUT
  19, // POWER PRESENCE INDICATION IN
  20, // 
  21, // 
  22, // LCD_BACKLIGHT_MID OUT
  23, // LCD_BACKLIGHT_HIGH OUT
  24, // 3V3 POWER CONTROL OUT
  25, // LCD_CS OUT
  26, // LCD_RESET OUT
  27, // STATUS LED (NOT STAFF) OUT
  28, // TP_INT IN
  29, // 
  30, // HRS3300-TEST IN
  31, // BATTERY VOLTAGE (Analog) IN
};

void initVariant()
{
  // LED1 & LED2
  //pinMode(PIN_LED1, OUTPUT);
  //ledOff(PIN_LED1);

  //pinMode(PIN_LED2, OUTPUT);
  //ledOff(PIN_LED2);
}

