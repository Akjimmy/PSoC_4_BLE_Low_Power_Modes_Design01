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
#include "app_ISR_UART1.h"
    
/*************************Pre-processor directives****************************/

    #define     PRINT_MESSAGE_LOG
     
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
