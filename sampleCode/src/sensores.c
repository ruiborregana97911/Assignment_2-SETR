/** @file sensores.c
 *  @brief Implementation of sensor simulation functions.
 * 
 *  This file provides the implementation of functions to simulate the reading of
 *  temperature, humidity, and CO2 sensors.
 * 
 *  The sensor values are generated from predefined arrays for simulation purposes.
 * 
 *  @authors Henrique Ferreira  
 *           Rui Borregana  
 *  @date 5 Apr 2025
 *  @bug No known bugs
 */
 
#include <stdio.h>
#include <string.h>

#include "sensores.h"



// Funções para ler os sensores
int readTemperature(void) {
    int value = temperatureData[tempIndex];
    tempIndex = (tempIndex + 1) % SENSOR_DATA_SIZE;
    return value;
}

int readHumidity(void) {
    int value = humidityData[humIndex];
    humIndex = (humIndex + 1) % SENSOR_DATA_SIZE;
    return value;
}

int readCO2(void) {
    int value = co2Data[co2Index];
    co2Index = (co2Index + 1) % SENSOR_DATA_SIZE;
    return value;
}
