

#include "variant.h"
#include "wiring_constants.h"
#include "wiring_digital.h"
#include "nrf.h"


const uint32_t g_ADigitalPinMap[] =
{
  NRF_GPIO_PIN_MAP(0, 31), // D0  - BAT_AD (AIN7)
  NRF_GPIO_PIN_MAP(0, 29), // D1  - CHRG_ST
  NRF_GPIO_PIN_MAP(0, 4 ), // D2  - MOT_PIN
  NRF_GPIO_PIN_MAP(0, 6 ), // D3  - LCD_WR
  NRF_GPIO_PIN_MAP(0, 8 ), // D4  - LCD_CS
  NRF_GPIO_PIN_MAP(1, 7 ), // D5  - SDA
  NRF_GPIO_PIN_MAP(1, 9 ), // D6  - LCD_SCLK
  NRF_GPIO_PIN_MAP(1, 6 ), // D7  - SCL
  NRF_GPIO_PIN_MAP(0, 11), // D8  - LCD_RES
  NRF_GPIO_PIN_MAP(0, 12), // D9  - LCD_SI
  NRF_GPIO_PIN_MAP(1, 4 ), // D10 - LCD_BL_PIN
  NRF_GPIO_PIN_MAP(1, 2 ), // D11 - KEY2_PIN
  NRF_GPIO_PIN_MAP(1, 1 ), // D12 - GR_INT
  NRF_GPIO_PIN_MAP(0, 14), // D13 - RX
  NRF_GPIO_PIN_MAP(0, 16), // D14 - F_HOLD
  NRF_GPIO_PIN_MAP(0, 25), // D15 - TX
  NRF_GPIO_PIN_MAP(0, 13), // D16 - TP_RST
  NRF_GPIO_PIN_MAP(0, 15), // D17 - TP_INT
  NRF_GPIO_PIN_MAP(0, 17), // D18 - F_CS
  NRF_GPIO_PIN_MAP(0, 20), // D19 - F_IO
  NRF_GPIO_PIN_MAP(0, 22), // D20 - F_SCLK
  NRF_GPIO_PIN_MAP(0, 24), // D21 - F_SO
  NRF_GPIO_PIN_MAP(1, 0 ), // D22 - F_WP
  NRF_GPIO_PIN_MAP(0, 10)  // D23
};



void initVariant()
{
  pinMode(PIN_VIBRATOR_CTRL, OUTPUT);
  digitalWrite(PIN_VIBRATOR_CTRL, HIGH);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(PIN_TP_RST, OUTPUT);
  //digitalWrite(PIN_TP_RST, LOW);

  pinMode(PIN_A0, INPUT);
  pinMode(PIN_CHARGE_IRQ, INPUT);
  pinMode(PIN_KEY_ACTION, INPUT);
  pinMode(PIN_BMA400_IRQ, INPUT);
  pinMode(PIN_TP_IRQ, INPUT);
}

