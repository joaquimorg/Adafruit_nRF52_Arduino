#include "backlight.h"
#include "Arduino.h"

/**
 * Constructor
 */
Backlight::Backlight(void) {

}


/*void Backlight::timer_callback(TimerHandle_t xTimer) {
    auto bl = static_cast<Backlight *>(pvTimerGetTimerID(xTimer));
    bl->backlight_timer();
}*/

static void timer_callback(lv_timer_t * timer) {
  auto bl = static_cast<Backlight *>(timer->user_data);
  bl->backlight_timer();
}

void Backlight::init(void){

   HwPWM0.addPin( LED_BUILTIN );
   HwPWM0.begin();
   HwPWM0.setMaxValue(128);
   HwPWM0.setClockDiv(PWM_PRESCALER_PRESCALER_DIV_16); // freq = 1Mhz

   HwPWM0.writePin(LED_BUILTIN, backlightValue, true);

   //lightTimer.begin(ms2tick(6), Backlight::timer_callback, this, pdTRUE);
   
}

void Backlight::backlight_timer(void) {
   
   if ( backlightValue > backlightNewValue ) {
      backlightValue--;
   } else if ( backlightValue < backlightNewValue ) {
      backlightValue++;
   }
   HwPWM0.writePin(LED_BUILTIN, backlightValue, true);

   if ( backlightNewValue == backlightValue ) {
      //lightTimer.stop();
      lv_timer_del(lightTimer);
      if ( backlightValue == 0 ) {
         HwPWM0.writePin(LED_BUILTIN, 0, true);
         digitalWrite(LED_BUILTIN, HIGH);
      }
   }
}

void Backlight::set_value(uint16_t value) {
   backlightNewValue = value;
   //HwPWM0.writePin(LED_BUILTIN, backlightValue, true);
   //lightTimer.start();
   lightTimer = lv_timer_create(timer_callback, 6, this);
   //lv_timer_set_repeat_count(lightTimer, 1);
   //lv_timer_ready(lightTimer);
   //isDimmed = false;
}

void Backlight::set_value_fast(uint16_t value) {
   backlightValue = value;
   HwPWM0.writePin(LED_BUILTIN, backlightValue, true);
   //lightTimer.start();
   //isDimmed = false;
}