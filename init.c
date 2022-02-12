/* 
 * File:   init.c
 * Author: kress-2
 *
 * Created on 20. Februar 2020, 10:20
 */



//------------------------------------------------------------------------------
//  Libraries
//------------------------------------------------------------------------------
#include <xc.h>


//------------------------------------------------------------------------------
//  Pragma Configurations
//------------------------------------------------------------------------------

// CONFIG3H

#pragma config FCMEN  = 1       // Fail safe clock enabled
#pragma config CSWEN  = 1       // Clock can be switched by software
#pragma config WDTE = 0         // WDT disabled, SEN bit is ignored
    

//------------------------------------------------------------------------------
//  Projectfiles  Headers
//------------------------------------------------------------------------------
#include "init.h"



//------------------------------------------------------------------------------
// Local Definitions
//------------------------------------------------------------------------------

// UART Baud Rate Generator Value calculated according to datasheet
#define BAUD_GENERATOR_VALUE (int)((long)CRYSTALFREQ/((long)UARTBAUDRATE*64))-1

void InitOscillator(void);
void InitPorts(void);
void InitPPS(void);
void InitUART(void);
void Interrupts(void);

 

void InitPIC16F18446(void)
{
    InitOscillator();
    InitPorts();
    InitPPS();
    InitUART();  
    Interrupts();
}


void Interrupts(void)
{
    INTCON |= 0xC0;     // Enable Global Interrupts
    PIE3 |= 0x20;       // Enable UART RX Interrupt
    
    
    
}



void InitOscillator()
{
    OSCFRQ = 0x05;      // Set HF Int OSC to 16MHz    
    OSCCON2= 0x60;      // Enable HF Int OSC, set clock divider to 1  
}



void InitPorts()
{
    LATA = 0x00;        // Port-A Outputs: RA0 - RA6 = 0
    TRISA = 0x00;       // TRISA bits: (1 = Pin gets an Input, 0 = Pin gets an Output)


    LATB = 0x00;        // Port-B Outputs: RB1 - RB7 = 0
    TRISB = 0x20;       // TRISB bits: (1 = Pin gets an Input, 0 = Pin gets an Output)
                        // - RB5: UART Input
                        // - RB7: UART Output
    ANSELB &= ~0x20;    // Disconnect RB5 from Analog Periperie

    LATC = 0x00;        // Port-C Outputs: RC0 - RC5 = 0
    TRISC = 0x00;       // TRISC bits: (1 = Pin gets an Input, 0 = Pin gets an Output)
}

void InitPPS()
{
    RX1PPS = 0x0D;      // Connect RB5 to UART RX   
    RB7PPS = 0x0F;      // Connect RB7 to UART TX
}


//------------------------------------------------------------------------------
//  UART_Init
//------------------------------------------------------------------------------
void InitUART()
{
    TX1STA =0;
    TXEN = 1;
    RC1STA = 0;
    SPEN = 1;
    CREN = 1;
    BAUD1CON = 0;
    SP1BRG = BAUD_GENERATOR_VALUE;
}
