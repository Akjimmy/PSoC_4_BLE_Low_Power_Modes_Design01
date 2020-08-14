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

/*******************************************************************************
* Function Name: HandleLowPowerMode
********************************************************************************
* Summary:
*        This function puts the BLESS in deep sleep mode and CPU to sleep mode. 
* System will resume from here when it wakes from user button press.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void HandleLowPowerMode(uint8 byLpm)
{
	#ifdef ENABLE_LOW_POWER_MODE
		/* Local variable to store the status of BLESS Hardware block */
		CYBLE_LP_MODE_T sleepMode;
		CYBLE_BLESS_STATE_T blessState;
        //uint8 interruptStatus;
        
        if (byLpm == DEEPSLEEP)
        {
            /* Leave chip in Deep Sleep mode */
    		/* Put BLESS into Deep Sleep and check the return status */
    		sleepMode = CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
    		
    		/* Disable global interrupt to prevent changes from any other interrupt ISR */
    		CyGlobalIntDisable;
    	    //interruptStatus=CyEnterCriticalSection();
            
    		/* Check the Status of BLESS */
    		blessState = CyBle_GetBleSsState();

    		if(sleepMode == CYBLE_BLESS_DEEPSLEEP)
    		{
    		    /* If the ECO has started or the BLESS can go to Deep Sleep, then place CPU 
    			* to Deep Sleep */
    			if(blessState == CYBLE_BLESS_STATE_ECO_ON || blessState == CYBLE_BLESS_STATE_DEEPSLEEP)
    		    {
    				/* Place CPU to Deep sleep */
                    #ifdef ENABLE_LED_NOTIFICATION
                    GREEN_SetDriveMode(GREEN_DM_STRONG);
                    GREEN_Write(0);
                    #endif
                    //************************************************************************
                    // Enable watchdog before deelpsleep would fail to depsleep
                    //#ifdef ENABLE_APP_WATCHDIG_INT
	                //InitializeWatchdog(CY_SYS_WDT_COUNTER0);
                    //CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER2_INT);
                    //CySysWdtEnable(CY_SYS_WDT_COUNTER2_MASK);
                    //#endif
                    
                    //************************************************************************                  
                    PinAppState_Write(0);//### Jimmy. Obseve the deepsleep state
                    
    		        CySysPmDeepSleep();
                    
                    PinAppState_Write(1);//### Jimmy
                    //************************************************************************
                    
                    //************************************************************************
                    /* Disable Watchdog to prevent Watchdog waking system */
                    #ifdef ENABLE_APP_WATCHDIG_INT
                        CyIntDisable(WATCHDOG_INT_VEC_NUM);
                        if(CySysWdtGetEnabledStatus(CY_SYS_WDT_COUNTER2))
                        {
                            //CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER2_INT);
                            CySysWdtDisable(CY_SYS_WDT_COUNTER2_MASK);
                        }
                    
                        if(CySysWdtGetEnabledStatus(CY_SYS_WDT_COUNTER0))
                        {
                            CySysWdtDisable(CY_SYS_WDT_COUNTER0_MASK);    
                        }
                    #endif
                    //************************************************************************
                    
                    //************************************************************************ 
                    
                    #ifdef ENABLE_LED_NOTIFICATION
                    GREEN_SetDriveMode(GREEN_DM_STRONG);
                    GREEN_Write(1);
                    #endif
                    //************************************************************************
                    
    		 	}
    		}
    		
    		/* Re-enable global interrupt mask after wakeup */
    		CyGlobalIntEnable;
            //CyExitCriticalSection(interruptStatus);
        }
        
        else if (byLpm == SLEEP)
        {
            /* Leave chip in Sleep mode */
            /* Leave chip in Deep Sleep mode */
    		/* Put BLESS into Deep Sleep and check the return status */
    		sleepMode = CyBle_EnterLPM(CYBLE_BLESS_SLEEP);
    		
    		/* Disable global interrupt to prevent changes from any other interrupt ISR */
    		CyGlobalIntDisable;
    	
    		/* Check the Status of BLESS */
    		blessState = CyBle_GetBleSsState();

    		if(sleepMode == CYBLE_BLESS_SLEEP)
    		{
    		    if(blessState != CYBLE_BLESS_STATE_EVENT_CLOSE)
    		    {
    				/* If BLE Event has not 
    				* closed yet, then place CPU to Sleep */
                    #ifdef ENABLE_LED_NOTIFICATION                   
                    BLUE_Write(0);
                    BLUE_SetDriveMode(BLUE_DM_STRONG);
                    #endif
                    
                    //PinAppState_Write(0);//### Jimmy. Obseve the deepsleep state
                    
    		        CySysPmSleep();
                    
                    //PinAppState_Write(1);//### Jimmy
                    
                    #ifdef ENABLE_LED_NOTIFICATION
                    BLUE_SetDriveMode(BLUE_DM_STRONG);
                    BLUE_Write(1);
                    #endif
    		    }
    		}
    		
    		/* Re-enable global interrupt mask after wakeup */
    		CyGlobalIntEnable;        
        }
        else if (byLpm == ACTIVE)
        {
            /* Leave chip in Active mode */
            #ifdef ENABLE_LED_NOTIFICATION
            RED_Write(0);
            RED_SetDriveMode(RED_DM_STRONG);
            #endif
        }
	#endif
}



/* [] END OF FILE */
