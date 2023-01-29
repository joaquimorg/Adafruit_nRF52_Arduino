#ifndef BACKLIGHT_CONFIG_H
#define BACKLIGHT_CONFIG_H

#include "stdint.h"
#include "Arduino.h"
#include "lvgl.h"

class Backlight
{
  protected:
    //SoftwareTimer lightTimer;
    lv_timer_t *  lightTimer;
    uint16_t backlightSaveValue = 0;
    uint16_t backlightNewValue = 0;
    uint16_t backlightValue = 0;
    bool isDimmed = false;

    //static void timer_callback(TimerHandle_t xTimer);
    //static void timer_callback(lv_timer_t * timer);    

  public:
    Backlight(void);
    void init(void);    

    void set_value(uint16_t value);
    void set_value_fast(uint16_t value);
    uint16_t get_value(void) { return backlightValue; }  
    uint16_t get_save_value(void) { return backlightSaveValue; }
    void save_value(void) { if(!isDimmed) { backlightSaveValue = backlightValue;} }

    void dim(void) {
      if(!isDimmed) {
        backlightSaveValue = backlightValue;
        if (backlightValue > 5) {
          set_value(5);
        }
        isDimmed = true;
      }
    };

    void restore_dim(void) {
      if(isDimmed) {
        isDimmed = false;
        set_value(backlightSaveValue);
      }
    }

    bool is_dimmed(void) { return isDimmed; };

    void backlight_timer(void);

};

#endif //BACKLIGHT_CONFIG_H