/*H**********************************************************************
* FILENAME :        uarth.h                    
*
* DESCRIPTION :
*       Provides global functions of uart.c
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



#ifndef RINGBUFFER_H
#define	RINGBUFFER_H

/*--- Project headers-------------------------------------------------------- */
#include "global.h"

/*--- Function declarations-------------------------------------------------- */
void BufferInit(volatile ringBuffer_t*);
void BufferPush(char, volatile ringBuffer_t*);
void BufferPull(char*, volatile ringBuffer_t*);

void TransferDataInRingBuffer(volatile ringBuffer_t*, messageBuffer_t*);
void TransferDataInMessageBuffer(volatile ringBuffer_t*, messageBuffer_t*);

#endif	/* RINGBUFFER_H */

