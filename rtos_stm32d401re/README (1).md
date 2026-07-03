# STM32 FreeRTOS 3-LED Multi-Tasker

**Author:** Adler Nunez

This project demonstrates a fundamental implementation of **FreeRTOS** on an STM32 microcontroller. It uses the FreeRTOS scheduler to safely manage three independent tasks, each responsible for toggling a specific LED at RTOS-safe intervals without blocking the CPU.

## 🛠️ Hardware Requirements

* **Microcontroller:** STM32F401RE (Nucleo-64 Board)
* **Onboard Component:** 1x Green LED (LD2) internally routed to pin `PA5`
* **External Components:**
  * 1x Red LED (connected to `red_led_ext_Pin` on GPIOA) `PA6`           
  * 1x Green LED (connected to `green_led_ext_Pin` on GPIOA) `PA7`


## 💻 Software & Tools

* **IDE:** STM32CubeIDE
* **RTOS:** FreeRTOS 
* **Configuration:** STM32CubeMX (84 MHz System Clock, TIM4 configured for HAL Timebase)

## 🧠 Core Task Logic

```c
/* USER CODE BEGIN 4 */
static void led_green_handler( void * parameters )
{
    while(1)
    {
        // Toggle the onboard Green LED (LD2)
    	printf("task1\n");
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
       HAL_Delay(1000);
       taskYIELD();
    }
}
static void led_red_handlerext( void * parameters )
{
    while(1)
    {
        // Toggle the external Red LED
    	printf("task2\n");
        HAL_GPIO_TogglePin(red_led_ext_GPIO_Port, red_led_ext_Pin);
        HAL_Delay(500);
        taskYIELD();
    }
}
static void led_green_handlerext( void * parameters )
{
    while(1)
    {
        // Toggle the external Green LED
    	printf("task3\n");
        HAL_GPIO_TogglePin(green_led_ext_GPIO_Port, green_led_ext_Pin);
        HAL_Delay(750);
        taskYIELD();
    }
}
/* USER CODE END 4 */
```

## 📂 Project Links

* **Source Code (GitHub):** https://github.com/adlernunez/Adler-Entri/tree/3df98d48816bbdc2a0de847ae18b9094aae3eff6/rtos_stm32d401re
* **Project Video (Google Drive):** https://drive.google.com/drive/folders/1D7-ofOIOUmrdLSwfGsz7y-27M_Nt6UHO
