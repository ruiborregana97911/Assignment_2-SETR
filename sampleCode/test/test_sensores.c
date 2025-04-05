/** @file test_sensores.c
 *  @brief Unit tests for functions defined in sensores.h using Unity framework.
 * 
 *  This file contains a set of unit tests to validate the behavior of the sensor functions,
 *  including reading simulated temperature, humidity, and CO2 data.
 * 
 *  The tests are implemented using the Unity testing framework.
 * 
 *  @authors Henrique Ferreira  
 *           Rui Borregana  
 *  @date 5 Apr 2025
 *  @bug No known bugs
 */
#include <unity.h>
#include "sensores.h"

/**
 * @brief Setup function for Unity tests.
 * This function is called before each test to initialize the test environment.
 */
void setUp(void)
{

}

/**
 * @brief Tear down function for Unity tests.
 * This function is called after each test to clean up.
 */
void tearDown(void)
{
	
}

/**
 * @brief Test for the readTemperature function.
 * This test ensures that the readTemperature function correctly reads the temperature
 * values from the predefined data and stores them in the history.
 */
void test_readTemperature(void){
	int first_value = readTemperature();
	TEST_ASSERT_EQUAL_INT(-10, first_value);
	
	int second_value = readTemperature();
	TEST_ASSERT_EQUAL_INT(-8, second_value);
	
	int third_value = readTemperature();
	TEST_ASSERT_EQUAL_INT(7, third_value);
			
}

/**
 * @brief Test for the readHumidity function.
 * This test ensures that the readHumidity function correctly reads the humidity
 * values from the predefined data and stores them in the history.
 */
void test_readHumidity(void){
	int first_value = readHumidity();
	TEST_ASSERT_EQUAL_INT(20, first_value);
	
	int second_value = readHumidity();
	TEST_ASSERT_EQUAL_INT(25, second_value);		
}

/**
 * @brief Test for the readCO2 function.
 * This test ensures that the readCO2 function correctly reads the CO2
 * values from the predefined data and stores them in the history.
 */
void test_readCO2(void){
	int first_value = readCO2();
	TEST_ASSERT_EQUAL_INT(400, first_value);
	
	int second_value = readCO2();
	TEST_ASSERT_EQUAL_INT(500, second_value);		
}

/**
 * @brief Test for the clearSensorHistory function.
 * This test ensures that the clearSensorHistory function correctly clears the 
 * history arrays of all sensors after reading sensor data.

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
} */

/**
 * @brief Main function to run the Unity test framework.
 * The main function executes all the test cases defined above.
 * 
 * @return Returns the result of Unity tests.
 */
int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_readTemperature);
	RUN_TEST(test_readHumidity);
	RUN_TEST(test_readCO2);
	return UNITY_END();
}
