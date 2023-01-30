#include "Arduino.h"
#include "ExternalFlash.h"
#include "nrf52_qspi.h"

//#define SHOW_DEBUG

enum {
    SFLASH_CMD_READ = 0x03,      // Single Read
    SFLASH_CMD_FAST_READ = 0x0B, // Fast Read
    SFLASH_CMD_QUAD_READ = 0x6B, // 1 line address, 4 line data

    SFLASH_CMD_READ_JEDEC_ID = 0x9f,

    SFLASH_CMD_PAGE_PROGRAM = 0x02,
    SFLASH_CMD_QUAD_PAGE_PROGRAM = 0x32, // 1 line address, 4 line data

    SFLASH_CMD_READ_STATUS = 0x05,
    SFLASH_CMD_READ_STATUS2 = 0x35,

    SFLASH_CMD_WRITE_STATUS = 0x01,
    SFLASH_CMD_WRITE_STATUS2 = 0x31,

    SFLASH_CMD_ENABLE_RESET = 0x66,
    SFLASH_CMD_RESET = 0x99,

    SFLASH_CMD_WRITE_ENABLE = 0x06,
    SFLASH_CMD_WRITE_DISABLE = 0x04,

    SFLASH_CMD_ERASE_SECTOR = 0x20,
    SFLASH_CMD_ERASE_BLOCK = 0xD8,
    SFLASH_CMD_ERASE_CHIP = 0xC7,

    SFLASH_CMD_4_BYTE_ADDR = 0xB7,
    SFLASH_CMD_3_BYTE_ADDR = 0xE9,
};

/// Constant that is (mostly) true to all external flash devices
enum {
    SFLASH_BLOCK_SIZE = 64 * 1024UL,
    SFLASH_SECTOR_SIZE = 4 * 1024,
    SFLASH_PAGE_SIZE = 256,
    SFLASH_START = 4096,
};

#define LFS_FLASH_TOTAL_SIZE  0x3FF000

void dump_buffer(uint8_t* buf, uint16_t size)
{

    for (uint32_t row = 0; row < size / 16; row++)
    {
        if (row == 0) Serial.print("0");
        if (row < 16) Serial.print("0");
        Serial.print(row * 16, HEX);
        Serial.print(" : ");

        for (uint32_t col = 0; col < 16; col++)
        {
            uint8_t val = buf[row * 16 + col];

            if (val < 16) Serial.print("0");
            Serial.print(val, HEX);

            Serial.print(" ");
        }

        Serial.println();
    }
}

static inline uint32_t lba2addr(uint32_t block)
{
    return SFLASH_START + (block * SFLASH_SECTOR_SIZE);
}

static int _external_flash_read(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size)
{
    (void)c;
    if (size == 0) return 0;
    //ExternalFlash& eflash = *(static_cast<ExternalFlash*>(c->context));
    uint32_t addr = lba2addr(block) + off;
    //eflash.readBuffer(addr, (uint8_t*)buffer, size);    
    nrf52_qspi_read_write_memory(true, addr, (uint8_t*)buffer, size);

#ifdef SHOW_DEBUG    
    Serial.print("E R > ");
    Serial.print(block);
    Serial.print(" : ");
    Serial.print(off);
    Serial.print(" : ");
    Serial.print(size);
    Serial.println("");
    dump_buffer((uint8_t*)buffer, size);
#endif
    return 0;
}

// Program a region in a block. The block must have previously
// been erased. Negative error codes are propogated to the user.
// May return LFS_ERR_CORRUPT if the block should be considered bad.
static int _external_flash_prog(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size)
{
    (void)c;
    //ExternalFlash& eflash = *(static_cast<ExternalFlash*>(c->context));
    uint32_t addr = lba2addr(block) + off;
    //eflash.writeBuffer(addr, (const uint8_t*)buffer, size);
    nrf52_qspi_read_write_memory(false, addr, (uint8_t*)buffer, size);    
#ifdef SHOW_DEBUG        
    Serial.print("E W > ");
    Serial.print(block);
    Serial.print(" : ");
    Serial.print(off);
    Serial.print(" : ");
    Serial.print(size);
    Serial.println("");
    dump_buffer((uint8_t*)buffer, size);
#endif    
    return 0;
}

// Erase a block. A block must be erased before being programmed.
// The state of an erased block is undefined. Negative error codes
// are propogated to the user.
// May return LFS_ERR_CORRUPT if the block should be considered bad.
static int _external_flash_erase(const struct lfs_config* c, lfs_block_t block)
{
    (void)c;
    //ExternalFlash& eflash = *(static_cast<ExternalFlash*>(c->context));
    uint32_t addr = lba2addr(block);
#ifdef SHOW_DEBUG     
    Serial.print("E E > ");
    Serial.print(block);
    Serial.println("");
#endif    
    //eflash.eraseSector(addr);
    nrf52_qspi_erase_command(SFLASH_CMD_ERASE_SECTOR, addr * SFLASH_SECTOR_SIZE);
    return 0;
}

// Sync the state of the underlying block device.
static int _external_flash_sync(const struct lfs_config* c)
{
    (void)c;
    //ExternalFlash& eflash = *(static_cast<ExternalFlash*>(c->context));
    //eflash.spiFlash.syncDevice();
    return 0;
}

static struct lfs_config _ExternalFSConfig =
{
    .context = NULL,

    .read = _external_flash_read,
    .prog = _external_flash_prog,
    .erase = _external_flash_erase,
    .sync = _external_flash_sync,

    .read_size = 128,
    .prog_size = 128,
    .block_size = SFLASH_SECTOR_SIZE,
    .block_count = LFS_FLASH_TOTAL_SIZE / SFLASH_SECTOR_SIZE,
    .block_cycles = 5000,
    .cache_size = 128,
    .lookahead_size = 128,
    .name_max = 50,
    .attr_max = 50,
};

ExternalFlash ExternalFS;

//--------------------------------------------------------------------
/*
#define XT25F32BDFIGT \
  { \
    .total_size = 0x400000, \
    .start_up_time_us = 5000, .manufacturer_id = 0x0b, \
    .memory_type = 0x40, .capacity = 0x16, .max_clock_speed_mhz = 16, \
    .quad_enable_bit_mask = 0x02, .has_sector_protection = false, \
    .supports_fast_read = true, .supports_qspi = true, \
    .supports_qspi_writes = false, .write_status_register_split = false, \
    .single_status_byte = false, .is_fram = false, \
  }

static const SPIFlash_Device_t my_flash_devices[] = {
  XT25F32BDFIGT,
};
const int flashDevices = 1;
*/

ExternalFlash::ExternalFlash(void)
    : Adafruit_LittleFS(&_ExternalFSConfig)
{
}

void ExternalFlash::dump_buf(uint8_t* buf, uint16_t size)
{

    for (uint32_t row = 0; row < size / 16; row++)
    {
        if (row == 0) Serial.print("0");
        if (row < 16) Serial.print("0");
        Serial.print(row * 16, HEX);
        Serial.print(" : ");

        for (uint32_t col = 0; col < 16; col++)
        {
            uint8_t val = buf[row * 16 + col];

            if (val < 16) Serial.print("0");
            Serial.print(val, HEX);

            Serial.print(" ");
        }

        Serial.println();
    }
}

uint8_t ExternalFlash::readStatus(void) {
    uint8_t status;
    nrf52_qspi_read_command(SFLASH_CMD_READ_STATUS, &status, 1);
    return status;
}

uint8_t ExternalFlash::readStatus2(void) {
    uint8_t status;
    nrf52_qspi_read_command(SFLASH_CMD_READ_STATUS2, &status, 1);
    return status;
}

bool ExternalFlash::writeEnable(void) {
    return nrf52_qspi_run_command(SFLASH_CMD_WRITE_ENABLE);
}

bool ExternalFlash::writeDisable(void) {
    return nrf52_qspi_run_command(SFLASH_CMD_WRITE_DISABLE);
}

void ExternalFlash::waitUntilReady(void) {
    // both WIP and WREN bit should be clear
    while (readStatus() & 0x03) {
        yield();
    }
}

bool ExternalFlash::eraseChip(void) {
    waitUntilReady();
    writeEnable();

    bool const ret = nrf52_qspi_run_command(SFLASH_CMD_ERASE_CHIP);
    return ret;
}

bool ExternalFlash::eraseSector(uint32_t sectorNumber) {

    // Before we erase the sector we need to wait for any writes to finish
    waitUntilReady();
    writeEnable();

    bool const ret = nrf52_qspi_erase_command(SFLASH_CMD_ERASE_SECTOR,
        sectorNumber * SFLASH_SECTOR_SIZE);


    return ret;
}

uint32_t ExternalFlash::readBuffer(uint32_t address, uint8_t* buffer, uint32_t len) {
    waitUntilReady();
    bool const rc = nrf52_qspi_read_write_memory(true, address, buffer, len);
    return rc ? len : 0;
}

uint32_t ExternalFlash::writeBuffer(uint32_t address, uint8_t const* buffer, uint32_t len) {

    uint32_t remain = len;

    // write one page (256 bytes) at a time and
    // must not go over page boundary
    while (remain) {
        waitUntilReady();
        writeEnable();

        uint32_t const leftOnPage =
            SFLASH_PAGE_SIZE - (address & (SFLASH_PAGE_SIZE - 1));
        uint32_t const toWrite = min(remain, leftOnPage);

        if (!nrf52_qspi_read_write_memory(false, address, (uint8_t*)buffer, toWrite))
            break;

        remain -= toWrite;
        buffer += toWrite;
        address += toWrite;
    }

    len -= remain;


    return len;
}

bool ExternalFlash::begin(void)
{
    /*lfs_config lcfg = {
        .context = this,

        .read = _external_flash_read,
        .prog = _external_flash_prog,
        .erase = _external_flash_erase,
        .sync = _external_flash_sync,

        .read_size = 128,
        .prog_size = 128,
        .block_size = SFLASH_PAGE_SIZE,
        .block_count = LFS_FLASH_TOTAL_SIZE / SFLASH_PAGE_SIZE,
        .block_cycles = 5000,
        .cache_size = 256,
        .lookahead_size = 256,
        .name_max = 50,
        .attr_max = 50,
    };*/

    Serial.println("Serial Flash Info : ");

    nrf52_qspi_begin();

    uint8_t jedec_ids[4];
    nrf52_qspi_read_command(SFLASH_CMD_READ_JEDEC_ID, jedec_ids, 4);

    Serial.printf("JEDEC ID: %02X %02X %02X %02X\n", jedec_ids[0], jedec_ids[1], jedec_ids[2], jedec_ids[3]);

    Serial.println("");

    // The write in progress bit should be low.
    while (readStatus() & 0x01) {}

    // The suspended write/erase bit should be low.    
    while (readStatus2() & 0x80) {}

    nrf52_qspi_run_command(SFLASH_CMD_ENABLE_RESET);
    nrf52_qspi_run_command(SFLASH_CMD_RESET);

    // Wait 30us for the reset
    delayMicroseconds(30);

    uint8_t status = readStatus2();

    // Check the quad enable bit.
    if ((status & 0x02) == 0) {
        writeEnable();
        uint8_t full_status[2] = { 0x00, 0x02 };
        nrf52_qspi_write_command(SFLASH_CMD_WRITE_STATUS, full_status, 2);
    }

    //nrf52_qspi_run_command(SFLASH_CMD_4_BYTE_ADDR);

    /*Serial.println("> QSPI Init done.");
    Serial.println("Erasing chip!");
    if (!eraseChip()) {
        Serial.println("Failed to erase chip!");
    }

    waitUntilReady();
    Serial.println("Successfully erased chip!");

    return false;*/


    /*
    //waitUntilReady();

    uint8_t buf[5] = { 0x00, 0x01, 0x02, 0x03, 0x04 };

    Serial.println("Write Sector 0:0!");
    writeBuffer(0, buf, 5);

    waitUntilReady();
    Serial.println("Successfully write to chip!");*/

    /*Serial.println("Dump Sectors ");
    dump_sector(0, 0);
    dump_sector(1, 0);
    dump_sector(2, 0);*/

    //return false;
    // failed to mount, erase all sector then format and mount again
    if (!Adafruit_LittleFS::begin())
    {
        Serial.println("Erasing External chip!");
        if (!eraseChip()) {
            Serial.println("Failed to erase chip!");
            return false;
        }
        waitUntilReady();
        Serial.println("Successfully erased chip!");

        Serial.println("Format External fs");
        // lfs format
        this->format();

        // mount again if still failed, give up
        if (!Adafruit_LittleFS::begin()) return false;
    }
    Serial.println("External FS ready !");

    return true;
}