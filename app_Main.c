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
#include <project.h>
#include "app_Main.h"
 
//######################################################
//#define _TEST_MAIN_   // uncomment to use test main()
//######################################################

#ifdef _TEST_MAIN_
int main_test(void)
{
    CYBLE_LP_MODE_T sleepMode;
	CYBLE_BLESS_STATE_T blessState;
    cyWdtCallback   aa;
    
    //*****************************************
    CyGlobalIntEnable; 

    PinLedBlue_SetDriveMode(PinLedBlue_DM_STRONG);
    PinLedBlue_Write(LED_OFF);
    
    PinLedRed_SetDriveMode(PinLedRed_DM_STRONG);
    PinLedRed_Write(LED_OFF);
   //*****************************************
    //CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
    //CySysWdtDisable(CY_SYS_WDT_COUNTER0_MASK);// Disable the WDT0
    //*****************************************
    //do
    //{
        aa=CySysWdtSetInterruptCallback(CY_SYS_WDT_COUNTER0,wdt0InterruptCallback);
        //CyDelayUs(WATCHDOG_REG_UPDATE_WAIT_TIME);
        
    //}while(aa==0);
    
    CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER2_INT);
    CySysWdtDisable(CY_SYS_WDT_COUNTER2_MASK);// Disable the WDT2
    
    CyGlobalIntDisable;
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    //CyBle_Start( bleStackEventHandler );// start BLESS
    
    isr_SW2_StartEx(SW2_ISR);//* Start the Button ISR to allow wakeup from low power mode sleep
    
        
    for(;;)
    {
        /* Place your application code here */
        //CyBle_ProcessEvents();
        //if(deviceConnected == TRUE )
		//{
			/* After the connection, send new connection parameter to the Client device 
			* to run the BLE communication on desired interval. This affects the data rate 
			* and power consumption. High connection interval will have lower data rate but 
			* lower power consumption. Low connection interval will have higher data rate at
			* expense of higher power. This function is called only once per connection. */
			//UpdateConnectionParam();
			
			/* When the Client Characteristic Configuration descriptor (CCCD) is written
			* by Central device for enabling/disabling notifications, then the same
			* descriptor value has to be explicitly updated in application so that
			* it reflects the correct value when the descriptor is read */
			//UpdateNotificationCCCD();
		//}
        
        /* Leave chip in Deep Sleep mode */
    	/* Put BLESS into Deep Sleep and check the return status */
    	//sleepMode = CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
        
        CyGlobalIntDisable;
        /* Check the Status of BLESS */
    	//blessState = CyBle_GetBleSsState();
        //if(blessState == CYBLE_BLESS_STATE_ECO_ON || blessState == CYBLE_BLESS_STATE_DEEPSLEEP)
        //{
            CySysPmDeepSleep(); // Put the chip into deep sleep. 
        //}
        CyGlobalIntEnable;
        
    }
}

#else
//*****************************************
int main(void)
{
    //*****************************************
    // Initialize Low power mode selecttion
    uint8 byAppPM;// application power mode
    //byAppPM = ACTIVE;
    //byAppPM = SLEEP;
    byAppPM = DEEPSLEEP;// default 
    
    //*****************************************
    // Initialize pin state
    PinLedRed_Write(LED_OFF);
    PinLedGreen_Write(LED_OFF);
    PinLedBlue_Write(LED_OFF);
    
    PinAppState_Write(1);
    
	PinLedRed_SetDriveMode(PinLedRed_DM_ALG_HIZ);
	PinLedGreen_SetDriveMode(PinLedGreen_DM_ALG_HIZ);
	PinLedBlue_SetDriveMode(PinLedBlue_DM_ALG_HIZ);
       
    //###################################
    PinLedBlue_SetDriveMode(PinLedBlue_DM_STRONG);
    PinLedBlue_Write(LED_ON);
    CyDelay(500);
    PinLedBlue_Write(LED_OFF);
    
    PinLedRed_SetDriveMode(PinLedRed_DM_STRONG);
    PinLedRed_Write(LED_ON);
    CyDelay(500);
    PinLedRed_Write(LED_OFF);
    //###################################
    
    //************************************************************************
    // Initialize Watchdog
    #ifdef ENABLE_APP_WATCHDIG_INT  //#define in main.h
    // WDT0,WDT2 configurate and enable  on the clocks tab of the design wide resources (DWR)
    // WDT0 : 1sec for wakeup system then disabled
    // WDT2 : enable after ADV STOP or disconnected. 30sec timeout to wakeup form deepsleep
        
    //uint32 uiWdtMode;
    //uint32 uiWdtClearOnMatchStatus;
    //uint32 uiWdt0MatchValue; 
    uint32 uiWDTcount;
    //uint8  byIntPriority; 
    uint32  uiWdt2ToggleBit;  
        
    /* Unlock the WDT registers for modification */
	CySysWdtUnlock(); 
        CySysWdtDisable(CY_SYS_WDT_COUNTER0_MASK);// Disable the WDT0
        CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
    
        //uiWdtMode=CySysWdtGetMode(CY_SYS_WDT_COUNTER0);
        //uiWdtClearOnMatchStatus=CySysWdtGetClearOnMatch(CY_SYS_WDT_COUNTER0);
        //CySysWdtSetClearOnMatch(CY_SYS_WDT_COUNTER0,0);
        //uiWdtClearOnMatchStatus=CySysWdtGetClearOnMatch(CY_SYS_WDT_COUNTER0);
        
        //uiWdt0MatchValue=CySysWdtGetMatch(CY_SYS_WDT_COUNTER0);
        
        /* Reset Counter 0 and give ~3 LFCLK cycles to take effect */
        //uiWDTcount=CySysWdtGetCount (CY_SYS_WDT_COUNTER0);
		CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET);
		CyDelayUs(WATCHDOG_REG_UPDATE_WAIT_TIME);
        //uiWDTcount=CySysWdtGetCount (CY_SYS_WDT_COUNTER0);
        //----------------------------------------------------------------
    
        CySysWdtDisable(CY_SYS_WDT_COUNTER2_MASK);// Disable the WDT2
        CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER2_INT);
        
        /* Reset Counter 2 and give ~3 LFCLK cycles to take effect */
        //uiWDTcount=CySysWdtGetCount (CY_SYS_WDT_COUNTER2);
		CySysWdtResetCounters(CY_SYS_WDT_COUNTER2_RESET);
		CyDelayUs(WATCHDOG_REG_UPDATE_WAIT_TIME);
        //uiWDTcount=CySysWdtGetCount (CY_SYS_WDT_COUNTER2);
        
        //uiWdt2ToggleBit=CySysWdtGetToggleBit();
        // Get Watchdog interrupt priority
        //byIntPriority=CyIntGetPriority(WATCHDOG_INT_VEC_NUM);
        
        // Set Watchdog interrupt to Highest priority
	    //CyIntSetPriority(WATCHDOG_INT_VEC_NUM, 0);
    
        /* Set the Watchdog Interrupt vector to the address of Interrupt routine WDT_INT_Handler. */
	    cyWdtCallback   aa;
       // CySysWdtDisableCounterIsr(CY_SYS_WDT_COUNTER0);
       //     do{
                aa=CySysWdtSetInterruptCallback(CY_SYS_WDT_COUNTER0,wdt0InterruptCallback);
       //     }while(aa==0);
       // CySysWdtEnableCounterIsr(CY_SYS_WDT_COUNTER0);
    
       // CySysWdtDisableCounterIsr(CY_SYS_WDT_COUNTER2);
       //     do{
                aa=CySysWdtSetInterruptCallback(CY_SYS_WDT_COUNTER2,wdt2InterruptCallback);
       //     }while(aa==0);
       // CySysWdtEnableCounterIsr(CY_SYS_WDT_COUNTER2);
    
    CySysWdtUnlock();
    
    /* Enable Watchdog Counter 0 */
	//CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
    //CySysWdtEnable(CY_SYS_WDT_COUNTER2_MASK);
    #endif
    
    //************************************************************************
    
    
    //************************************************************************
    // Enable global interrupts //CyGlobalIntDisable;// Power ON default
    
    CyGlobalIntEnable;
        
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    //CyBle_Start( bleStackEventHandler );// start BLESS
  
	isr_SW2_StartEx(SW2_ISR);//* Start the Button ISR to allow wakeup from low power mode sleep
    
    for(;;)
    {
        CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
        //CySysWdtEnable(CY_SYS_WDT_COUNTER2_MASK);
        
        PinAppState_Write(0);//### Jimmy. Obseve the deepsleep state
        
        CyGlobalIntDisable;
            CySysPmDeepSleep(); // Put the chip into deep sleep.
        CyGlobalIntEnable;// allow pendding interrupt to run ISR
            
        PinAppState_Write(1);//### Jimmy
        
        CyDelayUs(WATCHDOG_REG_UPDATE_WAIT_TIME);
    }
    
    //************************************************************************    
    for(;;)
    {
        
        /* Place your application code here */
        CyBle_ProcessEvents();
        
        if(deviceConnected == TRUE )
		{
			/* After the connection, send new connection parameter to the Client device 
			* to run the BLE communication on desired interval. This affects the data rate 
			* and power consumption. High connection interval will have lower data rate but 
			* lower power consumption. Low connection interval will have higher data rate at
			* expense of higher power. This function is called only once per connection. */
			UpdateConnectionParam();
			
			/* When the Client Characteristic Configuration descriptor (CCCD) is written
			* by Central device for enabling/disabling notifications, then the same
			* descriptor value has to be explicitly updated in application so that
			* it reflects the correct value when the descriptor is read */
			UpdateNotificationCCCD();
		}
        
        #ifdef ENABLE_LOW_POWER_MODE
			/* Put system to Deep sleep, including BLESS, and wakeup on interrupt. 
			* The source of the interrupt can be either BLESS Link Layer in case of 
			* BLE advertisement and connection or by User Button press during BLE 
			* disconnection */
            
			HandleLowPowerMode(byAppPM);
            
		#endif
        
        if(restartAdvertisement)
		{
			// Reset 'restartAdvertisement' flag
			restartAdvertisement = FALSE;

			// Start Advertisement and enter Discoverable mode
			CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);	
		}
        
    }
}
#endif            
/* [] END OF FILE */
