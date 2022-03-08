/*H**********************************************************************
* FILENAME :        configBits.h                    
*
* DESCRIPTION :
*       Provides the pragma configuration bits to configure the PIC controller
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

#ifndef CONFIGBITS_H
#define	CONFIGBITS_H

//------------------------------------------------------------------------------
//  Pragma Configurations
//------------------------------------------------------------------------------

// CONFIG1L
#pragma config FEXTOSC  = 4     // External Oscillator mode selection bits (Oscillator not enabled)
#pragma config RSTOSC  = 6      // Power-up default value for COSC bits (HFINTOSC (1MHz))

// CONFIG1H
#pragma config CLKOUTEN = 1     // CLKOUT function is disabled
#pragma config CSWEN  = 1       // Clock can be switched by software
#pragma config FCMEN  = 1       // Fail safe clock enabled


// CONFIG2L
#pragma config MCLRE = 1        // Master Clear Enable bit (MCLR pin is Master Clear function)
#pragma config PWRTS = 3        // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = 1      // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = 3        // Brown-out reset enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)

// CONFIG2H
#pragma config BORV = 1         // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 2.45V)
#pragma config ZCD = 1          // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = 1      // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
#pragma config STVREN = 1       // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)

// CONFIG3L
#pragma config WDTE = 0         // WDT disabled, SEN bit is ignored
#pragma config WDTCPS = 0x1F    // Divider Ratio 1:65536; software control of WDTPS

// CONFIG3H
#pragma config WDTCWS = 7       // Window always open (100%); software control; keyed access not required
#pragma config WDTCCS = 7       // Software Control

// CONFIG4L
#pragma config BBSIZE = 7       // 512 words boot block size
#pragma config BBEN = 1         // Boot Block disabled
#pragma config SAFEN = 1        // SAF disabled
#pragma config WRTAPP = 1       // Application Block not write protected

// CONFIG4H
#pragma config LVP = 1          // Low Voltage Propgramming enabled. MCLR/Vpp pin function is MCLR
#pragma config WRTSAF = 1       // SAF not write protected
#pragma config WRTD = 1         // Data EEPROM NOT write protected
#pragma config WRTC = 1         // Configuration egister not write protected
#pragma config WRTB = 1         // Boot Block not write protected

//CONFIG5L
#pragma config CP = 1           // UserNVM code protection disabled




#endif	/* CONFIGBITS_H */

