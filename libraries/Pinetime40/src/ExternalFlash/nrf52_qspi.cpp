#include "Arduino.h"
#include "nrf52_qspi.h"
#include "nrfx_qspi.h"

void nrf52_qspi_begin(void) {
    // Init QSPI flash
    nrfx_qspi_config_t qspi_cfg = {
        .xip_offset = 0,
        .pins =
            {
                .sck_pin = (uint8_t)g_ADigitalPinMap[PIN_QSPI_SCK],
                .csn_pin = (uint8_t)g_ADigitalPinMap[PIN_QSPI_CS],
                .io0_pin = (uint8_t)g_ADigitalPinMap[PIN_QSPI_IO0],
                .io1_pin = (uint8_t)g_ADigitalPinMap[PIN_QSPI_IO1],
                .io2_pin = (uint8_t)g_ADigitalPinMap[PIN_QSPI_IO2],
                .io3_pin = (uint8_t)g_ADigitalPinMap[PIN_QSPI_IO3],
            },
        .prot_if = {.readoc = NRF_QSPI_READOC_READ4O, // 0x6B read command
                    .writeoc = NRF_QSPI_WRITEOC_PP4O, // 0x32 write command
                    .addrmode = NRF_QSPI_ADDRMODE_24BIT,
                    .dpmconfig = false},
        .phy_if =
            {
                .sck_delay = 10,
                .dpmen = false,
                .spi_mode = NRF_QSPI_MODE_0,
                .sck_freq = NRF_QSPI_FREQ_32MDIV8,
            },
        .irq_priority = 7 };

    // No callback for blocking API
    nrfx_qspi_init(&qspi_cfg, NULL, NULL);
}

void nrf52_qspi_end(void) {
    nrfx_qspi_uninit();
}

/* -------------------------------------------------------------- */

bool nrf52_qspi_run_command(uint8_t command) {
    nrf_qspi_cinstr_conf_t cinstr_cfg = { .opcode = command,
                                         .length = NRF_QSPI_CINSTR_LEN_1B,
                                         .io2_level = true,
                                         .io3_level = true,
                                         .wipwait = false,
                                         .wren = false };

    return nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, NULL) == NRFX_SUCCESS;
}

bool nrf52_qspi_read_command(uint8_t command,
    uint8_t* response,
    uint32_t len) {
    nrf_qspi_cinstr_conf_t cinstr_cfg = { .opcode = command,
                                         .length =
                                             (nrf_qspi_cinstr_len_t)(len + 1),
                                         .io2_level = true,
                                         .io3_level = true,
                                         .wipwait = false,
                                         .wren = false };
    return nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, response) == NRFX_SUCCESS;
}

bool nrf52_qspi_write_command(uint8_t command,
    uint8_t const* data,
    uint32_t len) {
    nrf_qspi_cinstr_conf_t cinstr_cfg = {
        .opcode = command,
        .length = (nrf_qspi_cinstr_len_t)(len + 1),
        .io2_level = true,
        .io3_level = true,
        .wipwait = false,
        .wren = false // We do this manually.
    };
    return nrfx_qspi_cinstr_xfer(&cinstr_cfg, data, NULL) == NRFX_SUCCESS;
}

bool nrf52_qspi_erase_command(uint8_t command,
    uint32_t address) {
    nrf_qspi_erase_len_t erase_len = NRF_QSPI_ERASE_LEN_4KB;

    return NRFX_SUCCESS == nrfx_qspi_erase(erase_len, address);
}

/* ---------------------------------------------------------------------- */

uint32_t nrf52_qspi_read_write_odd(bool read_op, uint32_t addr, uint8_t* data,
    uint32_t len) {
    uint8_t buf4[4] __attribute__((aligned(4)));
    uint32_t count = 4 - (((uint32_t)data) & 0x03);
    count = min(count, len);

    if (read_op) {
        if (NRFX_SUCCESS != nrfx_qspi_read(buf4, 4, addr)) {
            return 0;
        }

        memcpy(data, buf4, count);
    }
    else {
        memset(buf4, 0xff, 4);
        memcpy(buf4, data, count);

        if (NRFX_SUCCESS != nrfx_qspi_write(buf4, 4, addr)) {
            return 0;
        }
    }

    return count;
}

bool nrf52_qspi_read_write_memory(bool read_op, uint32_t addr, uint8_t* data,
    uint32_t len) {
    // buffer is not 4-byte aligned
    if (((uint32_t)data) & 3) {
        uint32_t const count = nrf52_qspi_read_write_odd(read_op, addr, data, len);
        if (!count) {
            return false;
        }

        data += count;
        addr += count;
        len -= count;
    }

    // nrfx_qspi_read works in 4 byte increments, though it doesn't
    // signal an error if sz is not a multiple of 4.  Read (directly into data)
    // all but the last 1, 2, or 3 bytes depending on the (remaining) length.
    if (len > 3) {
        uint32_t const len4 = len & ~(uint32_t)3;

        if (read_op) {
            if (NRFX_SUCCESS != nrfx_qspi_read(data, len4, addr)) {
                return 0;
            }
        }
        else {
            if (NRFX_SUCCESS != nrfx_qspi_write(data, len4, addr)) {
                return 0;
            }
        }

        data += len4;
        addr += len4;
        len -= len4;
    }

    // Now, if we have any bytes left over, we must do a final read of 4
    // bytes and copy 1, 2, or 3 bytes to data.
    if (len) {
        if (!nrf52_qspi_read_write_odd(read_op, addr, data, len)) {
            return false;
        }
    }

    return true;
}

