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
/********************************************************************************
*	Contains  macros and function declaration used in the app_HandleLowPower.c file 
********************************************************************************/


#if !defined(_APP_HANDLELOWPOWER_H)
#define _APP_HANDLELOWPOWER_H
#include <cytypes.h>

    
/*************************Pre-processor directives****************************/
/* 'ENABLE_LOW_POWER_MODE' pre-processor directive enables the low power mode 
* handling in the firmware, ensuring low power consumption during project usage.
* To disable, comment the following #define. 
* If disabled, prevent usage of the project with coin cell */
#define ENABLE_LOW_POWER_MODE    
  
    
/**************************Function Declarations*****************************/
void HandleLowPowerMode(uint8 byLpm);
/****************************************************************************/

/**************************Macro definitions*****************************/
/* Power modes while maintaining BLE connections */
#define ACTIVE      (0u)
#define SLEEP       (1u)
#define DEEPSLEEP   (2u)
/****************************************************************************/
    
#endif
/* [] END OF FILE */
