/*
 * Copyright (c) 2020 Aaron Christophel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

void write_fast_spi_(const uint8_t* ptr, uint32_t len);

void init_fast_spi();
void enable_spi(bool state);
void write_fast_spi(const uint8_t* ptr, uint32_t len, bool mode);
void read_fast_spi(uint8_t* ptr, uint32_t len);