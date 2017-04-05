#ifndef AMOUNT_TYPE_H
#define AMOUNT_TYPE_H

#include <Arduino.h>
#include "Preference.h"

// DO NOT SET OVER 65535 SEC.

/*!
 * @brief Amout time table.
 */
enum AmountType
{
  AMOUNT_MODE_DISABLE = 0,//!< -2.4% sec.
  AMOUNT_MODE_01 = 3,     //!< 32
  AMOUNT_MODE_10 = 4,     //!< 512
  AMOUNT_MODE_11 = 14,    //!< 16384
};

inline AmountType NextAmountMode(AmountType mode)
{
  AmountType next = AMOUNT_MODE_DISABLE;
  switch(mode)
  {
  case AMOUNT_MODE_DISABLE: next = AMOUNT_MODE_01;      break;
  case AMOUNT_MODE_01:      next = AMOUNT_MODE_10;      break;
  case AMOUNT_MODE_10:      next = AMOUNT_MODE_11;      break;
  case AMOUNT_MODE_11:      next = AMOUNT_MODE_DISABLE; break;
  default: break;
  }
  return next;
}

inline AmountType PreviousAmountMode(AmountType mode)
{
  AmountType prev = AMOUNT_MODE_DISABLE;
  switch(mode)
  {
  case AMOUNT_MODE_DISABLE: prev = AMOUNT_MODE_DISABLE; break;
  case AMOUNT_MODE_01:      prev = AMOUNT_MODE_DISABLE; break;
  case AMOUNT_MODE_10:      prev = AMOUNT_MODE_01;      break;
  case AMOUNT_MODE_11:      prev = AMOUNT_MODE_10;      break;
  default: break;
  }
  return prev;
}

class CurrentData
{
public:

  //! Constructor.
  CurrentData(void):
    expired(false),
    lastResetTime(0),
    currentType(AMOUNT_MODE_DISABLE)
  {
  }
  
  inline void Reset(u32 totalElapsed)
  {
    expired = false;
    currentType = NextAmountMode(GetCurrentType());
    lastResetTime = totalElapsed;
  }

  inline void Reflesh(u32 totalElapsed)
  {
    const AmountType current = GetCurrentType();
    if (current != AMOUNT_MODE_DISABLE)
    {
      const AmountType lower = PreviousAmountMode(current);
      if (
        (_BV(static_cast<uint16_t>(current)) - _BV(static_cast<uint16_t>(lower)))
        < ((totalElapsed - lastResetTime) >> 10))
      {
        expired = ((currentType = lower) == AMOUNT_MODE_DISABLE);
        lastResetTime = totalElapsed;
      }
    }
  }

  inline AmountType GetCurrentType(void) const { return currentType; }
  
  inline uint8_t IsExpired(void) const { return expired; }
  
private:
  uint8_t expired;
  u32 lastResetTime;
  AmountType currentType;
};

#endif	// AMOUNT_TYPE_H
