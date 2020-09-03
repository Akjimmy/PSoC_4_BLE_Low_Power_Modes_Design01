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
#include "app_Main.h"

/**************************Variable Declarations*****************************/

/* Connection Parameter update values. This values are used by the BLE component
* to update the connector parameter, including connection interval, to desired 
* value */
static CYBLE_GAP_CONN_UPDATE_PARAM_T ConnectionParam =
{
    CONN_PARAM_UPDATE_MIN_CONN_INTERVAL,  		      
    CONN_PARAM_UPDATE_MAX_CONN_INTERVAL,		       
    CONN_PARAM_UPDATE_SLAVE_LATENCY,			    
    CONN_PARAM_UPDATE_SUPRV_TIMEOUT 			         	
};

/* This flag is used by application to know whether a Central 
* device has been connected. This is updated in BLE event callback 
* function*/
uint8 deviceConnected = FALSE;

/* 'restartAdvertisement' flag provided the present state of power mode in firmware */
uint8 restartAdvertisement = FALSE;

/* These flags are used to let application update the respective CCCD value of the 
* custom characteristics for correct read operation by connected Central device */
uint8 updateLPMNotificationCCCDAttribute = FALSE;

/*This flag is set when the Central device writes to CCCD of the 
* LPM LED Characteristic to enable notifications */
uint8 lpmselNotifications = FALSE;

/* Array to store the present LPM Sel control data. The 1 byte 
* of the array represents {LPM} */
uint8 LPMselData;


/* This flag is used to let application send a L2CAP connection update request
* to Central device */
static uint8 isConnectionUpdateRequested = TRUE;

/* Status flag for the Stack Busy state. This flag is used to notify the application 
* whether there is stack buffer free to push more data or not */
uint8 busyStatus = 0;

/****************************************************************************/

/*******************************************************************************
* Function Name: bleStackEventHandler
********************************************************************************
* Summary:
*        Call back event function to handle various events from BLE stack
*
* Parameters:
*  event:		event returned
*  eventParam:	link to value of the events returned
*
* Return:
*  void
*
*******************************************************************************/
void bleStackEventHandler( uint32 eventCode, void *eventParam )
{
    //CYBLE_GATT_ERR_CODE_T           errorCode;
    
    /* Local variable to store the data received as part of the Write request events */
	CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
    
    switch( eventCode )
    {
        /* Generic events */

        case CYBLE_EVT_STACK_ON:
            /* Configure the Link Layer to automatically switch PA
             * control pin P3[2] and LNA control pin P3[3] */
            CY_SET_XTND_REG32((void CYFAR *)(CYREG_BLE_BLESS_RF_CONFIG), 0x0331);
            CY_SET_XTND_REG32((void CYFAR *)(CYREG_SRSS_TST_DDFT_CTRL), 0x80000302);
            
            /* Set restartAdvertisement flag to allow calling Advertisement 
			* API from main function */
			restartAdvertisement = TRUE;
            
            break;
            
        //case CYBLE_EVT_TIMEOUT:
			/* Event Handling for Timeout  */
		//	break;            
        /**********************************************************
        *                       GAP Events
        ***********************************************************/
		case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
			
			// If the current BLE state is Disconnected, 
			// then the AdvertisementStart Stop event implies that advertisement has stopped.
            // If the current BLE state is CYBLE_STATE_ADVERTISING
			// then the Advertisement Start Stop event implies that Advertising in process.
            if(CyBle_GetState() == CYBLE_STATE_DISCONNECTED)
			{
				// Set restartAdvertisement flag to allow calling Advertisement 
				// API from main function ,then Sequential advertising could be started
				//restartAdvertisement = TRUE;
                 //#ifdef ENABLE_APP_WATCHDIG_INT
	                CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER2_INT);
                    CySysWdtEnable(CY_SYS_WDT_COUNTER2_MASK);
                 //#endif
			}
            
			break;    
        /* Add additional events as required */    
        case CYBLE_EVT_GAP_DEVICE_CONNECTED: 					
		    /* This event is received when device is connected over GAP layer */
            
		    break;
			
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
			/* This event is received when device is disconnected */
			
			/* Set restartAdvertisement flag to allow calling Advertisement 
			* API from main function */
			restartAdvertisement = TRUE;
            #ifdef ENABLE_APP_WATCHDIG_INT
	        CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
            CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
            #endif
			break;
		/**********************************************************
        *                       GATT Events
        ***********************************************************/
        case CYBLE_EVT_GATT_CONNECT_IND:
            /* This event is received when device is connected over GATT level */
			
			/* Update attribute handle on GATT Connection*/
            cyBle_connHandle = *(CYBLE_CONN_HANDLE_T  *)eventParam;
			
			/* This flag is used in application to check connection status */
			deviceConnected = TRUE;
           
			break; 
        case CYBLE_EVT_GATT_DISCONNECT_IND:
			/* This event is received when device is disconnected */
			
			/* Update deviceConnected flag*/
			deviceConnected = FALSE;
            
            /* Reset RGB notification flag to prevent further notifications
			 * being sent to Central device after next connection. */
			lpmselNotifications = FALSE;

             /* Reset the CCCD value to disable notifications */
			updateLPMNotificationCCCDAttribute = TRUE;
			UpdateNotificationCCCD();
            
            /* Reset the color coordinates */
			//LPMselData = ZERO;
            
            /* Reset the isConnectionUpdateRequested flag to allow sending
			* connection parameter update request in next connection */
			isConnectionUpdateRequested = TRUE;
            
            break;
            
        case CYBLE_EVT_GATTS_WRITE_REQ:
            /* This event is received when Central device sends a Write command on an Attribute */
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;
            if(CYBLE_CS_LPM_CCHARACT_LPM_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE == wrReqParam->handleValPair.attrHandle)
            {
				/* Extract the Write value sent by the Client for LPM CCCD */
                lpmselNotifications = wrReqParam->handleValPair.value.val[CYBLE_CS_LPM_CCHARACT_LPM_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_INDEX];
				
				/* Set flag to allow CCCD to be updated for next read operation by UpdateNotificationCCCD()in app_Main.c */
				updateLPMNotificationCCCDAttribute = TRUE;
				
				/* Update the LPM Notification attribute with new color coordinates */
				SendDataOverLPMselNotification(&LPMselData, LPM_CHAR_DATA_LEN);
            }
            
            /* Check if the returned handle is matching to LPM Control Write Attribute and extract the RGB data*/
            if(CYBLE_CS_LPM_CCHARACT_LPM_CHAR_HANDLE == wrReqParam->handleValPair.attrHandle)
            {
				/* Extract the Write value sent by the Client for RGB LED Color characteristic */
                LPMselData = wrReqParam->handleValPair.value.val[LPM_INDEX];           

                SendDataOverLPMselNotification(&LPMselData, LPM_CHAR_DATA_LEN);
            }
			
			/* Send the response to the write request received. */
			CyBle_GattsWriteRsp(cyBle_connHandle);
            break;
            
        case CYBLE_EVT_L2CAP_CONN_PARAM_UPDATE_RSP:
				/* If L2CAP connection parameter update response received, reset application flag */
            	isConnectionUpdateRequested = FALSE;
            break;
            
        case CYBLE_EVT_STACK_BUSY_STATUS:
			/* This event is generated when the internal stack buffer is full and no more
			* data can be accepted or the stack has buffer available and can accept data.
			* This event is used by application to prevent pushing lot of data to stack. */
			
			/* Extract the present stack status */
            busyStatus = * (uint8*)eventParam;
            break;    
        /* default catch-all case */
        default:
            break;    
    }
}    




/*******************************************************************************
* Function Name: UpdateConnectionParam
********************************************************************************
* Summary:
*        Send the Connection Update Request to Client device after connection 
* and modify the connection interval for low power operation.
*
* Parameters:
*	void
*
* Return:
*  void
*
*******************************************************************************/
void UpdateConnectionParam(void)
{
	/* If device is connected and Update connection parameter not updated yet,
	* then send the Connection Parameter Update request to GATT Client(GAP central). */
    if(deviceConnected && isConnectionUpdateRequested)
	{
		/* Reset the flag to indicate that connection Update request has been sent */
		isConnectionUpdateRequested = FALSE;
		
		/* Send Connection Update request with set Parameter */
		CyBle_L2capLeConnectionParamUpdateRequest(cyBle_connHandle.bdHandle, &ConnectionParam);

        
        #ifdef PRINT_MESSAGE_LOG 
            _EndTxFlag=0;// UART_1 ISR set _EndFlag to 1 when Tx Done
            UART_1_UartPutString("PERIPHERAL connected");
            // ### while(UART_1_SpiUartGetTxBufferSize());//still fail !!!loss character .transmit buffer=0,may be FIFI not empty,
            //while(!_EndTxFlag);//ok 
            
            // cypress suggested method
            // Wait until UART completes transfer data.
            //while ((UART_1_SpiUartGetTxBufferSize() + UART_1_GET_TX_FIFO_SR_VALID) != 0u); 
            // ### place these code in app_HandleLowPower.c
        #endif
       
    }
}
/*******************************************************************************
* Function Name: SendDataOverLPMselNotification
********************************************************************************
* Summary:
*        Send LPM SEL data as BLE Notifications. This function updates
* the notification handle with data and triggers the BLE component to send 
* notification
*
* Parameters:
*  LPMselData:	pointer to an array containing LPM sel value
*  len: length of the array
*
* Return:
*  void
*
*******************************************************************************/
void SendDataOverLPMselNotification(uint8 *LPMselData, uint8 len)
{
	/* 'rgbLednotificationHandle' stores RGB LED notification data parameters */
	CYBLE_GATTS_HANDLE_VALUE_NTF_T 	rgbLednotificationHandle;
	
	/* If stack is not busy, then send the notification */
	if(busyStatus == CYBLE_STACK_STATE_FREE)
	{
		/* Update notification handle will CapSense slider data*/
		rgbLednotificationHandle.attrHandle = CYBLE_CS_LPM_CCHARACT_LPM_CHAR_HANDLE;				
		rgbLednotificationHandle.value.val = LPMselData;
		rgbLednotificationHandle.value.len = len;
		
		/* Send the updated handle as part of attribute for notifications */
		CyBle_GattsNotification(cyBle_connHandle,&rgbLednotificationHandle);
	}
}

/*******************************************************************************
* Function Name: UpdateNotificationCCCD
********************************************************************************
* Summary:
*        Update the data handle for notification status and report it to BLE 
*	component so that it can be read by Central device.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void UpdateNotificationCCCD(void)
{
	/* Local variable to store the current CCCD value */
	uint8 RGBCCCDvalue[2];
	
	/* Handle value to update the CCCD */
	CYBLE_GATT_HANDLE_VALUE_PAIR_T RGBNotificationCCCDhandle;
	
	/* Update notification attribute only when there has been change in RGB LED CCCD*/
	if(updateLPMNotificationCCCDAttribute)
	{
		/* Reset the flag*/
		updateLPMNotificationCCCDAttribute = FALSE;
		
		/* Write the present RGB notification status to the local variable */
		RGBCCCDvalue[0] = lpmselNotifications;
		RGBCCCDvalue[1] = 0x00;
		
		/* Update CCCD handle with notification status data*/
		RGBNotificationCCCDhandle.attrHandle = CYBLE_CS_LPM_CCHARACT_LPM_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE;
		RGBNotificationCCCDhandle.value.val = RGBCCCDvalue;
		RGBNotificationCCCDhandle.value.len = CCCD_DATA_LEN;
		
		/* Report data to BLE component for sending data when read by Central device */
		CyBle_GattsWriteAttributeValue(&RGBNotificationCCCDhandle, ZERO, &cyBle_connHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
	}	
}


/* [] END OF FILE */
