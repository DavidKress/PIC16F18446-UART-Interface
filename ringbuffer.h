/* 
 * File:   ringbuffer.h
 * Author: David Kress
 *
 * Created on 29. Januar 2022, 20:31
 */

#ifndef RINGBUFFER_H
#define	RINGBUFFER_H

#define MAXMESSAGESIZE 32
#define BUFFERSIZE 16

typedef struct buffer_s
{                  // Definiere struct biffer_t  
   char bufferData[BUFFERSIZE];           // Buffer Data
   uint16_t pullPointer;
   uint16_t pushPointer;
   uint8_t bufferEmpty;
   uint8_t bufferFull;
   uint8_t bufferOverflow;
   uint8_t bufferUnderflow;
 
} buffer_t;

typedef struct messageBuffer_s
{                  // Definiere struct biffer_t  
   char bufferData[MAXMESSAGESIZE];           // Buffer Data
   uint16_t pushPointer;
   uint16_t pullPointer;   
   uint8_t bufferFull;
   uint8_t bufferEmpty;

} messageBuffer_t;


static messageBuffer_t UARTInputMessageBuffer;
static messageBuffer_t UARTOutputMessageBuffer;

static buffer_t RXBuffer;
static buffer_t TXBuffer;

void BufferInit(buffer_t*);
void BufferPush(char, buffer_t*);
void BufferPull(char*, buffer_t*);

void TransferDataInRingBuffer(buffer_t*, messageBuffer_t*);
void TransferDataInMessageBuffer(buffer_t*, messageBuffer_t*);




#endif	/* RINGBUFFER_H */

