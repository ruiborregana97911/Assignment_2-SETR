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
int cmdProcessor(void) {
    if (rxBufLen < 4) {
        printf("Comando muito curto para ser válido\n");
        return -1; // Comando muito curto para ser válido
    }
    
    int startIdx = -1;
    int endIdx = -1;
    
    // Procurar por '#' e '!'
    for (int i = 0; i < rxBufLen; i++) {
        if (UARTRxBuffer[i] == '#' && startIdx == -1) {
            startIdx = i;
        } else if (UARTRxBuffer[i] == '!' && startIdx != -1) {
            endIdx = i;
            break;
        }
    }
    
    // Se não encontrou ambos os delimitadores, retorna erro
    if (startIdx == -1 || endIdx == -1 || endIdx - startIdx < 3) {
        printf("Não encontrou ambos os delimitadores, retorna erro\n");
        return -1;
    }
    
    // Extrair CMD e CS
    int cmdIdx = startIdx + 1;
    int csIdx = endIdx - 1;
    
    // Calcular checksum esperado usando a função existente
    unsigned char calculatedCS = calcChecksum(&UARTRxBuffer[cmdIdx], csIdx - cmdIdx);
    unsigned char receivedCS = UARTRxBuffer[csIdx];
    
    // Exibir informações detalhadas
    printf("Frase completa lida: %.*s\n", endIdx - startIdx + 1, &UARTRxBuffer[startIdx]);
    printf("Comando: %c (%d)\n", UARTRxBuffer[cmdIdx], UARTRxBuffer[cmdIdx]);
    printf("Data: %.*s\n", csIdx - (cmdIdx + 1), &UARTRxBuffer[cmdIdx + 1]);
    printf("Comprimento total (com # e !): %d\n", endIdx - startIdx + 1);
    printf("Checksum recebido: %d\n", receivedCS);
    printf("Checksum calculado: %d\n", calculatedCS);
    
    // Verificar se o checksum é válido
    if (calculatedCS != receivedCS) {
        printf("Checksum inválido\n");
        return -2;
    }
    
    resetRxBuffer(); // Limpar buffer após processamento
    return 0; // Comando processado com sucesso
}




/* 
 * calcChecksum
 */ 
unsigned char calcChecksum(unsigned char * buf, int nbytes) {
	/* Here you are supposed to compute the modulo 256 checksum */
	/* of the first n bytes of buf. Then you should convert the */
	/* checksum to ascii (3 digitas/chars)*/
	
						
	
    int checksum = 0;
    
    for (int i = 0; i < nbytes; i++) {
        checksum += buf[i];
    }
    
    return (unsigned char)(checksum % 256); // Retorna como um unsigned char
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



