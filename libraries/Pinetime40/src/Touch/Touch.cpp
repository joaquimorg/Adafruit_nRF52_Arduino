#include "Touch.h"
#include "Arduino.h"
#include "I2C/i2c.h"

#define TP_TWI_ADDR                     0x15

#define CST816_CHIP_ID			        0xB4

#define CST816_REG_GESTURE				0x01
#define CST816_REG_FINGER_NUM			0x02
#define CST816_REG_XPOS_H				0x03
#define CST816_REG_XPOS_L				0x04
#define CST816_REG_YPOS_H				0x05
#define CST816_REG_YPOS_L				0x06
#define CST816_REG_BPC0_H				0xB0
#define CST816_REG_BPC0_L				0xB1
#define CST816_REG_BPC1_H				0xB2
#define CST816_REG_BPC1_L				0xB3
#define CST816_REG_CHIPID				0xA7
#define CST816_REG_PROJID				0xA8
#define CST816_REG_FW_VER				0xA9
#define CST816_REG_SLEEP_MODE			0xE5
#define CST816_REG_ERR_RESET			0xEA
#define CST816_REG_LONG_PRESS_TICK		0xEB
#define CST816_REG_MOTION_MASK			0xEC
#define CST816_REG_IRQ_PLUSE_WIDTH		0xED
#define CST816_REG_NOR_SCAN_PER			0xEE
#define CST816_REG_MOTION_SL_ANGLE		0xEF
#define CST816_REG_LP_SCAN_RAW1_H		0xF0
#define CST816_REG_LP_SCAN_RAW1_L		0xF1
#define CST816_REG_LP_SCAN_RAW2_H		0xF2
#define CST816_REG_LP_SCAN_RAW2_L		0xF3
#define CST816_REG_LP_AUTO_WAKE_TIME	0xF4
#define CST816_REG_LP_SCAN_TH			0xF5
#define CST816_REG_LP_SCAN_WIN			0xF6
#define CST816_REG_LP_SCAN_FREQ			0xF7
#define CST816_REG_LP_SCAN_IDAC			0xF8
#define CST816_REG_AUTO_SLEEP_TIME		0xF9
#define CST816_REG_IRQ_CTL				0xFA
#define CST816_REG_AUTO_RESET			0xFB
#define CST816_REG_LONG_PRESS_TIME		0xFC
#define CST816_REG_IO_CTL				0xFD
#define CST816_REG_DIS_AUTO_SLEEP		0xFE

/**
 * Constructor
 */
Touch::Touch(void) {

}

void Touch::writeReg(uint8_t reg, uint8_t data) {
    user_i2c_write(TP_TWI_ADDR, reg, &data, 1);
    delay_ns(25);
}

void Touch::init(void) {

    /*pinMode(PIN_TP_RST, INPUT);
    Serial.printf(">> PIN_TP_RST : %i \n", digitalRead(PIN_TP_RST));
    Serial.printf(">> PIN_TP_IRQ : %i \n", digitalRead(PIN_TP_IRQ));
    delay_ns(50);*/

    pinMode(PIN_TP_RST, OUTPUT);

    digitalWrite(PIN_TP_RST, HIGH);
    delay_ns(100);

    digitalWrite(PIN_TP_RST, LOW);
    delay_ns(10);

    digitalWrite(PIN_TP_RST, HIGH);
    delay_ns(100);

    // Config touchpanel

    //The default is 0, enabling automatic entry into low-power mode
    //writeReg(CST816_REG_DIS_AUTO_SLEEP, 0x00);

    //Set reporting rate
    writeReg(CST816_REG_NOR_SCAN_PER, 0x01);

    //Report: 0x60 Gesture: 0X11 Report plus gesture: 0X71
    //Setting Mode Announcement/Gesture
    writeReg(CST816_REG_IRQ_CTL, 0x60);

    //When the unit 1S is 0, the function is not enabled, the default is 5
    //Set the automatic reset time When there is a touch but no gesture within X seconds, it will automatically reset
    writeReg(CST816_REG_AUTO_RESET, 0x0);

    //When the unit 1S is 0, the function is not enabled, the default is 10
    //Set the automatic reset time Long press X seconds to automatically reset
    writeReg(CST816_REG_LONG_PRESS_TIME, 0x10);

    //Unit 0.1mS
    //Set interrupt low pulse output width
    writeReg(CST816_REG_IRQ_PLUSE_WIDTH, 0x02);

    read_config();

}

void Touch::read_config(void) {
    Serial.println(">> Touch read config");

    /*Serial.println(">> Touch read 0x15");*/
    writeReg(0x15, 0x00);
    user_i2c_read(TP_TWI_ADDR, 0x15, &version15, 1);
    delay_ns(50);

    //Serial.println(">> Touch read CST816_REG_CHIPID");
    user_i2c_read(TP_TWI_ADDR, CST816_REG_CHIPID, &versionInfo[0], 1);
    delay_ns(25);

    //Serial.println(">> Touch read CST816_REG_PROJID");
    user_i2c_read(TP_TWI_ADDR, CST816_REG_PROJID, &versionInfo[1], 1);
    delay_ns(25);

    //Serial.println(">> Touch read CST816_REG_FW_VER");
    user_i2c_read(TP_TWI_ADDR, CST816_REG_FW_VER, &versionInfo[2], 1);
    delay_ns(25);

    Serial.printf(">> Touch CHIPID : %02X PROJID : %02X FW_VER : %02X\n", versionInfo[0], versionInfo[1], versionInfo[2]);

    if (versionInfo[0] != CST816_CHIP_ID) {
        if (versionInfo[0] == 0xFF) {
            Serial.println(">> Touch in sleep mode...");
        }
        else {
            Serial.println(">> Touch CST816 not found...");
        }
    }
}

void Touch::sleep(bool state) {
    digitalWrite(PIN_TP_RST, LOW);
    delay_ns(5);
    digitalWrite(PIN_TP_RST, HIGH);
    delay_ns(50);
    if (state) {
        uint8_t standby_value = 0x03;
        user_i2c_write(TP_TWI_ADDR, CST816_REG_SLEEP_MODE, &standby_value, 1);
    }
}

void Touch::read(void) {
    user_i2c_read(TP_TWI_ADDR, CST816_REG_FINGER_NUM, data_raw, 6);
}


void Touch::get(void) {

    byte raw[8];

    user_i2c_read(TP_TWI_ADDR, CST816_REG_GESTURE, raw, 6);

    gesture = static_cast<Gestures>(raw[0]);

    touchpoints = raw[1];

    event = raw[2] >> 6;
    xpos = raw[3];
    ypos = raw[5];

}


Touch::Gestures Touch::readGesture(void) {

    byte raw[1];

    user_i2c_read(TP_TWI_ADDR, CST816_REG_GESTURE, raw, 1);
    gesture = static_cast<Gestures>(raw[0]);
    return gesture;
}