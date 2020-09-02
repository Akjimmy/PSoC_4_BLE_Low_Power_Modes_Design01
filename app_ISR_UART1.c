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

CY_ISR(ISR_UART1)
{
    uint32 uChar;
    if(0u != (UART_1_GetRxInterruptSourceMasked() & UART_1_INTR_RX_NOT_EMPTY)) 
    { 
        while (UART_1_SpiUartGetRxBufferSize() != 0u) 
        {
             uChar= UART_1_UartGetChar();   
        }
        
        UART_1_ClearRxInterruptSource(UART_1_INTR_RX_NOT_EMPTY);
    }
    
    if(UART_1_GetTxInterruptSourceMasked() & UART_1_INTR_TX_UART_DONE)
    {
        _EndTxFlag=1;
        UART_1_ClearTxInterruptSource(UART_1_INTR_TX_UART_DONE);
    }
    
}

/* [] END OF FILE */
