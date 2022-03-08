/*H**********************************************************************
* FILENAME :        init.c                    
*
* DESCRIPTION :
*       Functions to initialize the PIC microcontroller.
*       Every peripheral block is configured in a sepparate 
*       function which are called by the one general init function.
*       The global variables are also initialized in this block.
*
* PUBLIC FUNCTIONS :
*       void InitPIC16F18446(void)    // Calls all initialization functions
*       
* WRITTEN FOR HARDWARE:
*       Microchip PCB Curiosity LPC with PIC16F18446
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
#include "global.h"
#include "uart.h"

/*--- Constant values ------------------------------------------------------- */
#define BAUD_GENERATOR_VALUE (int)((long)CRYSTALFREQ/((long)UARTBAUDRATE*64))-1

/*--- Declaration of functions only accessed by this file --------------------*/
void InitGlobalVariables(void);
void InitOscillator(void);
void InitPorts(void);
void InitPPS(void);
void InitUART(void);
void InitInterrupts(void);


 
/* --- Functions -------------------------------------------------------------*/
void InitPIC16F18446(void)
{
    InitGlobalVariables();
    InitOscillator();
    InitPorts();
    InitPPS();
    InitUART();  
    InitInterrupts();
}

void InitGlobalVariables()
{         
    BufferInit(&RXBuffer);
    BufferInit(&TXBuffer);
}



void InitOscillator()
{
    OSCFRQbits.HFFRQ = 0x05;      // Set HF Int OSC to 16MHz    
    OSCCON1bits.NOSC = 0x06;      // Enable HF Int OSC, set clock divider to 1     
    OSCCON1bits.NDIV= 0;
 
}

void InitPorts()
{
    ANSELA = 0x00;   
    LATA = 0x00;
    TRISA = 0x00;

 
    ANSELB = 0x00;
    LATB = 0x00;
    TRISB = 0x00;    

    ANSELC = 0x00;
    LATC = 0x00;    
    TRISC = 0x00;
    

    
    TRISB = 0x00;
    TRISBbits.TRISB5 = 1;       // - RB5: UART input 
    ANSELBbits.ANSB5 = 0;      // Disconnect RB5 from analog periperie
    
    TRISC = 0x00;
}

void InitPPS()
{
    RX1PPSbits.PORT = 0x01;   // PORT B
    RX1PPSbits.PIN = 0x05;    // RX5 (X=B))
    RB7PPS = 0x0F;            // Connect RB7 to UART TX
}

void InitUART()
{
    TX1STA = 0x00;
    TX1STAbits.TXEN = 1;
    
    RC1STA = 0x00;
    RC1STAbits.SPEN = 1;
    RC1STAbits.CREN = 1;
    
    BAUD1CON = 0x00;
    
    SP1BRG = BAUD_GENERATOR_VALUE;
}

void InitInterrupts(void)
{
    //INTCON |= 0xC0;     // Enable Global Interrupts
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE3bits.RC1IE= 1; // Enable UART RX Interrupt 
}
