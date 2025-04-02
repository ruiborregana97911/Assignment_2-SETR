/* ******************************************************/
/* SETR 23/24, Paulo Pedreiras                          */
/*	Base code for Unit Testing                          */
/*  	Simple example of command processor             */
/*    	for smart sensor node with 3 sensors			*/
/*     													*/
/*	Code is just for illustrative effects. E.g. error 	*/ 
/*		codes are "magic numbers" in the middle of the	*/
/*    	code instead of being (defined) text literals, 	*/
/* 		sensor data is not properly emulated, missing 	*/
/* 		commands, Checksum not implemented, ...			*/
/*														*/
/* ******************************************************/

#ifndef CMD_PROC_H_
#define CMD_PROC_H_

/**
 * @file cmdproc.h
 * @brief Header file for UART command processing module.
 */



/* Some defines */
/* Other defines should be return codes of the functions */
/* E.g. #define CMD_EMPTY_STRING -1                      */
#define UART_RX_SIZE 20 	/* Maximum size of the RX buffer */ 
#define UART_TX_SIZE 500	/* Maximum size of the TX buffer */ 
#define SOF_SYM '#'	        /* Start of Frame Symbol */
#define EOF_SYM '!'          /* End of Frame Symbol */

#define HISTORY_SIZE 20

/** @brief UART receive buffer */
extern unsigned char UARTRxBuffer[UART_RX_SIZE];

/** @brief UART transmit buffer */
extern unsigned char UARTTxBuffer[UART_TX_SIZE];


//unsigned char UARTRxBuffer[UART_RX_SIZE];
//unsigned char UARTTxBuffer[UART_TX_SIZE];
static int rxBufLen = 0;
static int txBufLen = 0;



// Históricos dos últimos 20 valores lidos
/** 
 * @brief Arrays to store historical sensor data 
 */
static int temperatureHistory[HISTORY_SIZE] = {0};
static int humidityHistory[HISTORY_SIZE] = {0};
static int co2History[HISTORY_SIZE] = {0};

// Índices de leitura e histórico
/** 
 * @brief Indices for sensor data and history tracking 
 */
static int tempHistIndex = 0;
static int humHistIndex = 0;
static int co2HistIndex = 0;
/* Function prototypes */

/**
 * @brief Processes the chars in the RX buffer looking for commands.
 * 
 * @return 
 *   - 0  : if a valid command was found and executed
 *   - -1 : if empty string or incomplete command found
 *   - -2 : if string format is wrong
 *   - -3 : if a checksum (CS) error is detected (command not executed)
 *   - -4 : if an invalid command was found
 */
int cmdProcessor(void);

/**
 * @brief Adds a character to the RX buffer.
 * 
 * This function simulates receiving a character via UART.
 * 
 * @param car Character to add to the RX buffer.
 * @return 
 *   - 0  : if success
 *   - -1 : if buffer is full
 */
int rxChar(unsigned char car);

/**
 * @brief Adds a character to the TX buffer.
 * 
 * This function simulates transmitting a character via UART.
 * 
 * @param car Character to add to the TX buffer.
 * @return 
 *   - 0  : if success
 *   - -1 : if buffer is full
 */
int txChar(unsigned char car);

/**
 * @brief Resets (clears) the RX buffer.
 */
void resetRxBuffer(void);

/**
 * @brief Resets (clears) the TX buffer.
 */
void resetTxBuffer(void);


/**
 * @brief Computes the checksum of a given number of characters.
 * 
 * This function calculates a modulo-256 checksum over the first `nbytes` of the given buffer.
 * 
 * @param buf Pointer to the buffer containing the data.
 * @param nbytes Number of bytes to include in the checksum calculation.
 * @return Computed checksum as an unsigned char.
 */
unsigned char calcChecksum(unsigned char * buf, int nbytes);

#endif
