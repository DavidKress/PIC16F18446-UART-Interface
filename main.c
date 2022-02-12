/*
 * File:   main.c
 * Author: David Kress
 *
 * Created on 16. Januar 2022, 22:44
 */


#include <xc.h>
#include <string.h>
#include <stdbool.h>

#include "ringbuffer.h"

#include "init.h"
#include "uart.h"
#include "parser.h"

typedef enum
{
    init, 
    idle,
    receiveMessage
    
}statemachine_t;


//static  char cSend[25];           // RS232 from PIC to Device

static  char command[10];
static int16_t commandParameter[3];



uartCommand_t commandDefinitions[]=
{
    {
        "Jas",
        2
    },
    
        {
        "Ama",
        2
    },
    
    
    
    
};



static statemachine_t stateMachine = init;
static statemachine_t nextState = init;

static volatile unsigned long counter = 0;




void ParseMessage(char*);


void main(void) {
   
    BufferInit(&RXBuffer);
    BufferInit(&TXBuffer);
    
    while(1)
    {

        counter++;
        if (counter == 0x8000)
        {
            LATA ^= 0x04;
            LATA &= ~0x02;
            counter = 0;
        }
        
        stateMachine = nextState;
        switch(stateMachine)
        {
            case init:
                InitPIC16F18446();

                
                strcpy(UARTOutputMessageBuffer.bufferData, "Program Start\r\n");               
                UARTOutputMessageBuffer.pullPointer=0;
                UARTOutputMessageBuffer.bufferEmpty = false;        
                TransferDataInRingBuffer(&TXBuffer, &UARTOutputMessageBuffer);               
                
                
                
                nextState = idle;
                break;
        
            case idle:
                
                if(!RXBuffer.bufferEmpty)
                {
                    TransferDataInMessageBuffer(&RXBuffer, &UARTInputMessageBuffer);
                }
 
                if(UARTInputMessageBuffer.bufferFull)
                {
                    ParseMessage(UARTInputMessageBuffer.bufferData);
                    UARTInputMessageBuffer.bufferFull = false;
                }
                break;
                
                
                
            default:
                break;
        }
        

    }


        
    return;
}

void __interrupt() INTERRUPT_InterruptManager (void)
{
    if((PIR3 & 0x10) && (PIE3 & 0x10))
    {
        if(!TXBuffer.bufferEmpty)
            BufferPull((char*)&TX1REG, &TXBuffer);
        else
            PIE3 &= ~0x10;      
    }    
    
    
    if((PIR3 & 0x20) && (PIE3 & 0x20))
    {
        LATA |= 0x02;
        BufferPush(RC1REG, &RXBuffer);
        counter = 0;
    }
    return;
    
}




void ParseMessage(char* message)
{
    char tmpMessage[30];
    char* parsePointer;
    uint8_t foundArguments = 0;
    char delimiter[] = " ";
    uint8_t commandValid = false;

    strcpy(tmpMessage, message);

  //Decode Command and Parameters
    parsePointer = strtok(tmpMessage, delimiter);
    strcpy(command, parsePointer);

    strcpy(UARTOutputMessageBuffer.bufferData, "ES\r\n");
    int i = 0;
    while (i<2 && commandValid == false)
    {
        if (!(strcmp(command, commandDefinitions[i].command)))
        {
            if(parsePointer != NULL) {
                parsePointer = strtok(NULL, delimiter);
                commandParameter[0] = atoi(parsePointer);
                foundArguments++;
            }

            if(parsePointer != NULL) {
                parsePointer = strtok(NULL, delimiter);
                commandParameter[1] = atoi(parsePointer);
                foundArguments++;
            }
            
            //strcpy(UARTOutputMessageBuffer.bufferData, "Jas A\r\n");  
            commandValid = true;   
            strcat(strcpy(UARTOutputMessageBuffer.bufferData, commandDefinitions[i].command), " A\r\n");
        }
        
        if(!commandValid)
            i++;
    }
    

        
        
    
    /*if(strcmp(command,commandDefinitions[0].command) == 0)
    {

        if(parsePointer != NULL) {
            parsePointer = strtok(NULL, delimiter);
            commandParameter[0] = atoi(parsePointer);
            foundArguments++;

        }

        if(parsePointer != NULL) {
            parsePointer = strtok(NULL, delimiter);
            commandParameter[1] = atoi(parsePointer);
            foundArguments++;

        }            
        strcpy(UARTOutputMessageBuffer.bufferData, "Jas A\r\n");
    }
    else
    {
        strcpy(UARTOutputMessageBuffer.bufferData, "ES\r\n");
      //Todo \r\n schöner anhängen  
    }*/

    UARTOutputMessageBuffer.pullPointer=0;
    UARTOutputMessageBuffer.bufferEmpty = false;        

    TransferDataInRingBuffer(&TXBuffer, &UARTOutputMessageBuffer);
 
    return;
}