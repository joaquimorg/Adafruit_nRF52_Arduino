#ifndef EXTERNALFLASH_H_
#define EXTERNALFLASH_H_

#include "Adafruit_LittleFS.h"
//#include "Adafruit_SPIFlash.h"

using namespace Adafruit_LittleFS_Namespace;

class ExternalFlash: public Adafruit_LittleFS
{
public:
    ExternalFlash(void);

    // overwrite to also perform low level format (sector erase of whole flash region)
    bool begin(void);
    void dump_buf(uint8_t* buf, uint16_t size);
    uint8_t readStatus(void);
    uint8_t readStatus2(void);
    bool writeEnable(void);
    bool writeDisable(void);
    void waitUntilReady(void);
    bool eraseChip(void);
    bool eraseSector(uint32_t sectorNumber);
    uint32_t readBuffer(uint32_t address, uint8_t* buffer, uint32_t len);
    uint32_t writeBuffer(uint32_t address, uint8_t const* buffer, uint32_t len);

    //Adafruit_SPIFlash spiFlash;

};

extern ExternalFlash ExternalFS;

#endif /* EXTERNALFLASH_H_ */