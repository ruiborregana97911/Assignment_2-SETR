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


void test_calcChecksum(void){
	

	rxChar('A');	
    
    int expectedChecksum1 = 65; 
    int checksum1 = calcChecksum((unsigned char *)UARTRxBuffer, 1);
    TEST_ASSERT_EQUAL_INT(expectedChecksum1, checksum1);
    resetTxBuffer();
    
    rxChar('T');
    rxChar('+');
    rxChar('2');
    rxChar('0');	
    
    int expectedChecksum2 = 225; 
    int checksum2 = calcChecksum((unsigned char *)UARTRxBuffer, 4);
    TEST_ASSERT_EQUAL_INT(expectedChecksum2, checksum2);
    resetTxBuffer();
    
}





void test_valid_PT_command(void){

	rxChar('#');
	rxChar('P'); 
	rxChar('T'); 
	rxChar(164);
	rxChar('!');
	
	
	TEST_ASSERT_EQUAL_INT(0, cmdProcessor());
	
}

void test_valid_PH_command(void){

	rxChar('#');
	rxChar('P'); 
	rxChar('H'); 
	rxChar(152);
	rxChar('!');
	
	
	TEST_ASSERT_EQUAL_INT(0, cmdProcessor());
	
}

void test_valid_PC_command(void){

	rxChar('#');
	rxChar('P'); 
	rxChar('C'); 
	rxChar(147);
	rxChar('!');
	
	
	TEST_ASSERT_EQUAL_INT(0, cmdProcessor());
	
}




int main(void)
{
	UNITY_BEGIN();
	/* rxChar */
	RUN_TEST(test_rxChar);
	
	
	/* calcChecksum */
	//RUN_TEST(test_calcChecksum);
	
	
	/* cmdProcessor */
	RUN_TEST(test_valid_PT_command);
	RUN_TEST(test_valid_PH_command);
	RUN_TEST(test_valid_PC_command);
	
	return UNITY_END();
}
