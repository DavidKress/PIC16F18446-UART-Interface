/*H**********************************************************************
* FILENAME :        parser.c                    
*
* DESCRIPTION :
*       Functions to parse messages which are received on the UART interface.
*       It is checked if the message contains a valid command and the parameters are extracted
*       Command library of valid UART commands
*       
*
* PUBLIC FUNCTIONS :
*       void ValidateMessage(uartCommandParameters_t*)    // Checks if a message is valid and prepares the answer string
*       uartCommandParameters_t ParseMessage(char*)		  // Extracts the command and the parameters of a received message
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


/*--- Constant values ------------------------------------------------------- */



/*--- Type definitions ------------------------------------------------------- */


/*--- Definition of global variables, accessed only by this file ------------*/


/* --- Functions -------------------------------------------------------------*/
int ErrorSyntax(uint8_t nParameter, uint16_t *parameters)
{   
    return 1;
}

int PortActionAma(uint8_t nParameter, uint16_t *parameters)
{
    LATAbits.LATA5 = !LATAbits.LATA5;
    return 1;
}

int PortActionJas(uint8_t nParameter, uint16_t *parameters)
{
    LATCbits.LATC5 = !LATCbits.LATC5;
    return 1;
}

int ZFC100(uint8_t nParameter, uint16_t* parameters)
{
    switch(parameters[0])
    {
        case 0:
            if (parameters[1] == 1)
            {
                LATAbits.LATA5 = 1;
                LATCbits.LATC5 = 1;
            }
            else
            {
                LATAbits.LATA5 = 0;
                LATCbits.LATC5 = 0;
            }
            break;
        
        case 1:
            if (parameters[1] == 1)
                LATAbits.LATA5 = 1;
            else
                LATAbits.LATA5 = 0;
            break;

        case 2:
            if (parameters[1] == 1)
                LATCbits.LATC5 = 1;
            else
                LATCbits.LATC5 = 0;
        break;

        default:
            break;
    }
                    
    
    
    return 1;   
    
    
}

