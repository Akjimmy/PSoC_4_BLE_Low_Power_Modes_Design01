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
* Function Name: WDT_INT_Handler
********************************************************************************
* Summary:
*        Watchdog interrupt routine for controlling LED status in connected state
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void WDT_INT_Handler(void)
{
    //-------------------------------------
    /* If the Interrupt source is Counter 0 match, then process */
	if(CySysWdtGetInterruptSource() & CY_SYS_WDT_COUNTER0_INT)
	{
        /* Clear Watchdog Interrupt from Counter 0 */
		CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
        
        //******************************************************
        // place application code here
        
        //******************************************************
        
        /* Unlock the WDT registers for modification */
		CySysWdtUnlock();
		
		/* Disable Counter 0 to allow modifications */
		CySysWdtDisable(CY_SYS_WDT_COUNTER0_MASK);
		
		/* Reset Counter 0 and give ~3 LFCLK cycles to take effect */
		CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET);
		CyDelayUs(WATCHDOG_REG_UPDATE_WAIT_TIME);
        
		
		/* Write the Counter 0 match value for 1 second and give ~3 LFCLK
		* cycles to take effect */
		CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, WATCHDOG_ONE_SEC_COUNT_VAL);
		CyDelayUs(WATCHDOG_REG_UPDATE_WAIT_TIME);
		
		/* Enable Watchdog Counter 0 */
		CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
		
		/* Lock Watchdog to prevent any further change */
	    CySysWdtLock();    
    }
}
/*******************************************************************************
* Function Name: InitializeWatchdog
********************************************************************************
* Summary:
*        Reset watchdog timer
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
/* Initialize the Watchdog0 for 1 second timing events. 
* The watcdog is initialized after ???? and disabled after ????.
* 
* This is done to ensure that Watchdog provided periodic wakeup only
* when ???. 
*  During other times, it should not wakeup the system
*/
/* Disable Watchdog to prevent Watchdog waking system */
// CyIntDisable(WATCHDOG_INT_VEC_NUM);

void InitializeWatchdog(uint32 counterNum)
{
	/* Unlock the WDT registers for modification */
	CySysWdtUnlock(); 
	    if(counterNum == CY_SYS_WDT_COUNTER0)
        {
	        /* Write Mode for Counter 0 as Interrupt on Match */
            CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_INT);
	
	        /* Set Clear on Match for Counter 0*/
	        CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER0, TRUE);
        
            /* Write the match value equal to 1 second in Counter 0 */
	        CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, WATCHDOG_ONE_SEC_COUNT_VAL);         
            
	        /* Enable Counter 0 */
            CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
            
            
        }
	    /* Set Watchdog interrupt to lower priority */
	    CyIntSetPriority(WATCHDOG_INT_VEC_NUM, WATCHDOG_INT_VEC_PRIORITY);
        
        /* Set the Watchdog Interrupt vector to the address of Interrupt routine 
	    * WDT_INT_Handler. This routine counts the 3 seconds for LED ON state during
	    * connection. */
	    CyIntSetVector(WATCHDOG_INT_VEC_NUM, &WDT_INT_Handler);
	
	    /* Enable Watchdog Interrupt using Interrupt number */
        CyIntEnable(WATCHDOG_INT_VEC_NUM);
	
	/* Lock Watchdog to prevent further changes */
    CySysWdtLock();
}

/*******************************************************************************
* Function Name: wdt0InterruptCallback
********************************************************************************
* Summary:
*        watchdog timer0 ISR
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void wdt0InterruptCallback(void)
{
    //#############################
    //uint32  uiWDTcount;
    //#############################
    //-------------------------------------
    /* If the Interrupt source is Counter 0 match, then process */
    if(CySysWdtGetInterruptSource() & CY_SYS_WDT_COUNTER0_INT)
	{
        //-------------------------------------
        //Clear Watchdog Interrupt from Counter 0
        CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);//cleared by CySysWdtIsr() in CyLFClk.c
        //-------------------------------------
        
        //#############################
        // if CySysWdtSetClearOnMatch() is set true,WDTCount reset to zero when countervalue match
        //uiWDTcount=CySysWdtGetCount (CY_SYS_WDT_COUNTER0);
        //#############################
        
        //******************************************************
        // place application code here
        
        //#############################
        PinLedRed_SetDriveMode(PinLedRed_DM_STRONG);
        PinLedRed_Write(~PinLedRed_Read());     
        //#############################
        
        //******************************************************
        
        //******************************************************
        /* Unlock the WDT registers for modification */
		//CySysWdtUnlock();
    	/* Disable Counter 0 to allow modifications */
    	CySysWdtDisable(CY_SYS_WDT_COUNTER0_MASK);
            
    		// if CySysWdtSetClearOnMatch() is set true,WDTCount reset to zero when countervalue match
            /* Reset Counter 0 and give ~3 LFCLK cycles to take effect */
    		//CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET);
    		//CyDelayUs(WATCHDOG_REG_UPDATE_WAIT_TIME);
            //#############################
    		//uiWDTcount=CySysWdtGetCount (CY_SYS_WDT_COUNTER0);
            //#############################
            
    		/* Write the Counter 0 match value for 1 second and give ~3 LFCLK
    		* cycles to take effect */
    		//CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, WATCHDOG_ONE_SEC_COUNT_VAL);//CySysWdtSetMatch(CY_SYS_WDT_COUNTER0, WATCHDOG_ONE_SEC_COUNT_VAL);
    		//CyDelayUs(WATCHDOG_REG_UPDATE_WAIT_TIME);
          
    	/* Enable Watchdog Counter 0 */
    	//CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
            
		/* Lock Watchdog to prevent any further change */
	    //CySysWdtLock(); 
        //******************************************************     
    }
}
void wdt2InterruptCallback(void)
{
    //#############################
    //uint32 wdtToggbit;
    //uint32  uiWDTcount;
    //#############################
    //-------------------------------------
    /* If the Interrupt source is Counter 2 match, then process */
	if(CySysWdtGetInterruptSource() & CY_SYS_WDT_COUNTER2_INT)
	{
        //CySysWdtDisable(CY_SYS_WDT_COUNTER2_MASK);
        /* Clear Watchdog Interrupt from Counter 2 */
		CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER2_INT);//cleared by CySysWdtIsr() in CyLFClk.c
        
        //#############################
        //uiWDTcount=CySysWdtGetCount (CY_SYS_WDT_COUNTER2);
       // wdtToggbit=CySysWdtGetToggleBit();
        //#############################
        
        //******************************************************
        // place application code here
        
        //#############################  
        PinLedBlue_SetDriveMode(PinLedBlue_DM_STRONG);
        PinLedBlue_Write(~PinLedBlue_Read());
        
        //PinAppState_Write(~PinAppState_Read());
       
        //#############################
        
        //******************************************************
        
        //******************************************************
        
        // Unlock the WDT registers for modification
		//CySysWdtUnlock();
		
		// Disable Counter 2 to allow modifications
		CySysWdtDisable(CY_SYS_WDT_COUNTER2_MASK);
        
		    //#############################
            //wdtToggbit=CySysWdtGetToggleBit();
            //#############################
        
            //******************************************************
            // toggle bit 0->1 rasing an INT,toggle bit 1->0 rasing an INT again
            // reset WDT2 counter to avoid this.
		    // Reset Counter 2 and give ~3 LFCLK cycles to take effect                     
		    CySysWdtResetCounters(CY_SYS_WDT_COUNTER2_RESET);
		    CyDelayUs(WATCHDOG_REG_UPDATE_WAIT_TIME);
            //******************************************************
            
            //#############################
            //CySysWdtSetToggleBit(wdtToggbit);
            //CyDelayUs(WATCHDOG_REG_UPDATE_WAIT_TIME);
            //#############################
	
		// Enable Watchdog Counter 2
    	//CySysWdtEnable(CY_SYS_WDT_COUNTER2_MASK);
        
        // Lock Watchdog to prevent any further change
	    //CySysWdtLock();    
        
        //******************************************************
    }
}

/* [] END OF FILE */
