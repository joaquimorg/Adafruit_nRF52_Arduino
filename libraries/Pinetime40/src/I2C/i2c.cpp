#include <Arduino.h>
#include "i2c.h"

//#include "Wire.h"

#include "nrfx_twi.h"

uint8_t _uc_pinSDA = g_ADigitalPinMap[PIN_WIRE_SDA];
uint8_t _uc_pinSCL = g_ADigitalPinMap[PIN_WIRE_SCL];

// -------------------------------------------------------------------

#define SCL_PIN_INIT_CONF                                     \
    ( (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) \
    | (GPIO_PIN_CNF_DRIVE_S0D1     << GPIO_PIN_CNF_DRIVE_Pos) \
    | (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos)  \
    | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos) \
    | (GPIO_PIN_CNF_DIR_Input      << GPIO_PIN_CNF_DIR_Pos))

#define SDA_PIN_INIT_CONF        SCL_PIN_INIT_CONF

#define SDA_PIN_UNINIT_CONF                                     \
    ( (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) \
    | (GPIO_PIN_CNF_DRIVE_H0H1       << GPIO_PIN_CNF_DRIVE_Pos) \
    | (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  \
    | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) \
    | (GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos))

#define SCL_PIN_UNINIT_CONF      SDA_PIN_UNINIT_CONF

#define SCL_PIN_INIT_CONF_CLR                                 \
    ( (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) \
    | (GPIO_PIN_CNF_DRIVE_S0D1     << GPIO_PIN_CNF_DRIVE_Pos) \
    | (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos)  \
    | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos) \
    | (GPIO_PIN_CNF_DIR_Output     << GPIO_PIN_CNF_DIR_Pos))

#define SDA_PIN_INIT_CONF_CLR    SCL_PIN_INIT_CONF_CLR


static constexpr uint32_t HwFreezedDelay{ 161000 };


static void twi_clear_bus(uint8_t uc_pinSDA, uint8_t uc_pinSCL)
{

    uint32_t twi_state = NRF_TWIM0->ENABLE;
    NRF_TWIM0->ENABLE = (TWIM_ENABLE_ENABLE_Disabled << TWIM_ENABLE_ENABLE_Pos);
    

    //NRF_GPIO->PIN_CNF[uc_pinSCL] = SCL_PIN_INIT_CONF;
    //NRF_GPIO->PIN_CNF[uc_pinSDA] = SDA_PIN_INIT_CONF;

    pinMode(PIN_WIRE_SDA, INPUT);
    pinMode(PIN_WIRE_SCL, INPUT);

    Serial.printf(">> pinSDA : %i \n", digitalRead(PIN_WIRE_SDA));
    Serial.printf(">> pinSCL : %i \n", digitalRead(PIN_WIRE_SCL));

    /*nrf_gpio_pin_set(uc_pinSCL);
    nrf_gpio_pin_set(uc_pinSDA);
    delay_ns(10);

    if ((nrf_gpio_pin_read(uc_pinSDA)) && (nrf_gpio_pin_read(uc_pinSCL)))
    {
        return;
    }

    for (int i = 0; i < 18; i++)
    {
        if (nrf_gpio_pin_read(uc_pinSDA))
        {
            if (i == 0)
            {
                return;
            }
            else
            {
                break;
            }
        }
        nrf_gpio_pin_clear(uc_pinSCL);
        delay_ns(10);
        nrf_gpio_pin_set(uc_pinSCL);
        delay_ns(10);
    }
    nrf_gpio_pin_clear(uc_pinSDA);
    delay_ns(10);
    nrf_gpio_pin_set(uc_pinSDA);

    NRF_GPIO->PIN_CNF[uc_pinSCL] = SCL_PIN_INIT_CONF_CLR;
    NRF_GPIO->PIN_CNF[uc_pinSDA] = SDA_PIN_INIT_CONF_CLR;

    NRF_TWIM0->ENABLE = twi_state;*/
}

void FixHwFreezed() {
    Serial.println(">> I2C FixHwFreezed");
    uint32_t twi_state = NRF_TWIM0->ENABLE;
    NRF_TWIM0->ENABLE = (TWIM_ENABLE_ENABLE_Disabled << TWIM_ENABLE_ENABLE_Pos);
    NRF_TWIM0->ENABLE = twi_state;
}

void _sleep() {
    NRF_TWIM0->ENABLE = (TWIM_ENABLE_ENABLE_Disabled << TWIM_ENABLE_ENABLE_Pos);
}

void _wakeup() {
    NRF_TWIM0->ENABLE = (TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos);
}

uint8_t _write(uint8_t deviceAddress, const uint8_t* data, size_t size, bool stop);

void wire_begin(void) {
    Serial.println(">> I2C clean");
    twi_clear_bus(_uc_pinSDA, _uc_pinSCL);

    /*Serial.println(">> I2C Unblock");
    nrf_gpio_cfg_default(_uc_pinSCL);
    nrf_gpio_cfg_default(_uc_pinSDA);
    // Unblock I2C?
    nrf_gpio_cfg(_uc_pinSCL,
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        NRF_GPIO_PIN_NOPULL,
        NRF_GPIO_PIN_S0D1,
        NRF_GPIO_PIN_NOSENSE);
    nrf_gpio_pin_set(_uc_pinSCL);
    for (uint8_t i = 0; i < 8; i++) {
        nrf_gpio_pin_toggle(_uc_pinSCL);
        delay_ns(5);
    }
    nrf_gpio_cfg_default(_uc_pinSCL);
    delay_ns(5);*/

    Serial.println(">> I2C Config");
    NRF_GPIO->PIN_CNF[_uc_pinSCL] = SCL_PIN_INIT_CONF;/*((uint32_t)GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos)
        | ((uint32_t)GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
        | ((uint32_t)GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
        | ((uint32_t)GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos)
        | ((uint32_t)GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);*/

    NRF_GPIO->PIN_CNF[_uc_pinSDA] = SDA_PIN_INIT_CONF;/*((uint32_t)GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos)
        | ((uint32_t)GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
        | ((uint32_t)GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
        | ((uint32_t)GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos)
        | ((uint32_t)GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);*/


    NRF_TWIM0->FREQUENCY = TWIM_FREQUENCY_FREQUENCY_K250;
    NRF_TWIM0->PSEL.SCL = _uc_pinSCL;
    NRF_TWIM0->PSEL.SDA = _uc_pinSDA;
    NRF_TWIM0->EVENTS_LASTRX = 0;
    NRF_TWIM0->EVENTS_STOPPED = 0;
    NRF_TWIM0->EVENTS_LASTTX = 0;
    NRF_TWIM0->EVENTS_ERROR = 0;
    NRF_TWIM0->EVENTS_RXSTARTED = 0;
    NRF_TWIM0->EVENTS_SUSPENDED = 0;
    NRF_TWIM0->EVENTS_TXSTARTED = 0;
    
    Serial.println(">> I2C Ready");
    //NRF_TWIM0->ENABLE = (TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos);
    
}

uint8_t _read(uint8_t deviceAddress, uint8_t* buffer, size_t size, bool stop) {
    NRF_TWIM0->ADDRESS = deviceAddress;
    NRF_TWIM0->TASKS_RESUME = 0x1UL;
    NRF_TWIM0->RXD.PTR = (uint32_t)buffer;
    NRF_TWIM0->RXD.MAXCNT = size;

    NRF_TWIM0->TASKS_STARTRX = 1;

    while (!NRF_TWIM0->EVENTS_RXSTARTED && !NRF_TWIM0->EVENTS_ERROR)
        ;
    NRF_TWIM0->EVENTS_RXSTARTED = 0x0UL;

    uint32_t txStartedCycleCount = DWT->CYCCNT;
    uint32_t currentCycleCount;
    while (!NRF_TWIM0->EVENTS_LASTRX && !NRF_TWIM0->EVENTS_ERROR) {
        currentCycleCount = DWT->CYCCNT;
        if ((currentCycleCount - txStartedCycleCount) > HwFreezedDelay) {
            FixHwFreezed();
            return 1;
        }
    }
    NRF_TWIM0->EVENTS_LASTRX = 0x0UL;

    if (stop || NRF_TWIM0->EVENTS_ERROR) {
        NRF_TWIM0->TASKS_STOP = 0x1UL;
        while (!NRF_TWIM0->EVENTS_STOPPED)
            ;
        NRF_TWIM0->EVENTS_STOPPED = 0x0UL;
    }
    else {
        NRF_TWIM0->TASKS_SUSPEND = 0x1UL;
        while (!NRF_TWIM0->EVENTS_SUSPENDED)
            ;
        NRF_TWIM0->EVENTS_SUSPENDED = 0x0UL;
    }

    if (NRF_TWIM0->EVENTS_ERROR) {
        NRF_TWIM0->EVENTS_ERROR = 0x0UL;
    }
    return 0;
}

uint8_t _write(uint8_t deviceAddress, const uint8_t* data, size_t size, bool stop) {
    NRF_TWIM0->ADDRESS = deviceAddress;
    NRF_TWIM0->TASKS_RESUME = 0x1UL;
    NRF_TWIM0->TXD.PTR = (uint32_t)data;
    NRF_TWIM0->TXD.MAXCNT = size;

    NRF_TWIM0->TASKS_STARTTX = 1;

    while (!NRF_TWIM0->EVENTS_TXSTARTED && !NRF_TWIM0->EVENTS_ERROR)
        ;
    NRF_TWIM0->EVENTS_TXSTARTED = 0x0UL;

    uint32_t txStartedCycleCount = DWT->CYCCNT;
    uint32_t currentCycleCount;
    while (!NRF_TWIM0->EVENTS_LASTTX && !NRF_TWIM0->EVENTS_ERROR) {
        currentCycleCount = DWT->CYCCNT;
        if ((currentCycleCount - txStartedCycleCount) > HwFreezedDelay) {
            FixHwFreezed();
            return 1;
        }
    }
    NRF_TWIM0->EVENTS_LASTTX = 0x0UL;

    if (stop || NRF_TWIM0->EVENTS_ERROR) {
        NRF_TWIM0->TASKS_STOP = 0x1UL;
        while (!NRF_TWIM0->EVENTS_STOPPED)
            ;
        NRF_TWIM0->EVENTS_STOPPED = 0x0UL;
    }
    else {
        NRF_TWIM0->TASKS_SUSPEND = 0x1UL;
        while (!NRF_TWIM0->EVENTS_SUSPENDED)
            ;
        NRF_TWIM0->EVENTS_SUSPENDED = 0x0UL;
    }

    if (NRF_TWIM0->EVENTS_ERROR) {
        NRF_TWIM0->EVENTS_ERROR = 0x0UL;
        uint32_t error = NRF_TWIM0->ERRORSRC;
        NRF_TWIM0->ERRORSRC = error;
    }

    return 0;
}

void init_i2c() {
    Serial.println("> I2C init");
    wire_begin();
}

uint8_t user_i2c_read(uint8_t addr, uint8_t reg_addr, uint8_t* reg_data, uint32_t length) {
    _wakeup();
    auto ret = _write(addr, &reg_addr, 1, false);
    ret = _read(addr, reg_data, length, true);
    _sleep();
    return ret;
}

uint8_t user_i2c_write(uint8_t addr, uint8_t reg_addr, const uint8_t* reg_data, uint32_t length) {
    uint8_t internalBuffer[17] = { 0 };
    _wakeup();
    internalBuffer[0] = reg_addr;
    memcpy(internalBuffer + 1, reg_data, length);
    auto ret = _write(addr, internalBuffer, length + 1, true);
    _sleep();
    return ret;
}


/*
void init_i2c() {

    Serial.println("> I2C init");
    Serial.println(">> Unblock I2C");
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
    nrf_gpio_cfg_default(_uc_pinSCL);

    Wire.begin();
}

uint8_t user_i2c_read(uint8_t addr, uint8_t reg_addr, uint8_t* reg_data, uint32_t length) {
    Wire.beginTransmission(addr);
    Wire.write(reg_addr);
    Wire.endTransmission(true);
    Wire.requestFrom(addr, length);
    for (uint32_t i = 0; i < length; i++) {
        if (Wire.available()) {
            *reg_data++ = Wire.read();
        } else {
            return 1;
        }

    }
    return 0;
}

uint8_t user_i2c_write(uint8_t addr, uint8_t reg_addr, const uint8_t* reg_data, uint32_t length) {

    Wire.beginTransmission(addr);
    Wire.write(reg_addr);
    for (uint32_t i = 0; i < length; i++) {
        Wire.write(*reg_data++);
    }
    Wire.endTransmission(true);
    return 0;
}
*/