#include <stdint.h>
#include <stdio.h>
#include "stm32f446xx.h"
#include "stm32f446xx_usart_driver.h"
#include "stm32f446xx_i2c_driver.h"
#include "dht11_driver.h"
#include "ssd1306.h"

// Global Handles
USART_Handle_t usart2 = {0};
I2C_Handle_t i2c1_handle = {0};

void GPIO_Manual_Init(void) {

    GPIOA_PCLK_EN();
    GPIOB_PCLK_EN();

    // 2. Setup USART Pins (PA2, PA3)
    usart2_GPIO_PinSetUp();

    // 3. Setup I2C Pins (PB8=SCL, PB9=SDA)
    GPIO_Handle_t i2c_pins = {0};
    i2c_pins.pGPIOx = GPIOB;
    i2c_pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    i2c_pins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
    i2c_pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    i2c_pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    i2c_pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    i2c_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
    GPIO_Init(&i2c_pins);

    i2c_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
    GPIO_Init(&i2c_pins);
}

int main(void)
{
    GPIO_Manual_Init();

    // 1. USART Init
    usart2.pUSARTx = USART2;
    usart2.USART_Config.USART_Baud = USART_STD_BAUD_9600;
    usart2.USART_Config.USART_Mode = USART_MODE_TX_ONLY;
    USART_Init(&usart2);
    USART_PeripheralControl(USART2, ENABLE);

    USART_Transmit_String(&usart2, "\r\n=== F446 SENSOR TO OLED ===\r\n");

    // 2. I2C Init
    i2c1_handle.pI2Cx = I2C1;
    i2c1_handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
    i2c1_handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
    i2c1_handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
    I2C_Init(&i2c1_handle);

    // 3. OLED Init
    SSD1306_Init(&i2c1_handle);
    SSD1306_Fill(&i2c1_handle, 0x00); // Clear to black

    // 4. DHT11 Init
    DHT11_Init();
    DHT11_Data_t sensorData = {0};

    char display_buffer[32];

    while(1)
    {
        // Wait ~2 seconds
        for(uint32_t i = 0; i < 2000000; i++);

        if(DHT11_Read(&sensorData) == DHT11_OK)
        {
            sprintf(display_buffer, "Temp: %d.%d C", sensorData.temp_int, sensorData.temp_dec);

            // Print to Serial Monitor
            USART_Transmit_String(&usart2, display_buffer);
            USART_Transmit_String(&usart2, "\r\n");

            // --- DRAW TO OLED ---
            SSD1306_SetCursor(&i2c1_handle, 0, 2);
            SSD1306_WriteString(&i2c1_handle, "                  ");

            SSD1306_SetCursor(&i2c1_handle, 0, 2);
            SSD1306_WriteString(&i2c1_handle, display_buffer);

        }
        else
        {
            USART_Transmit_String(&usart2, "Sensor Error!\r\n");
            SSD1306_SetCursor(&i2c1_handle, 0, 2);
            SSD1306_WriteString(&i2c1_handle, "Sensor Error!     ");
        }
    }
}
