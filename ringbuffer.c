
#include <xc.h>
#include <stdbool.h>
#include <string.h>
#include "ringbuffer.h"





char eolSequence[] = "\r";

void BufferPush(char data, buffer_t* buffer)
{
    if (!buffer->bufferFull)
    {
        buffer->bufferData[buffer->pushPointer] = data;
        buffer->pushPointer = ((buffer->pushPointer+1)&0x0F);
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

void BufferPull(char* data, buffer_t* buffer)
{
    if (!buffer->bufferEmpty)
    {
        *data = buffer->bufferData[buffer->pullPointer];
        buffer->pullPointer = ((buffer->pullPointer+1)&0x0F);
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

void BufferInit(buffer_t* buffer)
{
    buffer->bufferEmpty = true;
}

void TransferDataInMessageBuffer(buffer_t *tempRingBuffer, messageBuffer_t* messageBuffer)
{
    char* token;

    if(!messageBuffer->bufferFull)
    {
        while(!tempRingBuffer->bufferEmpty)
        {
           BufferPull(&messageBuffer->bufferData[messageBuffer->pushPointer], tempRingBuffer);
           messageBuffer->pushPointer++;
           if(messageBuffer->bufferData[messageBuffer->pushPointer-1] == 0x0A && messageBuffer->bufferData[messageBuffer->pushPointer-2] == 0x0D)
           {
               messageBuffer->bufferFull = true;
               token = strtok(messageBuffer->bufferData, eolSequence);
               messageBuffer->pushPointer = 0; 
           }
        }
    } 
    return;
}


void TransferDataInRingBuffer(buffer_t *tempRingBuffer, messageBuffer_t* messageBuffer)
{
    if(!tempRingBuffer->bufferFull)
    {
        while(!(messageBuffer->bufferData[messageBuffer->pullPointer-1] == 0x0A && messageBuffer->bufferData[messageBuffer->pullPointer-2] == 0x0D))
        {
           BufferPush(messageBuffer->bufferData[messageBuffer->pullPointer], tempRingBuffer);
           PIE3 |= 0x10;
           messageBuffer->pullPointer++;           
        }
        messageBuffer->bufferEmpty = true;
        messageBuffer->pullPointer = 0;    
    } 
    return;
}