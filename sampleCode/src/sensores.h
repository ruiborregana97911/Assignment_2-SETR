/** @file sensores.h
 *  @brief Header file for sensor simulation functions and data structures.
 * 
 *  This file declares the functions and defines constants used for simulating sensor readings 
 *  (temperature, humidity, and CO2). It also contains global arrays that represent fake sensor data 
 *  along with index variables to track readings.
 * 
 *  The purpose of this file is to support testing and simulation in embedded systems or software 
 *  environments without real hardware sensors.
 * 
 *  @authors Henrique Ferreira  
 *           Rui Borregana  
 *  @date 5 Apr 2025
 *  @bug No known bugs
 */
  
#ifndef SENSORES_H_
#define SENSORES_H_
 
 /**
 * @def SENSOR_DATA_SIZE
 * @brief Size of the sensor data array.
 */
#define SENSOR_DATA_SIZE 25

// Vetores para armazenar valores fictícios dos sensores

/** 
 * @var static int temperatureData[SENSOR_DATA_SIZE]
 * @brief Simulated temperature sensor data 
 */
static int __attribute__((unused)) temperatureData[SENSOR_DATA_SIZE] = {
    -10, -8, +7, 0, +3, -5, +10, +12, +15, +18,
    +20, +22, +24, +25, +27, +28, +30, +32, +34, +35,
    +36, +37, +38, +39, +40
};

/** 
 * @var static int humidityData[SENSOR_DATA_SIZE]
 * @brief Simulated humidity sensor data 
 */
static int __attribute__((unused)) humidityData[SENSOR_DATA_SIZE] = {
    20, 25, 30, 35, 40, 45, 50, 55, 60, 65,
    70, 75, 80, 85, 90, 95, 100, 98, 96, 94,
    92, 90, 88, 86, 84
};

/** 
 * @var static int co2Data[SENSOR_DATA_SIZE]
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
 * @brief readTemperature reads the next temperature.
 * @return Current temperature reading.
 */
int readTemperature(void);

/**
 * @brief readHumidity reads the next humidity value.
 * @return Current humidity reading.
 */
int readHumidity(void);

/**
 * @brief readCO2 reads the next CO2 value.
 * @return Current CO2 reading.
 */
int readCO2(void);


#endif
