#include <Arduino.h>
#include "i2c.h"

#include "Wire.h"

uint8_t _uc_pinSDA = g_ADigitalPinMap[PIN_WIRE_SDA];
uint8_t _uc_pinSCL = g_ADigitalPinMap[PIN_WIRE_SCL];

/*Set to 1 to print debug i2c data (read and write communication)*/
#define I2C_DEBUG 0

void init_i2c() {

    Serial.println("> I2C init");
    /*Serial.println(">> Unblock I2C");
    // Unblock I2C?
    nrf_gpio_cfg(_uc_pinSCL,
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        NRF_GPIO_PIN_NOPULL,
        NRF_GPIO_PIN_S0D1,
        NRF_GPIO_PIN_NOSENSE);
    nrf_gpio_pin_set(_uc_pinSCL);
    for (uint8_t i = 0; i < 16; i++) {
        nrf_gpio_pin_toggle(_uc_pinSCL);
        delay_ns(5);
    }
    nrf_gpio_cfg_default(_uc_pinSCL);*/

    Wire.begin();
}

uint8_t user_i2c_read(uint8_t addr, uint8_t reg_addr, uint8_t* reg_data, uint32_t length) {
    Wire.beginTransmission(addr);
    Wire.write(reg_addr);
    Wire.endTransmission(true);
    Wire.requestFrom(addr, length);
    /*for (uint32_t i = 0; i < length; i++) {
        if (Wire.available()) {
            *reg_data++ = Wire.read();
        } else {
            return 1;
        }

    }*/
    while (Wire.available()) {
        for (uint16_t i = 0; i < length; i++) {
            reg_data[i] = Wire.read();
#if I2C_DEBUG
            Serial.print("<= 0x");
            Serial.print(reg_addr + i, HEX);
            Serial.print("\t0x");
            Serial.print(reg_data[i], HEX);
            Serial.print("\n");
#endif
        }
    }
    return 0;
}

uint8_t user_i2c_write(uint8_t addr, uint8_t reg_addr, const uint8_t* reg_data, uint32_t length) {

    Wire.beginTransmission(addr);
    Wire.write(reg_addr);
    for (uint32_t i = 0; i < length; i++) {
        //Wire.write(*reg_data++);
        Wire.write(reg_data[i]);
#if I2C_DEBUG
        Serial.print("=> 0x");
        Serial.print(reg_addr + i, HEX);
        Serial.print("\t0x");
        Serial.print(reg_data[i], HEX);
        Serial.print("\n");
#endif
    }
    Wire.endTransmission(true);
    return 0;
}
