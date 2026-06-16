#ifndef DHT11_DRIVER_H_
#define DHT11_DRIVER_H_

#include "stm32f401xx.h"
#include "STM32F401XX_gpio_driver.h"

// Struct to hold the sensor readings
typedef struct {
    uint8_t temp_int;
    uint8_t temp_dec;
    uint8_t hum_int;
    uint8_t hum_dec;
} DHT11_Data_t;

// Status Codes
#define DHT11_OK               0
#define DHT11_ERROR_TIMEOUT    1
#define DHT11_ERROR_CHECKSUM   2

// Function Prototypes
void DHT11_Init(void);
uint8_t DHT11_Read(DHT11_Data_t *SensorData);

#endif /* DHT11_DRIVER_H_ */
