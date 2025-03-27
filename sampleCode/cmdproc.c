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

 ////////////////////////////////////////////////////outro .h
 
 
#define SENSOR_DATA_SIZE 25
#define HISTORY_SIZE 20



// Vetores para armazenar valores fictícios dos sensores
static int temperatureData[SENSOR_DATA_SIZE] = {
    -10, -8, -5, 0, 3, 7, 10, 12, 15, 18,
    20, 22, 24, 25, 27, 28, 30, 32, 34, 35,
    36, 37, 38, 39, 40
};

static int humidityData[SENSOR_DATA_SIZE] = {
    20, 25, 30, 35, 40, 45, 50, 55, 60, 65,
    70, 75, 80, 85, 90, 95, 100, 98, 96, 94,
    92, 90, 88, 86, 84
};

static int co2Data[SENSOR_DATA_SIZE] = {
    400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300,
    1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300,
    2400, 2500, 2600, 2700, 2800
};


// Históricos dos últimos 20 valores lidos
static int temperatureHistory[HISTORY_SIZE] = {0};
static int humidityHistory[HISTORY_SIZE] = {0};
static int co2History[HISTORY_SIZE] = {0};

// Índices de leitura e histórico
static int tempIndex = 0, tempHistIndex = 0;
static int humIndex = 0, humHistIndex = 0;
static int co2Index = 0, co2HistIndex = 0;

//////////////////////////////////////////////////////////////mesmo .h
/* Function implementation */

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
        return -1;
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
        return -2;
    }

    // Processar comandos
    char cmd = UARTRxBuffer[cmdIdx];
    switch (cmd) {
        case 'A': {
            int temp = readTemperature();
            int hum = readHumidity();
            int co2 = readCO2();

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

            // Exibir o conteúdo do UARTTxBuffer
            printf("Conteúdo de UARTTxBuffer: ");
            for (int i = 0; i < txBufLen; i++) {
                printf("%c", UARTTxBuffer[i]);
            }
            printf("\n");

            // Exibir o checksum como inteiro
            printf("Checksum (inteiro): %d\n", checksum);

            break;
        }
        case 'L': {
            // Enviar 20 mensagens do histórico de leituras (A)
            for (int i = 0; i < 20; i++) {
                // Acessar os valores históricos de temperatura, umidade e CO2
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
                for (int i = 0; i < len; i++) {
                    txChar(response[i]);
                }

                // Exibir o conteúdo do UARTTxBuffer
                printf("Conteúdo de UARTTxBuffer: ");
                for (int i = 0; i < txBufLen; i++) {
                    printf("%c", UARTTxBuffer[i]);
                }
                printf("\n");

                // Exibir o checksum como inteiro
                printf("Checksum (inteiro): %d\n", checksum);
                
                resetTxBuffer();
            }

            break;
        }
        default:
            printf("Comando desconhecido!\n");
            return -3;
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

////////////////////////////////////////////////////////////////////////////////// outro .h




// Funções para ler os sensores, armazenar no histórico e atualizar os índices
int readTemperature() {
    int value = temperatureData[tempIndex];
    temperatureHistory[tempHistIndex] = value;
    tempHistIndex = (tempHistIndex + 1) % HISTORY_SIZE;
    tempIndex = (tempIndex + 1) % SENSOR_DATA_SIZE;
    return value;
}

int readHumidity() {
    int value = humidityData[humIndex];
    humidityHistory[humHistIndex] = value;
    humHistIndex = (humHistIndex + 1) % HISTORY_SIZE;
    humIndex = (humIndex + 1) % SENSOR_DATA_SIZE;
    return value;
}

int readCO2() {
    int value = co2Data[co2Index];
    co2History[co2HistIndex] = value;
    co2HistIndex = (co2HistIndex + 1) % HISTORY_SIZE;
    co2Index = (co2Index + 1) % SENSOR_DATA_SIZE;
    return value;
}


