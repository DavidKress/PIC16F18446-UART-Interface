/*H**********************************************************************
* FILENAME :        global.h                    
*
* DESCRIPTION :
*       Provides global preprocessor constant values, global type definitions
*       global variable declarations
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

#ifndef GLOBAL_H
#define	GLOBAL_H


/*--- Constants defined by the preprocessor ----------------------------------*/
#define CRYSTALFREQ     16000000UL
#define UARTBAUDRATE    9600
#define RINGBUFFERSIZE 16               // Must be power of two
#define MAXMESSAGESIZE 32
#define MAXNUMBOFPARAMETERS 3

/*--- Definition of global type definitions ----------------------------------*/

// This structure is designed to store char arrays with complete UART send- and 
// recieve strings. This strings are closed with an \n\r sequence.
typedef struct messageBuffer_s      
{                  
   char bufferData[MAXMESSAGESIZE];             // Buffer Data
   uint16_t pushPointer;                        // Next empty position
   uint16_t pullPointer;                        // Next read position
   uint8_t containsCompleteNotProcessedMessage; // \r\n on the end of the string and message is not processed yet
   uint8_t readyToReceiveNextMessage;           // Buffer empty or message is processed
}messageBuffer_t;  

// This buffer type is used to store data which is received and
// transmitted on the UART. It is designed to be interrupt-driven and to prevent
// an overflow in the UART peripheral.
typedef struct ringBuffer_s
{                   
   char bufferData[RINGBUFFERSIZE];
   uint16_t pullPointer;
   uint16_t pushPointer;
   uint8_t bufferEmpty;
   uint8_t bufferFull;
   uint8_t bufferOverflow;
   uint8_t bufferUnderflow;
} ringBuffer_t;

// This stucture is used to validate and execute a command after it has been
// recieved, parsed and validated
typedef struct uartCommandParameters_s
{
    uint16_t commandLibraryIndex;               // Command can be 
    uint8_t commandValid;
    uint8_t numberOfParametersValid;
    uint8_t numberOfParameters;
    uint16_t parameters[MAXNUMBOFPARAMETERS];
}uartCommandParameters_t;


/*--- Declaration of global variables acessible by all files -----------------*/
extern messageBuffer_t UARTOutputMessageBuffer;

/*--- Declaration of global variables acessible by all files and ISR ---------*/
extern volatile ringBuffer_t RXBuffer;
extern volatile ringBuffer_t TXBuffer;

#endif	/* GLOBAL_H */

