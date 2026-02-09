/*
 * STM32F401XX_gpio_driver.c
 *
 *  Created on: 01-Feb-2026
 *      Author: adler
 */


#include "STM32F401XX_gpio_driver.h"

//GPIO_PeriClockControl(GPIOA, ENABLE);
/*************************************
 * @FN 			- GPIO_PERICLOCKCONTROL
 * @BRIEF		- This fn enables or disables peripheral clock for the GPIO port
 * @param(in)	- pGPIOx: base address of the GPIO peripheral (GPIOA, GPIOB....GPIOH)
 * @PARAM(in)	- EnorDi: ENABLE OR DISABLE macros
 * @return		- none
 * @note		- none
 *************************************/

void GPIO_PeriClockControl(GPIO_RegDef_t*pGPIOx, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
		{
			if(pGPIOx == GPIOA)
			{
				GPIOA_CLK_EN();

			}
			else if (pGPIOx == GPIOB)
			{
				GPIOB_CLK_EN();

			}
			else if (pGPIOx == GPIOC)
			{
				GPIOC_CLK_EN();

			}
			else if (pGPIOx == GPIOD)
			{
				GPIOD_CLK_EN();

			}
			else if (pGPIOx == GPIOE)
			{
				GPIOE_CLK_EN();

			}
			else if (pGPIOx == GPIOH)
			{
				GPIOH_CLK_EN();

			}
		}
		else
		{

			if(pGPIOx == GPIOA)
			{
				GPIOA_CLK_DI();

			}
			else if (pGPIOx == GPIOB)
			{
				GPIOB_CLK_DI();

			}
			else if (pGPIOx == GPIOC)
			{
				GPIOC_CLK_DI();

			}
			else if (pGPIOx == GPIOD)
			{
				GPIOD_CLK_DI();

			}
			else if (pGPIOx == GPIOE)
			{
				GPIOE_CLK_DI();

			}
			else if (pGPIOx == GPIOH)
			{
				GPIOH_CLK_DI();

			}
		}

}


void GPIO_Init(GPIO_handle_t *pGPIOHandle){


	uint32_t temp=0;

	//enable the peripheral clock
	GPIO_PeriClockControl(pGPIOHandle -> pGPIOx, ENABLE);

	//GPIO MODE// Configure mode : 32 bit register, 2 bits for each pin
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode<=GPIO_MODE_ANALOG)
	{

		temp =((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode) << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));// to create temporary mask
		pGPIOHandle->pGPIOx->MODER &=~(0x3<<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));			// to clear the register
		pGPIOHandle->pGPIOx->MODER|=temp;	//set the value of register
		temp=0;
	}

	else// interrupt modes
	{
		}

	// speed
		temp =((pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed) << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));// to create temporary mask
		pGPIOHandle->pGPIOx->OSPEEDR &=~(0x3<<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));			// to clear the register
		pGPIOHandle->pGPIOx->OSPEEDR|=temp;	//set the value of register


	//PUPD
		temp =((pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl) << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));// to create temporary mask
		pGPIOHandle->pGPIOx->PUPDR &=~(0x3<<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));			// to clear the register
		pGPIOHandle->pGPIOx->PUPDR|=temp;	//set the value of register

	//OPTYPE
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);  // Clear 1 bit
		pGPIOHandle->pGPIOx->OTYPER|=temp;	//set the value of register

	//AF
	if((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode)<= GPIO_MODE_ALTFN)
	{
		uint8_t temp1,temp2;

		temp1= (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)/8;// 0 for pins0-7, 1 for pins8-15
		temp2 =(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)%8;// common value to set bit positions
		pGPIOHandle->pGPIOx->AFR[temp1]&=~((0x0F)<< (4*temp2));// clear bit positions
		pGPIOHandle->pGPIOx->AFR[temp1]|= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunction<< (4*temp2));// set ALT function
	}

}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
	//if user wants they can use
}

//GPIO_READFromInputPin
/*************************************
 * @FN 			- GPIO_READFromInputPin
 * @BRIEF		- This fn is to read from input pin
 * @param(in)	- GPIO  RegDef_t: pointer to GPIO GPIO Regdef_t structure
 * @return		- uint8_t value inside that pin
 * @note		- refer macros for gpio config
 *************************************/
uint8_t GPIO_READFromInputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber){

	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR>>PinNumber)& 0x00000001);
	return value;
}

uint16_t GPIO_READFromInputPort(GPIO_RegDef_t *pGPIOx){

	uint16_t value;
	value = (uint16_t)pGPIOx->IDR;
	return value;

}

void GPIO_WriteToOututPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value){

	if (Value ==1)
	{
		pGPIOx->ODR |=(1<<PinNumber);
	}
	else
	{
		pGPIOx->ODR &=~(1<<PinNumber);
	}
}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value){

	pGPIOx->ODR = Value;

}

uint8_t GPIO_READFromOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber){

	uint8_t value;
	value = (uint8_t)((pGPIOx->ODR>>PinNumber)& 0x00000001);
	return value;
}

uint16_t GPIO_READFromOutputPort(GPIO_RegDef_t *pGPIOx){

	uint16_t value;
	value = (uint16_t)pGPIOx->ODR;
	return value;

}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	pGPIOx->ODR ^=(1<<PinNumber);
}
