

#include "variant.h"
#include "wiring_constants.h"
#include "wiring_digital.h"
#include "nrf.h"

#define _PINNUM(port, pin)     ((port)*32 + (pin))

const uint32_t g_ADigitalPinMap[] =
{
  _PINNUM(0, 25), // D0 - TX
  _PINNUM(0, 14), // D1 - RX
  _PINNUM(1, 4 ), // D2 - LCD BL
  _PINNUM(1, 4 ), // D3
  _PINNUM(1, 11), // D4
  _PINNUM(1, 12), // D5
  _PINNUM(1, 14), // D6
  _PINNUM(0, 26), // D7
  _PINNUM(0, 27), // D8
  _PINNUM(0, 12), // D9
  _PINNUM(0, 6 ), // D10
  _PINNUM(0, 8 ), // D11
  _PINNUM(1, 9 ), // D12
  _PINNUM(0, 14), // D13

  _PINNUM(0, 4 ), // D14
  _PINNUM(0, 5 ), // D15
  _PINNUM(0, 28), // D16
  _PINNUM(0, 30), // D17
  _PINNUM(0, 2 ), // D18
  _PINNUM(0, 3 ), // D19
  _PINNUM(0, 29), // D20
  _PINNUM(0, 31), // D21

  _PINNUM(0, 15), // D22
  _PINNUM(0, 16), // D23

  _PINNUM(0, 11), // D24
  _PINNUM(1, 8 ), // D25
  _PINNUM(0, 7 ), // D26

  _PINNUM(0, 19), // D27
  _PINNUM(0, 20), // D28
  _PINNUM(0, 17), // D29
  _PINNUM(0, 23), // D30
  _PINNUM(0, 22), // D31
  _PINNUM(0, 21), // D32

  _PINNUM(1, 13), // D33
  _PINNUM(1, 15), // D34
  _PINNUM(0, 13), // D35
  _PINNUM(1, 2 ), // D36
  _PINNUM(1, 0 ), // D37
  _PINNUM(0, 9 ), // D38
  _PINNUM(0, 10), // D39

  _PINNUM(1, 13), // D40
  _PINNUM(1, 15), // D41
  _PINNUM(0, 13), // D42
  _PINNUM(1, 2 ), // D43
  _PINNUM(1, 0 ), // D44
  _PINNUM(0, 9 ), // D45
  _PINNUM(0, 10), // D46
  _PINNUM(0, 10)  // D47
    
};



void initVariant()
{

}

