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
*	Contains  macros and function declaration used in the app_Main.c file 
********************************************************************************/
#if !defined(_APP_MAIN_H)
#define _APP_MAIN_H
    
#include <project.h>

#include "app_BLE.h"
#include "app_ISR.h"
#include "app_HandleLowPower.h"
#include "app_WatchDog.h"    
    
/*************************Pre-processor directives****************************/
    
// 'ENABLE_APP_WATCHDIG_INT' pre-processor directive enables the watchdog to raise an interrupt    
//To disable, comment the following #define.
#define    ENABLE_APP_WATCHDIG_INT  //
     
/****************************************************************************/      
   
    
/***************************Macro Declarations*******************************/
/* Drive value for LED OFF. Note that LED on BLE Pioneer kit is
* active low */
#define LED_OFF						1
#define LED_ON						0	


/* General Macros */
#define TRUE							1
#define FALSE							0
#define ZERO							0  
/****************************************************************************/    
    
    
/**************************Function Declarations*****************************/

/****************************************************************************/ 
    
    
/**************************Extern Variables***********************************/
extern uint8 byAppPM;// application power mode    
/****************************************************************************/    
#endif

/* [] END OF FILE */
