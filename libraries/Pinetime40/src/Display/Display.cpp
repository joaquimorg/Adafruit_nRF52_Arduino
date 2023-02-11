#include "Display.h"
#include "fast_spi.h"


// ST7789 specific commands used in init
#define ST7789_NOP 0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID 0x04
#define ST7789_RDDST 0x09

#define ST7789_RDDPM 0x0A       // Read display power mode
#define ST7789_RDD_MADCTL 0x0B  // Read display MADCTL
#define ST7789_RDD_COLMOD 0x0C  // Read display pixel format
#define ST7789_RDDIM 0x0D       // Read display image mode
#define ST7789_RDDSM 0x0E       // Read display signal mode
#define ST7789_RDDSR 0x0F       // Read display self-diagnostic result (ST7789V)

#define ST7789_SLPIN 0x10
#define ST7789_SLPOUT 0x11
#define ST7789_PTLON 0x12
#define ST7789_NORON 0x13

#define ST7789_INVOFF 0x20
#define ST7789_INVON 0x21
#define ST7789_GAMSET 0x26  // Gamma set
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON 0x29
#define ST7789_CASET 0x2A
#define ST7789_RASET 0x2B
#define ST7789_RAMWR 0x2C
#define ST7789_RGBSET 0x2D  // Color setting for 4096, 64K and 262K colors
#define ST7789_RAMRD 0x2E

#define ST7789_PTLAR 0x30
#define ST7789_VSCRDEF 0x33   // Vertical scrolling definition (ST7789V)
#define ST7789_TEOFF 0x34     // Tearing effect line off
#define ST7789_TEON 0x35      // Tearing effect line on
#define ST7789_MADCTL 0x36    // Memory data access control
#define ST7789_VSCRSADD 0x37  // Vertical screoll address
#define ST7789_IDMOFF 0x38    // Idle mode off
#define ST7789_IDMON 0x39     // Idle mode on
#define ST7789_RAMWRC 0x3C    // Memory write continue (ST7789V)
#define ST7789_RAMRDC 0x3E    // Memory read continue (ST7789V)
#define ST7789_COLMOD 0x3A

#define ST7789_RAMCTRL 0xB0    // RAM control
#define ST7789_RGBCTRL 0xB1    // RGB control
#define ST7789_PORCTRL 0xB2    // Porch control
#define ST7789_FRCTRL1 0xB3    // Frame rate control
#define ST7789_PARCTRL 0xB5    // Partial mode control
#define ST7789_GCTRL 0xB7      // Gate control
#define ST7789_GTADJ 0xB8      // Gate on timing adjustment
#define ST7789_DGMEN 0xBA      // Digital gamma enable
#define ST7789_VCOMS 0xBB      // VCOMS setting
#define ST7789_LCMCTRL 0xC0    // LCM control
#define ST7789_IDSET 0xC1      // ID setting
#define ST7789_VDVVRHEN 0xC2   // VDV and VRH command enable
#define ST7789_VRHS 0xC3       // VRH set
#define ST7789_VDVSET 0xC4     // VDV setting
#define ST7789_VCMOFSET 0xC5   // VCOMS offset set
#define ST7789_FRCTR2 0xC6     // FR Control 2
#define ST7789_CABCCTRL 0xC7   // CABC control
#define ST7789_REGSEL1 0xC8    // Register value section 1
#define ST7789_REGSEL2 0xCA    // Register value section 2
#define ST7789_PWMFRSEL 0xCC   // PWM frequency selection
#define ST7789_PWCTRL1 0xD0    // Power control 1
#define ST7789_VAPVANEN 0xD2   // Enable VAP/VAN signal output
#define ST7789_CMD2EN 0xDF     // Command 2 enable
#define ST7789_PVGAMCTRL 0xE0  // Positive voltage gamma control
#define ST7789_NVGAMCTRL 0xE1  // Negative voltage gamma control
#define ST7789_DGMLUTR 0xE2    // Digital gamma look-up table for red
#define ST7789_DGMLUTB 0xE3    // Digital gamma look-up table for blue
#define ST7789_GATECTRL 0xE4   // Gate control
#define ST7789_SPI2EN 0xE7     // SPI2 enable
#define ST7789_PWCTRL2 0xE8    // Power control 2
#define ST7789_EQCTRL 0xE9     // Equalize time control
#define ST7789_PROMCTRL 0xEC   // Program control
#define ST7789_PROMEN 0xFA     // Program mode enable
#define ST7789_NVMSET 0xFC     // NVM setting
#define ST7789_PROMACT 0xFE    // Program action

#define TFT_MAD_RGB 0x00
#define TFT_MAD_BGR 0x08
#define TFT_INVOFF 0x20
#define TFT_INVON 0x21

#define TFT_MAD_COLOR_ORDER TFT_MAD_BGR

void write_command(uint8_t value) {
    write_fast_spi(&value, 1, false);
}
void write_data(uint8_t value) {
    write_fast_spi(&value, 1, true);
}
void write_buffer(const uint8_t* ptr, uint32_t len) {
    write_fast_spi(ptr, len, true);
}

void set_addr_display(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    uint8_t temp[4];
    write_command(ST7789_CASET);
    temp[0] = (x >> 8);
    temp[1] = x;
    temp[2] = ((x + w - 1) >> 8);
    temp[3] = (x + w - 1);
    write_buffer(temp, 4);
    write_command(ST7789_RASET);
    temp[0] = (y >> 8);
    temp[1] = y;
    temp[2] = ((y + h - 1) >> 8);
    temp[3] = ((y + h - 1) & 0xFF);
    write_buffer(temp, 4);
    //write_command(ST7789_RAMWR);
}

void init_display(void) {

    // start the SPI library:
    init_fast_spi();
    
    pinMode(PIN_LCD_CSN, OUTPUT);
    digitalWrite(PIN_LCD_CSN, HIGH);

    pinMode(PIN_LCD_DC, OUTPUT);
    digitalWrite(PIN_LCD_DC, HIGH);

    pinMode(PIN_LCD_RST, OUTPUT);
    digitalWrite(PIN_LCD_RST, HIGH);

    digitalWrite(PIN_LCD_CSN, LOW);

    enable_spi(true);

    write_command(0x00);  // Put SPI bus in known state for TFT with CS tied low
    digitalWrite(PIN_LCD_RST, HIGH);
    delay(5);
    digitalWrite(PIN_LCD_RST, LOW);
    delay(20);
    digitalWrite(PIN_LCD_RST, HIGH);

    delay(150);  // Wait for reset to complete

    write_command(ST7789_SWRESET);
    delay(150);  // Wait for reset to complete

    write_command(ST7789_SLPOUT);  // Sleep out
    delay(120);

    write_command(ST7789_NORON);  // Normal display mode on

    //------------------------------display and color format setting--------------------------------//
    write_command(ST7789_MADCTL);
    //write_data(0x00);
    write_data(TFT_MAD_COLOR_ORDER);

    // JLX240 display datasheet
    /*write_command(0xB6);
    write_data(0x0A);
    write_data(0x82);*/

    write_command(ST7789_RAMCTRL);
    write_data(0x00);
    write_data(0xE0);  // 5 to 6 bit conversion: r0 = r5, b0 = b5

    write_command(ST7789_COLMOD);
    write_data(0x55);  //16bit/pixel
    //write_data(0x53); // 12bit/pixel
    delay(10);

    //--------------------------------ST7789V Frame rate setting----------------------------------//
    write_command(ST7789_PORCTRL);
    write_data(0x0c);
    write_data(0x0c);
    write_data(0x00);
    write_data(0x33);
    write_data(0x33);

    write_command(ST7789_GCTRL);  // Voltages: VGH / VGL
    write_data(0x35);

    //---------------------------------ST7789V Power setting--------------------------------------//
    write_command(ST7789_VCOMS);
    write_data(0x28);  // JLX240 display datasheet

    write_command(ST7789_LCMCTRL);
    write_data(0x0C);

    write_command(ST7789_VDVVRHEN);
    write_data(0x01);
    write_data(0xFF);

    write_command(ST7789_VRHS);  // voltage VRHS
    write_data(0x10);

    write_command(ST7789_VDVSET);
    write_data(0x20);

    write_command(ST7789_FRCTR2);
    write_data(0x0f);

    write_command(ST7789_PWCTRL1);
    write_data(0xa4);
    write_data(0xa1);

    //--------------------------------ST7789V gamma setting---------------------------------------//
    write_command(ST7789_PVGAMCTRL);
    write_data(0xd0);
    write_data(0x00);
    write_data(0x02);
    write_data(0x07);
    write_data(0x0a);
    write_data(0x28);
    write_data(0x32);
    write_data(0x44);
    write_data(0x42);
    write_data(0x06);
    write_data(0x0e);
    write_data(0x12);
    write_data(0x14);
    write_data(0x17);

    write_command(ST7789_NVGAMCTRL);
    write_data(0xd0);
    write_data(0x00);
    write_data(0x02);
    write_data(0x07);
    write_data(0x0a);
    write_data(0x28);
    write_data(0x31);
    write_data(0x54);
    write_data(0x47);
    write_data(0x0e);
    write_data(0x1c);
    write_data(0x17);
    write_data(0x1b);
    write_data(0x1e);

    //write_command(ST7789_INVON);

    write_command(ST7789_CASET);  // Column address set
    write_data(0x00);
    write_data(0x00);
    write_data(0x00);
    write_data(0xE5);  // 239

    write_command(ST7789_RASET);  // Row address set
    write_data(0x00);
    write_data(0x00);
    write_data(0x00);
    write_data(0xE5);  // 239

    delay(120);

    write_command(ST7789_DISPON);  //Display on
    delay(120);

    write_command(TFT_INVON);

}

#define PX_RGB444_RGB(r, g, b) ((uint16_t)((r) << 8 | (g) << 4 | (b)))

inline uint32_t Rev16(uint32_t a) {
    asm("rev16 %1,%0"
        : "=r"(a)
        : "r"(a));
    return a;
}

void swapBytes(uint16_t* buffer, int size) {
    //for (int i = 0; i < size; i++) {
#pragma unroll(4)
    while (size--) {
        *buffer = Rev16(*buffer);
        buffer++;
    }
}


void draw_buffer(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* data) {
    set_addr_display(x, y, w, h);
    write_command(ST7789_RAMWR);
    write_buffer(data, (w * h) * 2);
}


void displaySleep(void) {    
    write_command(ST7789_SLPIN);
    write_command(ST7789_DISPOFF);    
    //digitalWrite(LCD_CSN, LOW);
    //digitalWrite(LCD_DC, LOW);
}

void displayWakeUp(void) {
    write_command(ST7789_DISPON);
    write_command(ST7789_SLPOUT);
}
