/*H**********************************************************************
* FILENAME :        uart.c                    
*
* DESCRIPTION :
*       Functions to operate the UART TX buffer, the UART RC buffer, the
*       message input and the message output buffer.
*       
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
#include <stdbool.h>
#include <string.h>

/*--- Project headers-------------------------------------------------------- */
#include "uart.h"

/*--- Functions --------------------------------------------------------------*/
void BufferPush(char data, volatile ringBuffer_t* buffer)
{
    if (!buffer->bufferFull)
    {
        buffer->bufferData[buffer->pushPointer] = data;
        buffer->pushPointer = ((buffer->pushPointer+1)&RINGBUFFERSIZE-1);
        buffer->bufferEmpty = false;
        buffer->bufferUnderflow = false;
        if(buffer->pushPointer== buffer->pullPointer)
            buffer->bufferFull = true;         
    }
    else
    {
        buffer->bufferOverflow = true;
    } 
}

void BufferPull(char* data, volatile ringBuffer_t* buffer)
{
    if (!buffer->bufferEmpty)
    {
        *data = buffer->bufferData[buffer->pullPointer];
        buffer->pullPointer = ((buffer->pullPointer+1)&RINGBUFFERSIZE-1);
        buffer->bufferFull = false;
        buffer->bufferOverflow = false;
        if(buffer->pushPointer== buffer->pullPointer)
            buffer->bufferEmpty = true;           
    }
    else
    {
       buffer->bufferUnderflow = true; 
        
    }
}

void BufferInit(volatile ringBuffer_t* buffer)
{
    for(int i = 0; i<RINGBUFFERSIZE;i++)
       buffer->bufferData[i] = '\0';
    buffer->pullPointer = 0;
    buffer->pushPointer = 0;
    buffer->bufferEmpty = true;
    buffer->bufferFull = false;
    buffer->bufferOverflow = false;
    buffer->bufferUnderflow = false;
}

void TransferDataInMessageBuffer(volatile ringBuffer_t *tempRingBuffer, messageBuffer_t* messageBuffer)
{
    char* token;
    char eolSequence[] = "\r";

    if(!messageBuffer->containsCompleteNotProcessedMessage)
    {
        while(!tempRingBuffer->bufferEmpty)
        {
           BufferPull(&messageBuffer->bufferData[messageBuffer->pushPointer], tempRingBuffer);
           messageBuffer->pushPointer++;
           if(messageBuffer->bufferData[messageBuffer->pushPointer-1] == 0x0A && messageBuffer->bufferData[messageBuffer->pushPointer-2] == 0x0D)
           {
               messageBuffer->containsCompleteNotProcessedMessage = true;
               token = strtok(messageBuffer->bufferData, eolSequence);
               messageBuffer->pushPointer = 0; 
           }
        }
    } 
    return;
}


void TransferDataInRingBuffer(volatile ringBuffer_t *tempRingBuffer, messageBuffer_t* messageBuffer)
{
    if(!tempRingBuffer->bufferFull)
    {
        while(!(messageBuffer->bufferData[messageBuffer->pullPointer-1] == 0x0A && messageBuffer->bufferData[messageBuffer->pullPointer-2] == 0x0D))
        {
           BufferPush(messageBuffer->bufferData[messageBuffer->pullPointer], tempRingBuffer);
           PIE3 |= 0x10;
           messageBuffer->pullPointer++;           
        }
        messageBuffer->readyToReceiveNextMessage = true;
        messageBuffer->pullPointer = 0;    
    } 
    return;
}

