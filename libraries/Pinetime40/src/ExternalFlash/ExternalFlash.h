#ifndef EXTERNALFLASH_H_
#define EXTERNALFLASH_H_

#include "Adafruit_LittleFS.h"
#include "Adafruit_SPIFlash.h"

class ExternalFlash: public Adafruit_LittleFS
{
public:
    ExternalFlash(void);

    // overwrite to also perform low level format (sector erase of whole flash region)
    bool begin(void);
    void dump_sector(uint32_t sector, uint32_t off);
    void dump_buf(uint8_t* buf, uint16_t size);

    Adafruit_SPIFlash spiFlash;
    
};

extern ExternalFlash ExternalFS;

#endif /* EXTERNALFLASH_H_ */