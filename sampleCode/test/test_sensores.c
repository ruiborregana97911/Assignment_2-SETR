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




int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_readTemperature);
	RUN_TEST(test_readHumidity);
	RUN_TEST(test_readCO2);
	return UNITY_END();
}
