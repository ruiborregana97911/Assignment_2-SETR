#include <unity.h>
#include "cmdproc.h"
#include "sensores.h"


void setUp(void)
{
	resetTxBuffer();
	resetRxBuffer();	
	
}
void tearDown(void)
{
	
}


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

void test_resetRxBuffer(void){

	rxChar('#');
	rxChar('A');
	rxChar('8');

	resetRxBuffer();
	
	TEST_ASSERT_EQUAL_CHAR(0, UARTRxBuffer[0]);
	TEST_ASSERT_EQUAL_CHAR(0, UARTRxBuffer[1]);
	TEST_ASSERT_EQUAL_CHAR(0, UARTRxBuffer[2]);

}

void test_resetTxBuffer(void){

	txChar('#');
	txChar('A');
	txChar('8');

	resetTxBuffer();
	
	TEST_ASSERT_EQUAL_CHAR(0, UARTTxBuffer[0]);
	TEST_ASSERT_EQUAL_CHAR(0, UARTTxBuffer[1]);
	TEST_ASSERT_EQUAL_CHAR(0, UARTTxBuffer[2]);

}




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


void test_valid_PT_command(void){

	rxChar('#');
	rxChar('P'); 
	rxChar('T'); 
	rxChar(164);
	rxChar('!');
	
	char buf[7] = {'#', 'T', '-', '1', '0', 226, '!'};
	
	TEST_ASSERT_EQUAL_INT(0, cmdProcessor());
	
	for(int i=0; i< 7;i++){
		TEST_ASSERT_EQUAL_CHAR(buf[i], UARTTxBuffer[i]);	
	}
}

void test_valid_PH_command(void){

	rxChar('#');
	rxChar('P'); 
	rxChar('H'); 
	rxChar(152);
	rxChar('!');
	
	char buf[7] = {'#', 'H', '0', '2', '0', 218, '!'};
	
	TEST_ASSERT_EQUAL_INT(0, cmdProcessor());
	
	for(int i=0; i< 7;i++){
		TEST_ASSERT_EQUAL_CHAR(buf[i], UARTTxBuffer[i]);	
	}

}

void test_valid_PC_command(void){

	rxChar('#');
	rxChar('P'); 
	rxChar('C'); 
	rxChar(147);
	rxChar('!');
	
	char buf[9] = {'#', 'C', '0', '0', '4', '0', '0', 55, '!'};
	
	TEST_ASSERT_EQUAL_INT(0, cmdProcessor());
	
	for(int i=0; i< 9;i++){
		TEST_ASSERT_EQUAL_CHAR(buf[i], UARTTxBuffer[i]);	
	}

}

void test_valid_A_command(void){
	
	rxChar('#');
	rxChar('A');
	rxChar(65);
	rxChar('!');	
	
	/*é este valor o proximo por causa de se chamar primeiro antes os 
	 * comandos P individualmente para sensor, logo nesta chamada os 
	 * proximos valores serao os que estao em segundo lugar!*/
	char buf[17] = {'#', 'T', '-', '0', '8', 'H', '0', '2', '5', 'C', '0', '0', '5', '0', '0', 0, '!'};
	
	TEST_ASSERT_EQUAL_INT(0, cmdProcessor());
	
	for(int i=0; i< 17;i++){
		TEST_ASSERT_EQUAL_CHAR(buf[i], UARTTxBuffer[i]);	
	}
	

}

/*verificar depopis pk nao concordo que o historico esteja no mudulo dos 
 * sensores ja que num sistema real os sensores nao gravam leituras, 
 * mas sim o microprocessador!!!!!
 * 
void test_valid_L_command(void){
	
	
	for (int i = 0; i < 18; i++) {
		rxChar('#');
		rxChar('A');
		rxChar(65);
		rxChar('!');
		
		
		cmdProcessor();
		resetTxBuffer();
	}	
	
}*/

void test_invalid_short_command(void){
	
	rxChar('#');	//falta o checksum
	rxChar('A');
	rxChar('!');	
	
	TEST_ASSERT_EQUAL_INT(-1, cmdProcessor());

}

void test_invalid_delimitator_command(void){
	
	rxChar('#');	//nao encontrou um dos delimitadores
	rxChar('A');
	rxChar(65);
	rxChar('#');	
		
	
	TEST_ASSERT_EQUAL_INT(-2, cmdProcessor());

}

void test_invalid_checksum_command(void){
	
	rxChar('#');	
	rxChar('A');
	rxChar(60);
	rxChar('!');	
		
	
	TEST_ASSERT_EQUAL_INT(-3, cmdProcessor());

}

void test_invalid_command(void){
	
	rxChar('#');	
	rxChar('B');
	rxChar(66);
	rxChar('!');	
		
	
	TEST_ASSERT_EQUAL_INT(-4, cmdProcessor());

}

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
	//RUN_TEST(test_valid_L_command);	ir ver comentario acima!
	RUN_TEST(test_invalid_short_command);
	RUN_TEST(test_invalid_delimitator_command);
	RUN_TEST(test_invalid_checksum_command);
	RUN_TEST(test_invalid_command);
	
	
	
	return UNITY_END();
}
