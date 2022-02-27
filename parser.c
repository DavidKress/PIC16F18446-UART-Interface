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
#include "parser.h"
#include "uart.h"

/*--- Constant values ------------------------------------------------------- */
#define COMMANDLIBRARYSIZE 3
#define MAXCOMMANDSIZE 7


/*--- Type definitions ------------------------------------------------------- */

/*--- uartCommandDefinition_t ---
* Defines the name of a command and it's number of attributes
* which it needs to be a valid command. 
* ToDo: Define the target of a function pointer that defines which 
* function shall be executed when the command is rreceived*/
typedef struct uartCommandDefinition_s
{                   
   char command[MAXCOMMANDSIZE];  // Command name
   uint8_t numbOfParameters;  // Number of parameters to make the command valid
} uartCommandDefinition_t;


/*--- Definition of global variables, accessed only by this file ------------*/
uartCommandDefinition_t uartCommandLibrary[COMMANDLIBRARYSIZE]=
{
    {
        "ES",
        0
    },
    {
        "Jas",
        2
    },
    
        {
        "Ama",
        2
    },    
};

/* --- Functions -------------------------------------------------------------*/
void ValidateMessage(uartCommandParameters_t* newMessage)
{
    char answerString[20];
   if(newMessage->commandValid != true)
       strcat(strcpy(answerString, "ES"), " \r\n");
   else
       strcat(strcpy(answerString, uartCommandLibrary[newMessage->commandLibraryIndex].command), " A\r\n");
        
    strcpy(UARTOutputMessageBuffer.bufferData, answerString);
    //UARTOutputMessageBuffer.pullPointer=0;
    //UARTOutputMessageBuffer.readyToReceiveNextMessage = false;        
    TransferDataInRingBuffer(&TXBuffer, &UARTOutputMessageBuffer);  
}

uartCommandParameters_t ParseMessage(char* message)
{
    char tmpMessage[30];
    char* parsePointer;
    char delimiter[] = " ";   
    static  char command[10];
    uint8_t foundArguments = 0;
    uint8_t commandValid = false;
    uint16_t commandCounter = 1;
    
    uartCommandParameters_t tempCommand;

    //Isolate Command   
    strcpy(tmpMessage, message);
    parsePointer = strtok(tmpMessage, delimiter);
    strcpy(command, parsePointer);

    //Validate Command and Isolate Paramaters
    while (commandCounter< COMMANDLIBRARYSIZE  && commandValid == false)
    {
        if (!(strcmp(command, uartCommandLibrary[commandCounter].command)))
        {
            commandValid = true;
            
            while(parsePointer != NULL) {
                parsePointer = strtok(NULL, delimiter);
                if(parsePointer != NULL)
                {
                    tempCommand.parameters[foundArguments] = (uint16_t)(atoi(parsePointer));
                    foundArguments++;
                }
            }

        }
        
        if(!commandValid)
            commandCounter++;
    }
    if(!commandValid)
    {
        commandCounter = 0;           
    }
    tempCommand.commandValid = commandValid;
    tempCommand.commandLibraryIndex = commandCounter;
    
    return tempCommand;
}