#include <unity.h>
#include "sensores.h"


void setUp(void)
{
	clearSensorHistory();		
	//return;
}
void tearDown(void)
{
	//return;
}

void test_readTemperature(void){
	int first_value = readTemperature();
	TEST_ASSERT_EQUAL_INT(-10, first_value);
	
	int second_value = readTemperature();
	TEST_ASSERT_EQUAL_INT(-8, second_value);
	
	int third_value = readTemperature();
	TEST_ASSERT_EQUAL_INT(7, third_value);
			
}

void test_readHumidity(void){
	int first_value = readHumidity();
	TEST_ASSERT_EQUAL_INT(20, first_value);
	
	int second_value = readHumidity();
	TEST_ASSERT_EQUAL_INT(25, second_value);		
}

void test_readCO2(void){
	int first_value = readCO2();
	TEST_ASSERT_EQUAL_INT(400, first_value);
	
	int second_value = readCO2();
	TEST_ASSERT_EQUAL_INT(500, second_value);		
}

void test_clearSensorHistory(void){
	readTemperature();
	readHumidity();
	readCO2();
	clearSensorHistory();
	
	for(int i=0;i<HISTORY_SIZE;i++){
		TEST_ASSERT_EQUAL_INT(0, temperatureHistory[i]);
		TEST_ASSERT_EQUAL_INT(0, humidityHistory[i]);
		TEST_ASSERT_EQUAL_INT(0, co2History[i]);
		
	}		
}


int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_readTemperature);
	RUN_TEST(test_readHumidity);
	RUN_TEST(test_readCO2);
	RUN_TEST(test_clearSensorHistory);
	return UNITY_END();
}
