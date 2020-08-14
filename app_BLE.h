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
*	Contains  macros and function declaration used in the app_BLE.c file 
********************************************************************************/
#if !defined(_APP_BLE_H)
#define _APP_BLE_H
#include <cytypes.h>

/*************************Pre-processor directives****************************/
    
/****************************************************************************/    
    
/***************************Macro Declarations*******************************/ 
    
/* define the test register to switch the PA/LNA hardware control pins */
#define CYREG_SRSS_TST_DDFT_CTRL 0x40030008
    
/* Read and write length of LPM LED data */
#define LPM_CHAR_DATA_LEN					1
    
/* Selector to denote where the LPM data is stored */
#define LPM_INDEX						0
    
/* Client Characteristic Configuration descriptor data length. This is defined
* as per BLE spec. */
#define CCCD_DATA_LEN						2    
    
/* Connection Update Parameter values to modify connection interval. These values
* are sent as part of CyBle_L2capLeConnectionParamUpdateRequest() which requests
* Client to update the existing Connection Interval to new value. Increasing 
* connection interval will reduce data rate but will also reduce power consumption.
* These numbers will influence power consumption */

/* Minimum connection interval = CONN_PARAM_UPDATE_MIN_CONN_INTERVAL * 1.25 ms*/
#define CONN_PARAM_UPDATE_MIN_CONN_INTERVAL	100        	
/* Maximum connection interval = CONN_PARAM_UPDATE_MAX_CONN_INTERVAL * 1.25 ms */
#define CONN_PARAM_UPDATE_MAX_CONN_INTERVAL	110        	
/* Slave latency = Number of connection events */
#define CONN_PARAM_UPDATE_SLAVE_LATENCY		0          
/* Supervision timeout = CONN_PARAM_UPDATE_SUPRV_TIMEOUT * 10*/
#define CONN_PARAM_UPDATE_SUPRV_TIMEOUT		200     
/****************************************************************************/

    
/**************************Function Declarations*****************************/
void bleStackEventHandler( uint32 eventCode, void *eventParam );
void UpdateConnectionParam(void);
void UpdateNotificationCCCD(void);
void SendDataOverLPMselNotification(uint8 *LPMselData, uint8 len);
/****************************************************************************/

    
/**************************Extern Variables***********************************/
extern uint8 deviceConnected;
extern uint8 restartAdvertisement;
extern uint8 updateLPMNotificationCCCDAttribute;
extern uint8 lpmselNotifications;
extern uint8 LPMselData;
/****************************************************************************/    
    
#endif
/* [] END OF FILE */
