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

/*--- Function declarations-------------------------------------------------- */
void ValidateMessage(uartCommandParameters_t*);
uartCommandParameters_t ParseMessage(char*);

#endif	/* PARSER_H */

