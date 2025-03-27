#include <stdio.h>
#include <string.h>

#include "sensores.h"



//Funções para ler os sensores, armazenar no histórico e atualizar os índices
int readTemperature() {
    int value = temperatureData[tempIndex];
    temperatureHistory[tempHistIndex] = value;
    tempHistIndex = (tempHistIndex + 1) % HISTORY_SIZE;
    tempIndex = (tempIndex + 1) % SENSOR_DATA_SIZE;
    return value;
}

int readHumidity() {
    int value = humidityData[humIndex];
    humidityHistory[humHistIndex] = value;
    humHistIndex = (humHistIndex + 1) % HISTORY_SIZE;
    humIndex = (humIndex + 1) % SENSOR_DATA_SIZE;
    return value;
}

int readCO2() {
    int value = co2Data[co2Index];
    co2History[co2HistIndex] = value;
    co2HistIndex = (co2HistIndex + 1) % HISTORY_SIZE;
    co2Index = (co2Index + 1) % SENSOR_DATA_SIZE;
    return value;
}
/*
 * clearSensorHistory
 * Função para limpar o histórico dos sensores
 */
void clearSensorHistory() {
    memset(temperatureHistory, 0, sizeof(temperatureHistory));
    memset(humidityHistory, 0, sizeof(humidityHistory));
    memset(co2History, 0, sizeof(co2History));

    tempHistIndex = 0;
    humHistIndex = 0;
    co2HistIndex = 0;
}
