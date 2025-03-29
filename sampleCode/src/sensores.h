////////////////////////////////////////////////////outro .h
 #ifndef SENSORES_H_
#define SENSORES_H_
 
#define SENSOR_DATA_SIZE 25
#define HISTORY_SIZE 20

/*coloquei isto, __attribute__((unused)) , para informar o compilador que
 * essas variaveis nao sao usadas intencinalmente, isto so para parar de 
 * aparecer esses warnings !!!! 
*/
// Vetores para armazenar valores fictícios dos sensores
static int __attribute__((unused)) temperatureData[SENSOR_DATA_SIZE] = {
    -10, -8, -5, 0, 3, 7, 10, 12, 15, 18,
    20, 22, 24, 25, 27, 28, 30, 32, 34, 35,
    36, 37, 38, 39, 40
};

static int __attribute__((unused)) humidityData[SENSOR_DATA_SIZE] = {
    20, 25, 30, 35, 40, 45, 50, 55, 60, 65,
    70, 75, 80, 85, 90, 95, 100, 98, 96, 94,
    92, 90, 88, 86, 84
};

static int __attribute__((unused)) co2Data[SENSOR_DATA_SIZE] = {
    400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300,
    1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300,
    2400, 2500, 2600, 2700, 2800
};


// Históricos dos últimos 20 valores lidos
static int __attribute__((unused)) temperatureHistory[HISTORY_SIZE] = {0};
static int __attribute__((unused)) humidityHistory[HISTORY_SIZE] = {0};
static int __attribute__((unused)) co2History[HISTORY_SIZE] = {0};

// Índices de leitura e histórico
static int __attribute__((unused)) tempIndex = 0, tempHistIndex = 0;
static int __attribute__((unused)) humIndex = 0, humHistIndex = 0;
static int __attribute__((unused)) co2Index = 0, co2HistIndex = 0;

//////////////////////////////////////////////////////////////mesmo .h

////////////////////////////////////////////////////////////////////////////////// outro .h




// Funções para ler os sensores, armazenar no histórico e atualizar os índices
int readTemperature(void);

int readHumidity(void);

int readCO2(void);

/*
 * clearSensorHistory
 * Função para limpar o histórico dos sensores
 */
void clearSensorHistory(void);

#endif
