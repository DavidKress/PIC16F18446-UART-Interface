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
*      RB5:         UART RC
*      RB7:         UART TX
*      RA1:         LED
*      RA2:         LED 
*      RA5:         LED
*      RC5:         LED 
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
    init_state, 
    idle_state,
    receiveMessage_state    
}systemState_t;

typedef enum
{   
    no_event,
    systemStart_event,
    uartCharReceived_event, 
    uartMessageComplete_event,  
}systemEvent_t;


/*--- Declaration of functions, used only in this file -----------------------*/
systemState_t HandlerSystemStart(void);
systemState_t HandlerUartCharReceived(void);
systemState_t HandlerUartMessageComplete(void);
systemEvent_t EventDetector(void);



/*--- Definition of global variables, also accessed by other files -----------*/
messageBuffer_t UARTOutputMessageBuffer;
volatile ringBuffer_t RXBuffer;
volatile ringBuffer_t TXBuffer;

/*--- Definition of varialbes only accessed by this file and ISR -------------*/
static volatile unsigned long counter = 0;

/*--- Definition of varialbes only accessed by this file ---------------------*/
messageBuffer_t             UARTInputMessageBuffer;


/*--- Main Routine -----------------------------------------------------------*/
void main(void) {

    /*--- Declaration of local variables -------------------------------------*/    



    /*--- Definition of local variables --------------------------------------*/    
    systemState_t              currentState   = idle_state;
    systemState_t              nextState      = idle_state;
    systemEvent_t              newEvent       = systemStart_event;


    /*--- Endless main Loop --------------------------------------------------*/
    while(1)
    {

        /*--- Control LED blinks to indicate program running -----------------*/
        counter++;
        if (counter == 0x8000)
        {
            LATA ^= 0x04;   // Control LED
            LATA &= ~0x02;  // Clear UART RC interrupt LED
            counter = 0;
        }

        /*--- State machine --------------------------------------------------*/        
        currentState = nextState;
        switch(currentState)
        {
      
            case idle_state:
                
                if(newEvent == systemStart_event)
                {
                    nextState = HandlerSystemStart();
                    newEvent = no_event;
                }   
                
                if(newEvent == uartCharReceived_event)
                {
                    nextState = HandlerUartCharReceived();
                    newEvent = no_event;
                }   
                
                if(newEvent == uartMessageComplete_event)
                {
                    nextState = HandlerUartMessageComplete();
                    newEvent = no_event;
                }    
                
                break;
                
                
                
            default:
                nextState = idle_state;
                break;
        }
        newEvent = EventDetector();
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
        LATAbits.LATA1 = 1;
        BufferPush(RC1REG, &RXBuffer);
        counter = 0;
    }
    return;
    
}

systemEvent_t EventDetector(void)
{
    if(UARTInputMessageBuffer.containsCompleteNotProcessedMessage)
        return uartMessageComplete_event;
    
    if(!RXBuffer.bufferEmpty)
        return uartCharReceived_event;
 
    return no_event;
    
}


systemState_t HandlerSystemStart(void)
{
    InitPIC16F18446();
    strcpy(UARTOutputMessageBuffer.bufferData, "Program Start\r\n");                   
    TransferDataInRingBuffer(&TXBuffer, &UARTOutputMessageBuffer);               
    return idle_state;
}

systemState_t HandlerUartCharReceived(void)
{
    TransferDataInMessageBuffer(&RXBuffer, &UARTInputMessageBuffer);              
    return idle_state;
}

systemState_t HandlerUartMessageComplete(void)
{
    uartCommandParameters_t     receivedCommand; 
    receivedCommand = ParseMessage(UARTInputMessageBuffer.bufferData);
    UARTInputMessageBuffer.containsCompleteNotProcessedMessage = false;
    ValidateMessage(&receivedCommand);
    (*uartCommandLibrary[receivedCommand.commandLibraryIndex].commandFunction)(receivedCommand.numberOfParameters, receivedCommand.parameters);   
    return idle_state;
}