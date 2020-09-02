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

/*******************************************************************************
* Function Name: SW2_ISR
********************************************************************************
* Summary:
*        ISR routine for isr_button. System enters here after CPU wakeup.
* Clears pending interrupt.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/

#include "app_Main.h"

CY_ISR(SW2_ISR)
{
	/* Clear Pending interrupts */
	isr_SW2_ClearPending();
	
	SW2_ClearInterrupt();
    
    //PinLedBlue_SetDriveMode(PinLedBlue_DM_STRONG);
    //PinLedBlue_Write(~PinLedBlue_Read());
    
    
    //PinAppState_Write(~PinAppState_Read());
}

CY_ISR(Pin0_5_ISR)
{
    isr_Pin0_5_ClearPending();
    Pin0_5_ClearInterrupt();
    if(Pin2_0_Read()==0)
        Pin2_0_Write(1);
    else 
        Pin2_0_Write(0);
    //Pin2_0_Write(~Pin2_0_Read());
}
/* [] END OF FILE */
