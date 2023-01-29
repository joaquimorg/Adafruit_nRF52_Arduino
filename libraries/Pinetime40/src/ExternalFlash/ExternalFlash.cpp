
#include "ExternalFlash.h"
#include "flash_config.h"

#define LFS_FLASH_TOTAL_SIZE  0x400000
#define LFS_BLOCK_SIZE        4096

static inline uint32_t lba2addr(uint32_t block)
{
    return LFS_BLOCK_SIZE + (block * LFS_BLOCK_SIZE);
}

static int _external_flash_read(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size)
{
    //(void)c;    
    ExternalFlash& eflash = *(static_cast<ExternalFlash*>(c->context));
    uint32_t addr = lba2addr(block) + off;
    Serial.print("E R > ");
    Serial.print(block);
    Serial.print(" : ");
    Serial.print(off);
    Serial.print(" : ");
    Serial.print(size);
    Serial.println("");
    //eflash.spiFlash.readSectors(addr, (uint8_t*)buffer, size);
    eflash.spiFlash.readBuffer(addr, (uint8_t*)buffer, size);
    eflash.dump_buf((uint8_t*)buffer, size);
    return 0;
}

// Program a region in a block. The block must have previously
// been erased. Negative error codes are propogated to the user.
// May return LFS_ERR_CORRUPT if the block should be considered bad.
static int _external_flash_prog(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size)
{
    //(void)c;
    ExternalFlash& eflash = *(static_cast<ExternalFlash*>(c->context));
    uint32_t addr = lba2addr(block) + off;
    Serial.print("E W > ");
    Serial.print(block);
    Serial.print(" : ");
    Serial.print(off);
    Serial.print(" : ");
    Serial.print(size);
    Serial.println("");
    //eflash.spiFlash.writeSectors(addr, (uint8_t*)buffer, size);
    eflash.spiFlash.writeBuffer(addr, (const uint8_t*)buffer, size);
    eflash.dump_buf((uint8_t*)buffer, size);
    return 0;
}

// Erase a block. A block must be erased before being programmed.
// The state of an erased block is undefined. Negative error codes
// are propogated to the user.
// May return LFS_ERR_CORRUPT if the block should be considered bad.
static int _external_flash_erase(const struct lfs_config* c, lfs_block_t block)
{
    //(void)c;
    ExternalFlash& eflash = *(static_cast<ExternalFlash*>(c->context));
    uint32_t addr = lba2addr(block);
    /*Serial.print("E E > ");
    Serial.print(block);
    Serial.println("");*/
    eflash.spiFlash.eraseSector(addr);
    return 0;
}

// Sync the state of the underlying block device.
static int _external_flash_sync(const struct lfs_config* c)
{
    //(void)c;
    ExternalFlash& eflash = *(static_cast<ExternalFlash*>(c->context));
    eflash.spiFlash.syncDevice();
    return 0;
}

ExternalFlash ExternalFS;

//--------------------------------------------------------------------

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

ExternalFlash::ExternalFlash(void)
    : Adafruit_LittleFS()
{
    spiFlash = Adafruit_SPIFlash(&flashTransport);
}

void ExternalFlash::dump_sector(uint32_t sector, uint32_t off)
{
    uint8_t buf[256];
    memset(buf, 0xff, sizeof(buf));

    Serial.print("External ");
    Serial.print(sector);
    Serial.print(" : ");
    Serial.println(off);

    spiFlash.readBuffer((sector * LFS_BLOCK_SIZE) + off, buf, 256);

    for (uint32_t row = 0; row < sizeof(buf) / 16; row++)
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

bool ExternalFlash::begin(void)
{
    lfs_config lcfg = {
        .context = this,

        .read = _external_flash_read,
        .prog = _external_flash_prog,
        .erase = _external_flash_erase,
        .sync = _external_flash_sync,

        .read_size = 256,
        .prog_size = 256,
        .block_size = LFS_BLOCK_SIZE,
        .block_count = LFS_FLASH_TOTAL_SIZE / LFS_BLOCK_SIZE,
        .block_cycles = 1000,
        .cache_size = 256,
        .lookahead_size = 8,
        .name_max = 50,
        .attr_max = 50,
    };

    Serial.println("Serial Flash Info : ");
    spiFlash.begin(my_flash_devices, flashDevices);

    Serial.print("JEDEC ID: 0x");
    Serial.println(spiFlash.getJEDECID(), HEX);
    Serial.print("Flash size: ");
    Serial.print(spiFlash.size() / 1024);
    Serial.println(" KB");
    Serial.println("");

    /*Serial.println("Erasing chip!");
    if (!spiFlash.eraseChip()) {
        Serial.println("Failed to erase chip!");
    }

    spiFlash.waitUntilReady();
    Serial.println("Successfully erased chip!");

    return false;*/


    /*
    //spiFlash.waitUntilReady();

    uint8_t buf[5] = { 0x00, 0x01, 0x02, 0x03, 0x04 };

    Serial.println("Write Sector 0:0!");
    spiFlash.writeBuffer(0, buf, 5);

    spiFlash.waitUntilReady();
    Serial.println("Successfully write to chip!");*/

    /*Serial.println("Dump Sectors ");
    dump_sector(0, 0);
    dump_sector(1, 0);
    dump_sector(2, 0);*/

    //return false;
    // failed to mount, erase all sector then format and mount again
    if (!Adafruit_LittleFS::begin(&lcfg))
    {
        Serial.println("Erasing External chip!");
        if (!spiFlash.eraseChip()) {
            Serial.println("Failed to erase chip!");
            return false;
        }
        spiFlash.waitUntilReady();
        Serial.println("Successfully erased chip!");

        Serial.println("Format External fs");
        // lfs format
        this->format();

        // mount again if still failed, give up
        if (!Adafruit_LittleFS::begin(&lcfg)) return false;
    }
    Serial.println("External FS ready !");
    return true;
}