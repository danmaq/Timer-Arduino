#include <Prescaler.h>
#include "AmountType.h"
#include "InputDetector.h"
#include "DisplayDriver.h"

/*!
 * @brief Entry point.
 * @retval Exit code(0).
 */
int main(void)
{
  // Reserved.
  init();
#if defined(USBCON)
  USBDevice.attach();
#endif

  // Setup
  DisplayDriver::PortSetup();
  u32 elapsed;
  Prescaler ps(CLOCK_PRESCALER_256);
  {
    InputDetector input;
    CurrentData data;
  
    // Loop
    do
    {
      elapsed = ps.TrueMillis();
      if (input.CheckInput())
      {
        data.Reset(elapsed);
        DisplayDriver::StopMotor();
      }
      data.Reflesh(elapsed);
      DisplayDriver::ViewAmount(data.GetCurrentType());
      
      // Reserved.
      if (serialEventRun)
      {
        serialEventRun();
      }
    } while(!data.IsExpired());
    DisplayDriver::StartMotor();
  }
  
  ps.TrueDelay(2000);
//  delay(ps.RescaleDuration(1000));
  DisplayDriver::Reset();
  return 0;
}

