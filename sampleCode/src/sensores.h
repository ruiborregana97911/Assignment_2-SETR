////////////////////////////////////////////////////outro .h
/**
 * @file sensores.h
 * @brief Header file for sensor data management.
 */
  
#ifndef SENSORES_H_
#define SENSORES_H_
 
#define SENSOR_DATA_SIZE 25


/*coloquei isto, __attribute__((unused)) , para informar o compilador que
 * essas variaveis nao sao usadas intencinalmente, isto so para parar de 
 * aparecer esses warnings !!!! 
*/
// Vetores para armazenar valores fictícios dos sensores

/** 
 * @brief Simulated temperature sensor data 
 */
static int __attribute__((unused)) temperatureData[SENSOR_DATA_SIZE] = {
    -10, -8, +7, 0, +3, -5, +10, +12, +15, +18,
    +20, +22, +24, +25, +27, +28, +30, +32, +34, +35,
    +36, +37, +38, +39, +40
};

/** 
 * @brief Simulated humidity sensor data 
 */
static int __attribute__((unused)) humidityData[SENSOR_DATA_SIZE] = {
    20, 25, 30, 35, 40, 45, 50, 55, 60, 65,
    70, 75, 80, 85, 90, 95, 100, 98, 96, 94,
    92, 90, 88, 86, 84
};

/** 
 * @brief Simulated CO2 sensor data 
 */
static int __attribute__((unused)) co2Data[SENSOR_DATA_SIZE] = {
    400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300,
    1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300,
    2400, 2500, 2600, 2700, 2800
};

static int __attribute__((unused)) tempIndex = 0;
static int __attribute__((unused)) humIndex = 0;
static int __attribute__((unused)) co2Index = 0;

// Funções para ler os sensores, armazenar no histórico e atualizar os índices

/**
 * @brief Reads the next temperature value and updates history.
 * @return Current temperature reading.
 */
int readTemperature(void);

/**
 * @brief Reads the next humidity value and updates history.
 * @return Current humidity reading.
 */
int readHumidity(void);

/**
 * @brief Reads the next CO2 value and updates history.
 * @return Current CO2 reading.
 */
int readCO2(void);


#endif
