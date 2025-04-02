/**
 * @file sensores.c
 * @brief Implementation of sensor data management.
 */
 
#include <stdio.h>
#include <string.h>

#include "sensores.h"



/// Funções para ler os sensores
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
