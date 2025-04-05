/** @file cmdproc.h
 *  @brief Header file for UART communication and command processing functions.
 * 
 *  This module provides function declarations and global buffers for handling UART communication 
 *  and parsing high-level commands. It allows for receiving and transmitting characters over UART, 
 *  managing communication buffers, calculating checksums, and processing structured commands 
 *  that control system behavior (e.g., requesting sensor data).
 * 
 *  The command processor interprets commands following a specific protocol (delimited with '#', 
 *  checksum verification, etc.) and produces appropriate responses by interacting with other modules, 
 *  such as sensor reading functions.
 * 
 * 
 *  @authors Henrique Ferreira  
 *           Rui Borregana  
 *  @date 5 Apr 2025
 *  @bug No known bugs
 */

#ifndef CMD_PROC_H_
#define CMD_PROC_H_


/* Some defines */

/** @def UART_RX_SIZE
 *  @brief Maximum number of characters the UART RX buffer can store.
 */                    
#define UART_RX_SIZE 20 

/** @def UART_TX_SIZE
 *  @brief Maximum number of characters the UART TX buffer can store.
 */	 
#define UART_TX_SIZE 10000	

/** @def SOF_SYM
 *  @brief Start Of Frame symbol used to mark the beginning of a command frame.
 */ 
#define SOF_SYM '#'	        

/** @def EOF_SYM
 *  @brief End Of Frame symbol used to mark the end of a command frame.
 */
#define EOF_SYM '!'   
       
/** @def HISTORY_SIZE
 *  @brief Number of past readings stored for each sensor (e.g., temperature, humidity, CO2).
 */
#define HISTORY_SIZE 20

/**
 * @var extern unsigned char UARTRxBuffer[UART_RX_SIZE]
 * @brief UART receive buffer 
 * 
 * Stores incoming characters received via UART.
 * 
 */
extern unsigned char UARTRxBuffer[UART_RX_SIZE];

/**
 * @var extern unsigned char UARTTxBuffer[UART_TX_SIZE]
 * @brief UART transmit buffer 
 * 
 * Stores outgoing characters to be transmitted via UART.
 * 
 */
extern unsigned char UARTTxBuffer[UART_TX_SIZE];


/**
 * @var static int rxBufLen
 * @brief  Current length of the UART RX buffer.
 * 
 */
static int __attribute__((unused)) rxBufLen = 0;

/**
 * @var static int txBufLen
 * @brief  Current length of the UART TX buffer.
 * 
 */
static int __attribute__((unused)) txBufLen = 0;



// Históricos dos últimos 20 valores lidos
/** 
 * @var static int temperatureHistory[HISTORY_SIZE]
 * @brief Historical temperature readings. 
 */
static int __attribute__((unused)) temperatureHistory[HISTORY_SIZE] = {0};

/** 
 * @var static int humidityHistory[HISTORY_SIZE]
 * @brief Historical humidity readings. 
 */
static int __attribute__((unused)) humidityHistory[HISTORY_SIZE] = {0};

/** 
 * @var static int co2History[HISTORY_SIZE]
 * @brief  Historical CO2 readings. 
 */
static int __attribute__((unused)) co2History[HISTORY_SIZE] = {0};

// Índices de leitura e histórico
/** 
 * @var static int tempHistIndex
 * @brief  Index for the temperature history array. 
 */
static int __attribute__((unused)) tempHistIndex = 0;

/** 
 * @var static int humHistIndex
 * @brief  Index for the humidity history array. 
 */
static int __attribute__((unused)) humHistIndex = 0;

/** 
 * @var static int co2HistIndex
 * @brief  Index for the CO2 history array. 
 */
static int __attribute__((unused)) co2HistIndex = 0;
/* Function prototypes */

/**
 * @brief Processes the chars in the RX buffer looking for commands.
 *
 * Parses and validates command frames from the RX buffer, then executes the appropriate response.
 *
 * @return
 *   -  0 : Valid command executed successfully  
 *   - -1 : Empty or incomplete command  
 *   - -2 : Invalid format (missing start/end symbols)  
 *   - -3 : Checksum mismatch  
 *   - -4 : Unknown command
 */
int cmdProcessor(void);

/**
 * @brief Adds a character to the UART RX buffer.
 *
 * Simulates receiving a character through UART and stores it in the receive buffer.
 *
 * @param car Character to be added to the buffer.
 * @return
 *   -  0 : Success  
 *   - -1 : Buffer overflow
 */
int rxChar(unsigned char car);

/**
 * @brief Adds a character to the UART TX buffer.
 *
 * Simulates transmitting a character through UART and stores it in the transmit buffer.
 *
 * @param car Character to be added to the buffer.
 * @return
 *   -  0 : Success  
 *   - -1 : Buffer overflow
 */
int txChar(unsigned char car);

/**
 * @brief Clears the UART RX buffer.
 *
 * Resets all values in the receive buffer to 0.
 */
void resetRxBuffer(void);

/**
 * @brief Clears the UART TX buffer.
 *
 * Resets all values in the transmit buffer to 0.
 */
void resetTxBuffer(void);


/**
 * @brief Computes a modulo-256 checksum of a data buffer.
 *
 * Used to validate the integrity of command frames.
 *
 * @param buf Pointer to the buffer to compute the checksum from.
 * @param nbytes Number of bytes to consider for the checksum.
 * @return The resulting checksum value.
 */
unsigned char calcChecksum(unsigned char * buf, int nbytes);

#endif
