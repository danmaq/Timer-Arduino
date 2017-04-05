#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <Arduino.h>
#include <avr/io.h>
#include "Preference.h"
#include "AmountType.h"

namespace DisplayDriver
{
  inline void StartMotor(void)
  {
    PORTB |= MOTOR_1;
  }
  
  inline void StopMotor(void)
  {
    PORTB &= ~MOTOR_1;
  }
  
  inline void Reset(void)
  {
    PORTB ^= RESET_1;
  }

  inline void PortSetup(void)
  {
    DDRD &= ~BUTTON_1;
    DDRB |= INDICATOR_1 | INDICATOR_2 | RESET_1 | MOTOR_1;
    StopMotor();
    PORTB |= RESET_1;
  }
  
  inline void ViewAmount(AmountType mode)
  {
    uint8_t reg = PORTB | (INDICATOR_1 | INDICATOR_2);
    switch(mode)
    {
    case AMOUNT_MODE_DISABLE:
    case AMOUNT_MODE_01:
      reg ^= INDICATOR_1;
      break;
    default: break;
    }
    switch(mode)
    {
    case AMOUNT_MODE_DISABLE:
    case AMOUNT_MODE_10:
      reg ^= INDICATOR_2;
      break;
    default: break;
    }
    PORTB = reg;
  }
};

#endif  // DISPLAY_DRIVER_H

