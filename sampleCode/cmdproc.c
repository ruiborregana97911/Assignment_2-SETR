/* ****************************** */
/* See cmdProc.h for indications  */
/* ****************************** */
#include <stdio.h>
#include <string.h>

#include "cmdproc.h"

#define UART_RX_SIZE 20
#define UART_TX_SIZE 20

static unsigned char UARTRxBuffer[UART_RX_SIZE];
static unsigned char UARTTxBuffer[UART_TX_SIZE];
static int rxBufLen = 0;
static int txBufLen = 0;

/* Internal variables */
/* Used as part of the UART emulation */
static unsigned char UARTRxBuffer[UART_RX_SIZE];


static unsigned char UARTTxBuffer[UART_TX_SIZE];

 
/* Function implementation */

/* 
 * cmdProcessor
 */ 
int cmdProcessor(void)
{
	int i;
	unsigned char sid;
		
	/* Detect empty cmd string */
	if(rxBufLen == 0)
		return -1; 
	
	/* Find index of SOF */
	for(i=0; i < rxBufLen; i++) {
		if(UARTRxBuffer[i] == SOF_SYM) {
			break;
		}
	}
	
	/* If a SOF was found look for commands */
	if(i < rxBufLen) {
		
		switch(UARTRxBuffer[i+1]) { 
			
			case 'P':		
				/* Command "P" detected.							*/
				/* Follows one DATA byte that specifies the sensor	*/ 
				/* to read. I assume 't','h','c' for temp., humid. 	*/
				/* and CO2, resp.									*/   
		
				/* Check sensor type */
				sid = UARTRxBuffer[i+2];
				if(sid != 't' && sid != 'h' && sid != 'c') {
					return -2;
				}
				
				/* Check checksum */
				if(!(calcChecksum(&(UARTRxBuffer[i+1]),2))) {
					return -3;
				}
				
				/* Check EOF */
				if(UARTRxBuffer[i+6] != EOF_SYM) {
					return -4;
				}
			
				/* Command is (is it? ... ) valid. Produce answer and terminate */ 
				txChar('#');
				txChar('p'); /* p is the reply to P 							*/	
				txChar('t'); /* t indicate that it is a temperature 			*/
				txChar('+'); /* This is the sensor reading. You should call a 	*/
				txChar('2'); /*   function that emulates the reading of a 		*/
				txChar('1'); /*   sensor value 	*/
				txChar('1'); /* Checksum is 114 decimal in this case		*/
				txChar('1'); /*   You should call a funcion that computes 	*/
				txChar('4'); /*   the checksum for any command 				*/  
				txChar('!');
				
				/* Here you should remove the characters that are part of the 		*/
				/* command from the RX buffer. I'm just resetting it, which is not 	*/
				/* a good solution, as a new command could be in progress and		*/
				/* resetting  will generate errors									*/
				rxBufLen = 0;	
				
				return 0;
								
			default:
				/* If code reaches this place, the command is not recognized */
				return -2;				
		}
		
		
	}
	
	/* Cmd string not null and SOF not found */
	return -4;

}

/* 
 * calcChecksum
 */ 
int calcChecksum(unsigned char * buf, int nbytes) {
	/* Here you are supposed to compute the modulo 256 checksum */
	/* of the first n bytes of buf. Then you should convert the */
	/* checksum to ascii (3 digitas/chars) and compare each one */
	/* of these digits/characters to the ones in the RxBuffer,	*/
	/* positions nbytes, nbytes + 1 and nbytes +2. 				*/
	
	/* That is your work to do. In this example I just assume 	*/
	/* that the checksum is always OK.							*/	
	return 1;		
}



/*
 * rxChar
 */
int rxChar(unsigned char car)
{
    /* If rxbuff not full add char to it */
    if (rxBufLen < UART_RX_SIZE) {
        UARTRxBuffer[rxBufLen] = car;
        rxBufLen += 1;
        return 0;        
    }    
    /* If cmd string full return error */
    return -1;
}

/*
 * txChar
 */
int txChar(unsigned char car)
{
    /* If txbuff not full add char to it */
    if (txBufLen < UART_TX_SIZE) {
        UARTTxBuffer[txBufLen] = car;
        txBufLen += 1;
        return 0;        
    }    
    /* If cmd string full return error */
    return -1;
}

/*
 * resetRxBuffer
 */
void resetRxBuffer(void)
{
    rxBufLen = 0;        
}

/*
 * resetTxBuffer
 */
void resetTxBuffer(void)
{
    txBufLen = 0;        
}

/*
 * getTxBuffer
 */
void getTxBuffer(unsigned char * buf, int * len)
{
    *len = txBufLen;
    if (txBufLen > 0 && buf != NULL) {
        memcpy(buf, UARTTxBuffer, *len);
    }        
}



