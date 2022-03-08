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

#ifndef SERVICEFUNCTIONS_H
#define	SERVICEFUNCTIONS_H

/*--- Function declarations-------------------------------------------------- */
int ErrorSyntax(uint8_t , uint16_t*);
int PortActionJas(uint8_t , uint16_t*);
int PortActionAma(uint8_t , uint16_t*);
int ZFC100(uint8_t , uint16_t*);
#endif	/* SERVICEFUNCTIONS_H */

