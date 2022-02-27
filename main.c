/*H**********************************************************************
* FILENAME :        main.c                    
*
* DESCRIPTION :
*       Entrypoint of the program 'uC Terminal Interface'. 
*       Contains the endless main loop, the statemachine and 
*       the interrupt service routine. The Software receives 
*       commands with parameters on the UART interface. It answers 
*       with an acknowledge if the command is valid or it sends
*       a Syntax Error message.
*       UART settings: 9600 Baud, 8 Bits, no Parity, 1 Stopbit, no FC  
*       
* WRITTEN FOR HARDWARE:
*       Microchip PCB Curiosity LPC with PIC16F18446
* 
* PIN TABLE:
*      PIN          Function          
*      PB5:         UART RC
*      PB7:         UART TX
* 
* NOTES :
*       This file is part of the project 'uC Terminal Interface'
*
*       Copyright: David Kress 2022.  All rights reserved.
*
* AUTHOR :    David Kress        CREATION DATE :    2022-02-17
*
* CHANGES :
*
* VERSION DATE       WHO      DETAIL
* 1.0     2022-02-17 DKr      Initial version
*
*H*/

/*--- Libraries --------------------------------------------------------------*/
#include <xc.h>
#include <string.h>
#include <stdbool.h>

/*--- Project headers-------------------------------------------------------- */
#include "configBits.h"
#include "global.h"
#include "init.h"
#include "uart.h"
#include "parser.h"

/*--- Type definitions ------------------------------------------------------ */
typedef enum
{
    init, 
    idle,
    receiveMessage
    
}statemachine_t;


/*--- Definition of global variables, also accessed by other files -----------*/
messageBuffer_t UARTOutputMessageBuffer;
volatile ringBuffer_t RXBuffer;
volatile ringBuffer_t TXBuffer;

/*--- Definition of varialbes only accessed by this file and ISR -------------*/
static volatile unsigned long counter = 0;


/*--- Main Routine -----------------------------------------------------------*/
void main(void) {
    
    statemachine_t stateMachine = init;
    statemachine_t nextState = init;
 
    uartCommandParameters_t receivedCommand;
    
    messageBuffer_t UARTInputMessageBuffer;
    
    while(1)
    {

        counter++;
        if (counter == 0x8000)
        {
            LATA ^= 0x04;
            LATA &= ~0x02;
            counter = 0;
        }
        
        stateMachine = nextState;
        switch(stateMachine)
        {
            case init:
                InitPIC16F18446();
        
                strcpy(UARTOutputMessageBuffer.bufferData, "Program Start\r\n");                   
                TransferDataInRingBuffer(&TXBuffer, &UARTOutputMessageBuffer);               
                
                nextState = idle;
                break;
        
            case idle:
                
                if(!RXBuffer.bufferEmpty)
                {
                    TransferDataInMessageBuffer(&RXBuffer, &UARTInputMessageBuffer);
                }
 
                if(UARTInputMessageBuffer.containsCompleteNotProcessedMessage)
                {
                    receivedCommand = ParseMessage(UARTInputMessageBuffer.bufferData);
                    UARTInputMessageBuffer.containsCompleteNotProcessedMessage = false;
                    ValidateMessage(&receivedCommand);
                }
                break;
                
                
                
            default:
                nextState = idle;
                break;
        }
 
    }       
    return;
}

/*--- Interrupt Service Routine ----------------------------------------------*/
void __interrupt() INTERRUPT_InterruptManager (void)
{
    if(TX1IF && TX1IE)
    {
        if(!TXBuffer.bufferEmpty)
            BufferPull((char*)&TX1REG, &TXBuffer);
        else
            TX1IE = 0;      
    }    
    
    
    if(RC1IF && RC1IE)
    {
        LATA1 = 1;
        BufferPush(RC1REG, &RXBuffer);
        counter = 0;
    }
    return;
    
}
