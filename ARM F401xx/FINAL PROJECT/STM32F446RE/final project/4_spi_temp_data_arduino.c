#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f446xx_usart_driver.h"
#include "dht11_driver.h"
#include "stm32f446xx_i2c_driver.h"
#include "stm32f446xx_spi_driver.h"
#include "ssd1306.h"

// Global Handles
USART_Handle_t usart2 = {0};
I2C_Handle_t i2c1_handle = {0};
SPI_Handle_t spi1_handle = {0}; // Added SPI Handle

void GPIO_Manual_Init(void) {
    GPIOA_CLK_EN();
    GPIOB_CLK_EN();

    usart2_GPIO_PinSetUp();

    // --- I2C PINS (PB8, PB9) ---
    GPIO_handle_t i2c_pins = {0};
    i2c_pins.pGPIOx = GPIOB;
    i2c_pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    i2c_pins.GPIO_PinConfig.GPIO_PinAltFunction = 4; // AF4
    i2c_pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OPENDRAIN;
    i2c_pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PULLUP;
    i2c_pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    i2c_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
    GPIO_Init(&i2c_pins);
    i2c_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
    GPIO_Init(&i2c_pins);

    // --- SPI1 PINS (PA4, PA5, PA6, PA7) ---
    GPIO_handle_t spi_pins = {0};
    spi_pins.pGPIOx = GPIOA;
    spi_pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    spi_pins.GPIO_PinConfig.GPIO_PinAltFunction = 5; // AF5 for SPI1
    spi_pins.GPIO_PinConfig.GPIO_PinOPType = 0; // Push-Pull (default)
    spi_pins.GPIO_PinConfig.GPIO_PinPuPdControl = 0; // No Pull-up/Pull-down
    spi_pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    // SCK (Clock)
    spi_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    GPIO_Init(&spi_pins);
    // MISO (Master In Slave Out)
    spi_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
    GPIO_Init(&spi_pins);
    // MOSI (Master Out Slave In)
    spi_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
    GPIO_Init(&spi_pins);

    // NSS (Chip Select) - We control this manually as a standard Output pin
    spi_pins.GPIO_PinConfig.GPIO_PinMode = 1; // Standard Output Mode (GPIO_MODE_OUT)
    spi_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_4;
    GPIO_Init(&spi_pins);

    // Set Chip Select HIGH initially (Idle state)
    // Assuming your GPIO driver has a basic write function like this:
    // GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_4, 1);
}

void SPI1_Setup(void) {
    spi1_handle.pSPIx = SPI1;
    spi1_handle.SPI_Config.SPI_Device_Mode = SPI_DEVICE_MODE_MASTER;
    spi1_handle.SPI_Config.SPI_Bus_Config = SPI_BUS_CONFIG_FD;      // Full Duplex
    spi1_handle.SPI_Config.SPI_DataFrame = SPI_DFF_8BIT;            // 8-bit characters
    spi1_handle.SPI_Config.SPI_CPHA = SPI_CPHA_LOW;                 // Standard Mode 0
    spi1_handle.SPI_Config.SPI_CPOL = SPI_CPOL_LOW;                 // Standard Mode 0
    spi1_handle.SPI_Config.SPI_SSM = SPI_SSM_EN;                    // Software Slave Management
    spi1_handle.SPI_Config.SPI_CLK_Speed = SPI_PRESCALAR_128;       // Slow speed ensures Arduino can keep up

    SPI_Init(&spi1_handle);
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

    USART_Transmit_String(USART2, "\r\n=== SENSOR TO OLED & ARDUINO ===\r\n");

    // 2. I2C & SPI Init
    i2c1_handle.pI2Cx = I2C1;
    i2c1_handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
    i2c1_handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
    I2C_Init(&i2c1_handle);

    SPI1_Setup();
    SPI_PeripheralControl(SPI1, ENABLE); // Turn on SPI hardware

    // 3. OLED Init
    SSD1306_Init(&i2c1_handle);
    SSD1306_Fill(&i2c1_handle, 0x00);

    // 4. DHT11 Init
    DHT11_Init();
    DHT11_Data_t sensorData = {0};

    char display_buffer[32];

    while(1)
    {
        for(uint32_t i = 0; i < 2000000; i++); // ~2 sec delay

        if(DHT11_Read(&sensorData) == DHT11_OK)
        {
            sprintf(display_buffer, "Temp:%d.%dC", sensorData.temp_int, sensorData.temp_dec);

            // --- DRAW TO OLED ---
            SSD1306_SetCursor(&i2c1_handle, 0, 2);
            SSD1306_WriteString(&i2c1_handle, "                  ");
            SSD1306_SetCursor(&i2c1_handle, 0, 2);
            SSD1306_WriteString(&i2c1_handle, display_buffer);


            // 1. Pull Chip Select LOW to wake up the Arduino
            // GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_4, 0);

            // 2. Transmit the string + 1 extra byte for the null terminator '\0'
            uint8_t datalen = strlen(display_buffer) + 1;
            SPI_Transmit_Buffer(SPI1, (uint8_t*)display_buffer, datalen);

            // 3. Pull Chip Select HIGH to end transmission
            // GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_4, 1);
        }
        else
        {
            USART_Transmit_String(USART2, "Sensor Error!\r\n");
            SSD1306_SetCursor(&i2c1_handle, 0, 2);
            SSD1306_WriteString(&i2c1_handle, "Sensor Error!     ");
        }
    }
}
