/* 
 * File:   uart.c
 * Author: kress-2
 *
 * Created on 21. Februar 2020, 13:06
 */



//------------------------------------------------------------------------------
//  Libraries
//------------------------------------------------------------------------------
#include <xc.h>

//------------------------------------------------------------------------------
//  Projectfiles  Headers
//------------------------------------------------------------------------------

#include "uart.h"

//------------------------------------------------------------------------------
//  UART_Send (RS232)
//------------------------------------------------------------------------------

/*void UART_Send(char *String_send)   // *String_send is a pointer to the actual Array or Variable
{
    while(*String_send != 0x00)     // Will be executed until String_send is equal to 0x00 (NUL -> look at the ASCII-Table)
    {
        while(TX1IF == 0)            // Will be executed until TXIF (TX-Interrupt-Flag) is equal to 1
        {
        }
        TX1REG = *String_send;       // TXREG gets the 8-bit value of the String_send 
        ++String_send;              // String_send gets to the next address over the pointer
    }
    while(TX1IF == 0)                // Will be executed until TXIF (TX-Interrupt-Flag) is equal to 1
    {
    }
    TX1REG = 0x0D;               // Sends a Carriage Return
    while(TX1IF == 0)                // Will be executed when TXIF (TX-Interrupt-Flag) is equal to 0
    {
    }   
    TX1REG = 0x0A;               // Sends a Line Feed
}*/

/*void UART_Send(char *String_send)   // *String_send is a pointer to the actual Array or Variable
{
    while(*String_send != 0x00)     // Will be executed until String_send is equal to 0x00 (NUL -> look at the ASCII-Table)
    {
        while(TX1IF == 0)            // Will be executed until TXIF (TX-Interrupt-Flag) is equal to 1
        {
        }
        TX1REG = *String_send;       // TXREG gets the 8-bit value of the String_send 
        ++String_send;              // String_send gets to the next address over the pointer
    }
    while(TX1IF == 0)                // Will be executed until TXIF (TX-Interrupt-Flag) is equal to 1
    {
    }
    TX1REG = 0x0D;               // Sends a Carriage Return
    while(TX1IF == 0)                // Will be executed when TXIF (TX-Interrupt-Flag) is equal to 0
    {
    }   
    TX1REG = 0x0A;               // Sends a Line Feed
}/*

//------------------------------------------------------------------------------
// UART_Read (RS232)
//------------------------------------------------------------------------------
/*void UART_Read(char *String_read, int Max_Read)
{
    char *String_read_0;
    
    String_read_0 = String_read;
    *String_read = RC1REG;
    while(*String_read != 0x0A)     // Will be executed until String_read is equal to 0x0A (Line Feed -> look at the ASCII-Table)
    {
        if(String_read<(String_read_0 + Max_Read - 1))
        {
            ++String_read;          // Pointer gets incremented
        }
        while(RC1IF == 0)
        {
        }
        
        *String_read = RC1REG;
    }
    *String_read = 0x00; 
    if(String_read > String_read_0)
    {
        --String_read;              // Pointer gets decremented
    }
    if(*String_read == 0x0D)        // Will be executed when String_read is equal to 0x0D (Carriage Return)
    {
        *String_read = 0x00;
    }
}*/
