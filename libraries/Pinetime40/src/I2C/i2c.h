/*
 * Copyright (c) 2020 Aaron Christophel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus */

void init_i2c();

uint8_t user_i2c_read(uint8_t addr, uint8_t reg_addr, uint8_t *reg_data, uint32_t length);
uint8_t user_i2c_write(uint8_t addr, uint8_t reg_addr, const uint8_t *reg_data, uint32_t length);

#ifdef __cplusplus
}
#endif /*__cplusplus */