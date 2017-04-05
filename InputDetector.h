#ifndef INPUT_DETECTOR_H
#define INPUT_DETECTOR_H

#include <Arduino.h>
#include <avr/io.h>
#include "Preference.h"

/** @brief Detect button input. */
class InputDetector
{
public:
  //! Constructor.
  InputDetector() : previous(LOW) { }

  inline uint8_t CheckInput(void)
  {
    const uint8_t now = PIND & BUTTON_1;
    const uint8_t result = (now != LOW && previous == LOW);
    previous = now;
    return result;
  }

private:
  uint8_t previous;
  
};

#endif	// INPUT_DETECTOR_H
