/**
 * @file cmdproc.c
 * @brief Implementation of the UART command processing module.
 * 
 * This file contains the implementation of the functions responsible for
 * processing commands received via UART. It includes parsing of command frames,
 * checksum validation, and generation of appropriate responses to the TX buffer.
 * 
 * The supported commands allow reading values from different sensors such as
 * temperature, humidity, and CO2, either individually or all at once.
 * 
 * @authors Henrique Ferreira  
 * 			Rui Borregana
 * 
 * @date 5 Apr 2025
 * @bug None known.
 * 
 * @see cmdproc.h
 * @see sensores.c
 */
 
#include <stdio.h>
#include <string.h>

#include "cmdproc.h"
#include "sensores.h"



unsigned char UARTRxBuffer[UART_RX_SIZE];
unsigned char UARTTxBuffer[UART_TX_SIZE];


 
/* Function implementation */

/* Função para limpar o histórico dos sensores */
void clearSensorHistory(void) {
    memset(temperatureHistory, 0, sizeof(temperatureHistory));
    memset(humidityHistory, 0, sizeof(humidityHistory));
    memset(co2History, 0, sizeof(co2History));
    tempHistIndex = 0;
    humHistIndex = 0;
    co2HistIndex = 0;
}


/* 
 * cmdProcessor
 * // Exibir informações detalhadas
    printf("Frase completa lida: %.*s\n", endIdx - startIdx + 1, &UARTRxBuffer[startIdx]);
    printf("Comando: %c (%d)\n", UARTRxBuffer[cmdIdx], UARTRxBuffer[cmdIdx]);
    printf("Data: %.*s\n", csIdx - (cmdIdx + 1), &UARTRxBuffer[cmdIdx + 1]);
    printf("Comprimento total (com # e !): %d\n", endIdx - startIdx + 1);
    printf("Checksum recebido: %d\n", receivedCS);
    printf("Checksum calculado: %d\n", calculatedCS);
 */ 
int cmdProcessor(void) {
    if (rxBufLen < 4) {
        printf("Comando muito curto para ser válido\n");
        resetRxBuffer();
        return -1;
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
        resetRxBuffer();
        return -2;
    }

    // Extrair CMD e CS
    int cmdIdx = startIdx + 1;
    int csIdx = endIdx - 1;

    // Calcular checksum esperado usando a função existente
    unsigned char calculatedCS = calcChecksum(&UARTRxBuffer[cmdIdx], csIdx - cmdIdx);
    unsigned char receivedCS = UARTRxBuffer[csIdx];

    // Verificar se o checksum é válido
    if (calculatedCS != receivedCS) {
        printf("Checksum inválido\n");
        resetRxBuffer();
        return -3;
    }

    // Processar comandos
    char cmd = UARTRxBuffer[cmdIdx];
    switch (cmd) {
        case 'A': {
            int temp = readTemperature();
            int hum = readHumidity();
            int co2 = readCO2();
            
							// atualizar historico
							temperatureHistory[tempHistIndex] = temp;
							humidityHistory[humHistIndex] = hum;
							co2History[co2HistIndex] = co2;

							tempHistIndex = (tempHistIndex + 1) % HISTORY_SIZE;
							humHistIndex = (humHistIndex + 1) % HISTORY_SIZE;
							co2HistIndex = (co2HistIndex + 1) % HISTORY_SIZE;

            // Montar resposta com formatação padronizada
            char response[100];
            int len = 0;

            response[len++] = '#';

            // T(valor temperatura) - sempre com 3 caracteres
            response[len++] = 'T';
            len += sprintf(&response[len], "%03d", temp); // Formatar para 3 caracteres (com zero à esquerda se necessário)

            // H(valor humidade) - sempre com 3 caracteres
            response[len++] = 'H';
            len += sprintf(&response[len], "%03d", hum);  // Formatar para 3 caracteres

            // C(valor CO2) - sempre com 5 caracteres
            response[len++] = 'C';
            len += sprintf(&response[len], "%05d", co2);  // Formatar para 5 caracteres

            // Calcular checksum
            unsigned char checksum = calcChecksum((unsigned char *)response + 1, len - 1);

            // Adicionar checksum
            response[len++] = checksum;

            // Adicionar '!'
            response[len++] = '!';

            // Enviar resposta char a char
            for (int i = 0; i < len; i++) {
                txChar(response[i]);
            }

            /*// Exibir o conteúdo do UARTTxBuffer
            printf("Conteúdo de UARTTxBuffer: ");
            for (int i = 0; i < txBufLen; i++) {
                printf("%c", UARTTxBuffer[i]);
            }
            printf("\n");

            // Exibir o checksum como inteiro
            printf("Checksum (inteiro): %d\n", checksum);*/

            break;
        }
        case 'L': {
            // Enviar 20 mensagens do histórico de leituras (A)
            for (int i = 0; i < HISTORY_SIZE; i++) {
                int temp = temperatureHistory[i];
				int hum = humidityHistory[i];
				int co2 = co2History[i];

                // Montar resposta com formatação padronizada
                char response[100];
                int len = 0;

                response[len++] = '#';

                // T(valor temperatura) - sempre com 3 caracteres
                response[len++] = 'T';
                len += sprintf(&response[len], "%03d", temp); // Formatar para 3 caracteres (com zero à esquerda se necessário)

                // H(valor humidade) - sempre com 3 caracteres
                response[len++] = 'H';
                len += sprintf(&response[len], "%03d", hum);  // Formatar para 3 caracteres

                // C(valor CO2) - sempre com 5 caracteres
                response[len++] = 'C';
                len += sprintf(&response[len], "%05d", co2);  // Formatar para 5 caracteres

                // Calcular checksum
                unsigned char checksum = calcChecksum((unsigned char *)response + 1, len - 1);

                // Adicionar checksum
                response[len++] = checksum;

                // Adicionar '!'
                response[len++] = '!';

                // Enviar resposta char a char
                for (int ii = 0; ii < len; ii++) {
                    txChar(response[ii]);
                }

                /*// Exibir o conteúdo do UARTTxBuffer
                printf("Conteúdo de UARTTxBuffer: ");
                for (int i = 0; i < txBufLen; i++) {
                    printf("%c", UARTTxBuffer[i]);
                }
                printf("\n");

                // Exibir o checksum como inteiro
                printf("Checksum (inteiro): %d\n", checksum);
                
                //resetTxBuffer();*/
            }

            break;
        }
        case 'R': {
            // Limpar o histórico
            clearSensorHistory();
            
            
            // Montar a resposta de confirmação
            char response[] = "#DONE";
            int len = 5;

           

            // Calcular checksum da palavra "DONE"
            unsigned char checksum = calcChecksum((unsigned char *)response + 1, len - 1);

            // Adicionar checksum
            response[len++] = checksum;

            // Adicionar '!'
            response[len++] = '!';

            // Enviar resposta char a char
            for (int i = 0; i < len; i++) {
                txChar(response[i]);
            }

            /*// Exibir o conteúdo do UARTTxBuffer
            printf("Conteúdo de UARTTxBuffer: ");
            for (int i = 0; i < txBufLen; i++) {
                printf("%c", UARTTxBuffer[i]);
            }
            printf("\n");

            // Exibir o checksum como inteiro
            printf("Checksum (inteiro): %d\n", checksum);*/

            break;
        }
        
        
        
        case 'P': {
            char subCmd = UARTRxBuffer[cmdIdx + 1];
            char response[100];
            int len = 0;
            response[len++] = '#';

            if (subCmd == 'T') {
                int temp = readTemperature();
                response[len++] = 'T';
                len += sprintf(&response[len], "%03d", temp);
                temperatureHistory[tempHistIndex] = temp;
                tempHistIndex = (tempHistIndex + 1) % HISTORY_SIZE;
            } else if (subCmd == 'H') {
                int hum = readHumidity();
                response[len++] = 'H';
                len += sprintf(&response[len], "%03d", hum);
                humidityHistory[humHistIndex] = hum;
                humHistIndex = (humHistIndex + 1) % HISTORY_SIZE;
            } else if (subCmd == 'C') {
                int co2 = readCO2();
                response[len++] = 'C';
                len += sprintf(&response[len], "%05d", co2);
                co2History[co2HistIndex] = co2;
                co2HistIndex = (co2HistIndex + 1) % HISTORY_SIZE;
            } else {
                printf("Comando desconhecido!\n");
                return -3;
            }

            unsigned char checksum = calcChecksum((unsigned char *)response + 1, len - 1);
            response[len++] = checksum;
            response[len++] = '!';

            for (int i = 0; i < len; i++) {
                txChar(response[i]);
            }
            
             /* // Exibir o conteúdo do UARTTxBuffer
            printf("Conteúdo de UARTTxBuffer: ");
            for (int i = 0; i < txBufLen; i++) {
                printf("%c", UARTTxBuffer[i]);
            }
            printf("\n");

            // Exibir o checksum como inteiro
            printf("Checksum (inteiro): %d\n", checksum);*/
            
            
            
            break;
        }
         
        
        default:
            printf("Comando desconhecido!\n");
            return -4;
    }

    resetRxBuffer();
    return 0;
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
    memset(UARTRxBuffer, 0, UART_RX_SIZE); // Limpa o buffer
    rxBufLen = 0;        
}

/*
 * resetTxBuffer
 */
void resetTxBuffer(void)
{
    memset(UARTTxBuffer, 0, UART_TX_SIZE); // Limpa o buffer
    txBufLen = 0;        
}




