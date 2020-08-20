/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#if !defined(_APP_WATCHDOG_H)
#define _APP_WATCHDOG_H
#include <cytypes.h>
    
/* The 16 bit Counter 0 in Watchdog is used for counting time to keep
* the Status LED on for pre-detremined time. The count value is set for 
* 1 second and 3 such interrupts are used to select 3 seconds of ON time */
#define WATCHDOG_ONE_SEC_COUNT_VAL			32768

/* Watchdog Interrupt Vector number in PSoC 4 BLE. See PSoC 4 BLE TRM for
* details */ 
#define WATCHDOG_INT_VEC_NUM				8
											
/* Desired Watchdog Interrupt priority */ 
#define WATCHDOG_INT_VEC_PRIORITY			3

/* After Resetting or Writing the Counter 0 register with match value, it takes
* ~3 LFCLK cycles to take effect. Provide a delay of 100 us after above actions
* for changes to take effect */
#define WATCHDOG_REG_UPDATE_WAIT_TIME		100     //3 clock of 32768Hz is 91.552uS
    
// void WDT_INT_Handler(void);
// void InitializeWatchdog(uint32 counterNum);

void wdt0InterruptCallback(void);
void wdt2InterruptCallback(void);


// 'ENABLE_APP_WATCHDIG_INT' pre-processor directive enables the watchdog to raise an interrupt    
//To disable, comment the following #define.
#define ENABLE_APP_WATCHDIG_INT  //
#define _ENABLE_WATCHDOG_IN_ISR_
#define __TEST_WATCHDOG__
    
#endif
/* [] END OF FILE */
