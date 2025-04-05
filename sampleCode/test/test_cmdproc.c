/**
 * @file test_cmdproc.c
 * @brief Test suite for the command processing module.
 * 
 * This file contains the test cases for validating the functionality of the command 
 * processing functions in the `cmdproc` module. It uses the Unity testing framework 
 * to verify the correctness of functions like `rxChar`, `txChar`, `resetRxBuffer`, 
 * `resetTxBuffer`, and `cmdProcessor`. The tests ensure that the UART buffers are 
 * correctly handled, checksums are computed properly, and the expected responses 
 * are generated for valid and invalid commands.
 * 
 * The test cases also cover various command types, including valid PT, PH, PC, 
 * A, L, R, and invalid commands with issues like missing data, incorrect delimiters, 
 * and invalid checksums.
 * 
 * @authors	Henrique Ferreira  
 * 			Rui Borregana
 * 
 * @date 5 Apr 2025
 * @bug None known.
 * 
 * @see cmdproc.c
 */

#include <unity.h>
#include "cmdproc.h"
#include "sensores.h"
#include <stdio.h>

/**
 * @brief Setup function for Unity tests.
 * 
 * This function is called before each test to initialize the test environment. 
 * It resets the TX and RX buffers to ensure each test starts with a clean state.
 */
void setUp(void)
{
	resetTxBuffer();
	resetRxBuffer();	
	
}

/**
 * @brief Tear down function for Unity tests.
 * 
 * This function is called after each test to clean up. Currently, no cleanup 
 * is needed, but the function is provided for future extensions if necessary.
 */
void tearDown(void)
{
	// No cleanup required
}

/**
 * @brief Function to print the buffer in hexadecimal format.
 * 
 * This function takes the buffer and its size as input and prints the buffer content
 * in hexadecimal format for debugging purposes.
 * 
 * @param buffer Pointer to the buffer to be printed.
 * @param size Size of the buffer.
 */
// Função para imprimir o buffer em formato hexadecimal
void printBufferHex(const unsigned char *buffer, size_t size) {
    char message1[100];  // Buffer para armazenar a primeira mensagem
    char message2[1020];  // Buffer para armazenar a segunda mensagem (com o conteúdo em hex)
    size_t offset = 0;   // Usar size_t para compatibilidade com sizeof

    // Primeira mensagem: Texto explicativo
    offset += snprintf(message1 + offset, sizeof(message1) - offset, "Buffer TX (Hex): ");
    
    // Exibir a primeira mensagem
    TEST_MESSAGE(message1);  // Exibir no Unity
    
    // Segunda mensagem: O conteúdo do buffer em hexadecimal
    offset = 0;  // Resetar o offset para a segunda mensagem
    for (size_t i = 0; i < size; i++) {
        offset += snprintf(message2 + offset, sizeof(message2) - offset, "%02X ", buffer[i]);
        if (offset >= sizeof(message2)) {
            break;  // Evita estouro do buffer
        }
    }

    // Exibir a segunda mensagem com o conteúdo hexadecimal do buffer
    TEST_MESSAGE(message2);  // Exibir no Unity
}



/**
 * @brief Test for the rxChar function.
 * 
 * This test ensures that the rxChar function correctly stores the received characters 
 * in the RX buffer and returns the expected result.
 */
void test_rxChar(void){
	
	char char1 = '#';
	char char2 = 'B';
	char char3 = '5';
	
	
	int return1 = rxChar(char1);
	int return2 = rxChar(char2);
	int return3 = rxChar(char3);


	TEST_ASSERT_EQUAL_CHAR(char1, UARTRxBuffer[0]);
	TEST_ASSERT_EQUAL_CHAR(char2, UARTRxBuffer[1]);
	TEST_ASSERT_EQUAL_CHAR(char3, UARTRxBuffer[2]);
	
	
	TEST_ASSERT_EQUAL_INT(0, return1);
	TEST_ASSERT_EQUAL_INT(0, return2);
	TEST_ASSERT_EQUAL_INT(0, return3);
	
}


/**
 * @brief Test for the txChar function.
 * 
 * This test ensures that the txChar function correctly stores the transmitted characters 
 * in the TX buffer and returns the expected result.
 */
void test_txChar(void){
	
	char char1 = '#';
	char char2 = 'B';
	char char3 = '5';
	
	
	int return1 = txChar(char1);
	int return2 = txChar(char2);
	int return3 = txChar(char3);


	TEST_ASSERT_EQUAL_CHAR(char1, UARTTxBuffer[0]);
	TEST_ASSERT_EQUAL_CHAR(char2, UARTTxBuffer[1]);
	TEST_ASSERT_EQUAL_CHAR(char3, UARTTxBuffer[2]);
	
	
	TEST_ASSERT_EQUAL_INT(0, return1);
	TEST_ASSERT_EQUAL_INT(0, return2);
	TEST_ASSERT_EQUAL_INT(0, return3);
	
}

/**
 * @brief Test for the resetRxBuffer function.
 * 
 * This test ensures that the resetRxBuffer function correctly clears the RX buffer.
 */
void test_resetRxBuffer(void){

	rxChar('#');
	rxChar('A');
	rxChar('8');

	resetRxBuffer();
	
	TEST_ASSERT_EQUAL_CHAR(0, UARTRxBuffer[0]);
	TEST_ASSERT_EQUAL_CHAR(0, UARTRxBuffer[1]);
	TEST_ASSERT_EQUAL_CHAR(0, UARTRxBuffer[2]);

}

/**
 * @brief Test for the resetTxBuffer function.
 * 
 * This test ensures that the resetTxBuffer function correctly clears the TX buffer.
 */
void test_resetTxBuffer(void){

	txChar('#');
	txChar('A');
	txChar('8');

	resetTxBuffer();
	
	TEST_ASSERT_EQUAL_CHAR(0, UARTTxBuffer[0]);
	TEST_ASSERT_EQUAL_CHAR(0, UARTTxBuffer[1]);
	TEST_ASSERT_EQUAL_CHAR(0, UARTTxBuffer[2]);

}

/**
 * @brief Test for the calcChecksum function.
 * 
 * This test ensures that the calcChecksum function correctly calculates the checksum 
 * of the RX buffer.
 */
void test_calcChecksum(void){
	

	rxChar('A');	
    
    int expectedChecksum1 = 65; 
    int checksum1 = calcChecksum((unsigned char *)UARTRxBuffer, 1);
    TEST_ASSERT_EQUAL_INT(expectedChecksum1, checksum1);
    resetRxBuffer();
    
    rxChar('T');
    rxChar('+');
    rxChar('2');
    rxChar('0');	
    
    int expectedChecksum2 = 225; 
    int checksum2 = calcChecksum((unsigned char *)UARTRxBuffer, 4);
    TEST_ASSERT_EQUAL_INT(expectedChecksum2, checksum2);
    
    
}

/**
 * @brief Test for the valid PT command processing.
 * 
 * This test ensures that the PT command is processed correctly and generates the 
 * expected response in the TX buffer.
 */
void test_valid_PT_command(void){

	rxChar('#');
	rxChar('P'); 
	rxChar('T'); 
	rxChar(164);
	rxChar('!');
	
	char buf[7] = {'#', 'T', '-', '1', '0', 226, '!'};
	
	TEST_ASSERT_EQUAL_INT(0, cmdProcessor());
	
	/* Verifica se todo o buffer foi preenchido corretamente */
	TEST_ASSERT_EQUAL_MEMORY(buf, UARTTxBuffer, sizeof(buf));
}

/**
 * @brief Test for the valid PH command processing.
 * 
 * This test ensures that the PH command is processed correctly and generates the 
 * expected response in the TX buffer.
 */
void test_valid_PH_command(void){

	rxChar('#');
	rxChar('P'); 
	rxChar('H'); 
	rxChar(152);
	rxChar('!');
	
	char buf[7] = {'#', 'H', '0', '2', '0', 218, '!'};
	
	TEST_ASSERT_EQUAL_INT(0, cmdProcessor());
	
	/* Verifica se todo o buffer foi preenchido corretamente */
	TEST_ASSERT_EQUAL_MEMORY(buf, UARTTxBuffer, sizeof(buf));

}

/**
 * @brief Test for the valid PC command processing.
 * 
 * This test ensures that the PC command is processed correctly and generates the 
 * expected response in the TX buffer.
 */
void test_valid_PC_command(void){

	rxChar('#');
	rxChar('P'); 
	rxChar('C'); 
	rxChar(147);
	rxChar('!');
	
	char buf[9] = {'#', 'C', '0', '0', '4', '0', '0', 55, '!'};
	
	TEST_ASSERT_EQUAL_INT(0, cmdProcessor());
	
	/* Verifica se todo o buffer foi preenchido corretamente */
	TEST_ASSERT_EQUAL_MEMORY(buf, UARTTxBuffer, sizeof(buf));
}


/**
 * @brief Test for the valid A command processing.
 * 
 * This test ensures that the A command is processed correctly and generates the 
 * expected response in the TX buffer.
 */
void test_valid_A_command(void){
	
	rxChar('#');
	rxChar('A');
	rxChar(65);
	rxChar('!');	
	
	/* Este é o valor esperado no buffer TX */
	char buf[17] = {'#', 'T', '-', '0', '8', 'H', '0', '2', '5', 'C', '0', '0', '5', '0', '0', 0, '!'};
	

	TEST_ASSERT_EQUAL_INT(0, cmdProcessor());

	/* Verifica se todo o buffer foi preenchido corretamente */
	TEST_ASSERT_EQUAL_MEMORY(buf, UARTTxBuffer, sizeof(buf));
}

/*verificar depopis pk nao concordo que o historico esteja no mudulo dos 
 * sensores ja que num sistema real os sensores nao gravam leituras, 
 * mas sim o microprocessador!!!!!
 */
 
 /**
 * @brief Test for the valid L command processing.
 * 
 * This test ensures that the L command is processed correctly and generates the 
 * expected response in the TX buffer after simulating multiple readings.
 */
void test_valid_L_command(void){
	
	/* Simular múltiplas leituras para gerar um histórico */
    for (int i = 0; i < 17; i++) {
        rxChar('#');
        rxChar('A');
        rxChar(65);
        rxChar('!');
        cmdProcessor();
        resetTxBuffer();
    }

    /* Enviar comando L */
    rxChar('#');
    rxChar('L');
    rxChar(76);
    rxChar('!');
    
       

   // Chama a função para imprimir o buffer UARTTxBuffer em hexadecimal
   // printBufferHex(UARTTxBuffer, sizeof(UARTTxBuffer));
      
      

	char buf[] = {
    '#', 'T', '-', '1', '0', 'H', '0', '2', '0', 'C', '0', '0', '4', '0', '0',0xF3,'!',
    '#', 'T', '-', '0', '8', 'H', '0', '2', '5', 'C', '0', '0', '5', '0', '0',0x00,'!',
    '#', 'T', '0', '0', '7', 'H', '0', '3', '0', 'C', '0', '0', '6', '0', '0',0xFF,'!',
    '#', 'T', '0', '0', '0', 'H', '0', '3', '5', 'C', '0', '0', '7', '0', '0',0xFE,'!',
    '#', 'T', '0', '0', '3', 'H', '0', '4', '0', 'C', '0', '0', '8', '0', '0',0xFE,'!',
    '#', 'T', '-', '0', '5', 'H', '0', '4', '5', 'C', '0', '0', '9', '0', '0',0x03,'!',
    '#', 'T', '0', '1', '0', 'H', '0', '5', '0', 'C', '0', '1', '0', '0', '0',0xF6,'!',
    '#', 'T', '0', '1', '2', 'H', '0', '5', '5', 'C', '0', '1', '1', '0', '0',0xFE,'!',
    '#', 'T', '0', '1', '5', 'H', '0', '6', '0', 'C', '0', '1', '2', '0', '0',0xFE,'!',
    '#', 'T', '0', '1', '8', 'H', '0', '6', '5', 'C', '0', '1', '3', '0', '0',0x07,'!',
    '#', 'T', '0', '2', '0', 'H', '0', '7', '0', 'C', '0', '1', '4', '0', '0',0xFD,'!',
    '#', 'T', '0', '2', '2', 'H', '0', '7', '5', 'C', '0', '1', '5', '0', '0',0x05,'!',
    '#', 'T', '0', '2', '4', 'H', '0', '8', '0', 'C', '0', '1', '6', '0', '0',0x04,'!',
    '#', 'T', '0', '2', '5', 'H', '0', '8', '5', 'C', '0', '1', '7', '0', '0',0x0B,'!',
    '#', 'T', '0', '2', '7', 'H', '0', '9', '0', 'C', '0', '1', '8', '0', '0',0x0A,'!',
    '#', 'T', '0', '2', '8', 'H', '0', '9', '5', 'C', '0', '1', '9', '0', '0',0x11,'!',
	'#', 'T', '0', '3', '0', 'H', '1', '0', '0', 'C', '0', '2', '0', '0', '0',0xF5,'!',
	'#', 'T', '0', '3', '2', 'H', '0', '9', '8', 'C', '0', '2', '1', '0', '0',0x08,'!',
	'#', 'T', '0', '3', '4', 'H', '0', '9', '6', 'C', '0', '2', '2', '0', '0',0x09,'!',
	'#', 'T', '0', '0', '0', 'H', '0', '0', '0', 'C', '0', '0', '0', '0', '0',0xEF,'!',
	};



    TEST_ASSERT_EQUAL_INT(0, cmdProcessor());
    
     

     //Verificar se a resposta do buffer TX é a esperada 
    TEST_ASSERT_EQUAL_MEMORY(buf, UARTTxBuffer, sizeof(buf));
}

/**
 * @brief Test for the valid R command processing.
 * 
 * This test ensures that the R command is processed correctly and generates the 
 * expected response in the TX buffer.
 */
void test_valid_R_command(void) {
    
    /* Enviar o comando R */
    rxChar('#');
    rxChar('R');
    rxChar(82);  // Código ASCII para 'R'
    rxChar('!');

    /* Buffer esperado (exemplo: resposta ao comando R) */
    char expectedBuffer[500] = {'#', 'D', 'O', 'N', 'E', 38, '!'};

    /* Chama o processador de comandos */
    TEST_ASSERT_EQUAL_INT(0, cmdProcessor());

    /* Verificar se o buffer TX contém o valor esperado */
    TEST_ASSERT_EQUAL_MEMORY(expectedBuffer, UARTTxBuffer, sizeof(expectedBuffer));

    
}

/**
 * @brief Test for invalid short commands.
 * 
 * This test ensures that a command with missing data (such as a checksum) is handled 
 * correctly by returning the appropriate error code.
 */
void test_invalid_short_command(void){
	
	rxChar('#');	//falta o checksum
	rxChar('A');
	rxChar('!');	
	
	TEST_ASSERT_EQUAL_INT(-1, cmdProcessor());

}

/**
 * @brief Test for invalid command with an incorrect delimiter.
 * 
 * This test ensures that commands with incorrect delimiters are handled correctly 
 * by returning the appropriate error code.
 */
void test_invalid_delimitator_command(void){
	
	rxChar('#');	//nao encontrou um dos delimitadores
	rxChar('A');
	rxChar(65);
	rxChar('#');	
		
	
	TEST_ASSERT_EQUAL_INT(-2, cmdProcessor());

}


/**
 * @brief Test for invalid checksum in command.
 * 
 * This test ensures that commands with an incorrect checksum are handled correctly 
 * by returning the appropriate error code.
 */
void test_invalid_checksum_command(void){
	
	rxChar('#');	
	rxChar('A');
	rxChar(60);
	rxChar('!');	
		
	
	TEST_ASSERT_EQUAL_INT(-3, cmdProcessor());

}

/**
 * @brief Test for invalid command.
 * 
 * This test ensures that commands that are not recognized are handled correctly 
 * by returning the appropriate error code.
 */
void test_invalid_command(void){
	
	rxChar('#');	
	rxChar('B');
	rxChar(66);
	rxChar('!');	
		
	
	TEST_ASSERT_EQUAL_INT(-4, cmdProcessor());

}







/**
 * @brief Main function to run Unity test framework.
 * 
 * The main function executes all the test cases defined above. It begins the 
 * Unity test session, runs each test, and then ends the session.
 * 
 * @return Returns the result of the Unity tests.
 */
int main(void)
{
	UNITY_BEGIN();
	/* rxChar */
	RUN_TEST(test_rxChar);
	
	/* txChar */
	RUN_TEST(test_txChar);
	
	/* resetTxBuffer */
	RUN_TEST(test_resetTxBuffer);
	
	/* resetRxBuffer */
	RUN_TEST(test_resetRxBuffer);
	
	/* calcChecksum */
	RUN_TEST(test_calcChecksum);
	
	
	/* cmdProcessor */
	RUN_TEST(test_valid_PT_command);
	RUN_TEST(test_valid_PH_command);
	RUN_TEST(test_valid_PC_command);
	RUN_TEST(test_valid_A_command);
	RUN_TEST(test_valid_L_command);
	RUN_TEST(test_valid_R_command);
	RUN_TEST(test_invalid_short_command);
	RUN_TEST(test_invalid_delimitator_command);
	RUN_TEST(test_invalid_checksum_command);
	RUN_TEST(test_invalid_command);
	
	
	
	return UNITY_END();
}
