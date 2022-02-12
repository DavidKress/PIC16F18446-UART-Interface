/* 
 * File:   parser.h
 * Author: David Kress
 *
 * Created on 12. Februar 2022, 07:39
 */

#ifndef PARSER_H
#define	PARSER_H


#define MAXCOMMANDSIZE 5

typedef struct uartCommand_s
{                  // Definiere struct biffer_t  
   char command[MAXCOMMANDSIZE];           // Buffer Data
   uint8_t numbOfParameters;
} uartCommand_t;






#endif	/* PARSER_H */

