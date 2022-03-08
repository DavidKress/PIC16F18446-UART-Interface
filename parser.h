/*H**********************************************************************
* FILENAME :        parser.h                    
*
* DESCRIPTION :
*       Provides public functions of parser.c
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

#ifndef PARSER_H
#define	PARSER_H

#define MAXCOMMANDSIZE 7
#define COMMANDLIBRARYSIZE 4

/*--- uartCommandDefinition_t ---
* Defines the name of a command and it's number of attributes
* which it needs to be a valid command. 
* ToDo: Define the target of a function pointer that defines which 
* function shall be executed when the command is rreceived*/
typedef struct uartCommandDefinition_s
{                   
   char command[MAXCOMMANDSIZE];  // Command name
   uint8_t numbOfParameters;  // Number of parameters to make the command valid
   int (*commandFunction)(uint8_t , uint16_t*);
} uartCommandDefinition_t;


/*--- Function declarations-------------------------------------------------- */
void ValidateMessage(uartCommandParameters_t*);
uartCommandParameters_t ParseMessage(char*);
uartCommandDefinition_t uartCommandLibrary[COMMANDLIBRARYSIZE];

#endif	/* PARSER_H */

