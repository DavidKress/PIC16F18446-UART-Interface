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
    TX1STA =0b00100000;
    //        ||||||||
    //        |||||||\_____ // Bit 0: 9th bit of Transmit Data (Can be adress/data bit or a parity bit)
    //        ||||||+------ // Bit 1: Transmit Shift Register Status bit (1 = TSR empty / 0 = TSR full)
    //        |||||+------- // Bit 2: High Baud Rate Select bit (1 = High Speed / 0 = Low Speed)
    //        ||||+-------- // Bit 3: Send Break Character bit (1 = Send Sync Break on next transmition / 0 = Sync Break transmition completed)
    //        |||\--------- // Bit 4: EUSART Mode select bit (1 = Synchronous mode / 0 = Asynchronous mode)
    //        ||\__________ // Bit 5: Transmit enable bit (1 = Transmit enabled / 0 = Transmit disabled)
    //        |\----------- // Bit 6: 9-Bit Transmit Enable bit (1 = Selects 9-bit transmition / 0 = Selects 8-bit transmition)
    //        +------------ // Bit 7: Clock Source Select bit (Asynchronous Mode = Don't care) (Synchronous Mode: 1 = Master Mode / 0 = Slave Mode)
    //---------------------------------------------------------------------------------------------------------------------------------------------
    
    RC1STA =0b10010000;
    //        ||||||||
    //        |||||||+----- // Bit 0: 9th bit of Received Data (This can be address/data bit or a parity bit and must be calculated by user firmware)
    //        ||||||+------ // Bit 1: Overrun Error bit (1 = Overrun error (can be cleared by cleaning bit CREN / 0 = No overrun error)
    //        |||||+------- // Bit 2: Framing Error bit (1 = Framing error (can be updated by reading RCREG register and receiving next valid byte / 0 = No framing error)
    //        ||||+-------- // Bit 3: Address Detect Enable bit (1 = Enables address detection, enables interrupt and loads the receiver buffer when RSR<8> is set / 
    //        |||           //                                   0 = Disable address detection, all bytes are received and ninth bit can be used as parity bit)
    //        |||+--------- // Bit 4: Continuous Receive Enable bit (Asynchronous: 1 = Enables Receiver / 0 = Disables Receiver)
    //        |||           //                                      (Synchronous: 1 = Enables continuous receive until enable bit CREN is cleard / 0 = Disables continuous reveive)
    //        ||+---------- // Bit 5: Single Receive Enable bit (Asynchronous: Don't care) (Master Mode: 1 = Enables single receive / 0 = Disables single receive) (Slave Mode: 0 = Don't care)
    //        |+----------- // Bit 6: 9-Bit Receive Enable bit (1 = Selects 9-bit reception / 0 = Selects 8-bit reception)
    //        +------------ // Bit 7: Serial Port Enable bit (1 = Serial port enabled (configures RX/DT and TX/CK pins as serial port pins) / 0 = Serial port is disabled (held in reset))
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    
    BAUD1CON = 0b00000000;
    //          ||||||||
    //          |||||||+--- // Bit 0: Auto-Baud Detect Enable bit (Asynchronous: 1 = Enable baud rate measurement on the next character / 0 = Baud rate measurement disabled or completed)
    //          |||||||     //                                    (Synchronous: Unused in this mode)
    //          ||||||+---- // Bit 1: Wake-up Enable bit (Asynchronous: 1 = EUSART will continue to sample the RX pin (interrupt generated on falling edge) 0 = RX pin not monitored on rising edge detected)
    //          ||||||      //                           (Synchronous: Unused in this mode)
    //          |||||+----- // Bit 2: Unimplemendet (Read as 0)
    //          ||||+------ // Bit 3: 16-Bit Baud Rate Register Enable bit (1 = 16-bit Baud Rate Generator (SPBRGH and SPBRG) / 0 = 8-bit Baud Rate Generator (SPBRG only))
    //          |||+------- // Bit 4: Clock and Data Polarity Select bit (Asynchronous: 1 = TX data inverted / 0 = TX data not inverted) (Synchronous: 1 = CK clocks are inverted / 0 = CK clocks are not inverted)
    //          ||+-------- // Bit 5: Received Data Polarity Select bit (Asynchronous: 1 = RX data is inverted / 0 = RX data received is not inverted)
    //          ||          //                                          (Synchronous: 1 = CK clocks are inverted / 0 = CK clocks are not inverted)
    //          |+--------- // Bit 6: Receive IOperation Idle Status bit (1 = Receive operation is Idle / 0 = Receive operation is active)
    //          +---------- // Bit 7: Auto-Baud Acquisition Rollover Status bit (1 = A BRG rollover has occurred during Auto-Baud Rate Detect mode (must be cleared in software) / 0 = No BRG rollover has occurred)
    //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    SP1BRG = BAUD_GENERATOR_VALUE;             // generates the Baud-Rate of 9600 with the 16MHz external Crystal 
}

