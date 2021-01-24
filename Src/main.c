/*
 * main.c
 *
 *  Created on: 23/01/2021
 *      Author: PC
 */

#include "stm32f4xx.h"
#include "main.h"
#include <string.h>

/*
 * Functions prototypes
 */

#define TRUE	1
#define FALSE	0

void UART2_Init();
void Error_handler();

UART_HandleTypeDef huart2; //Handle of UART 2


int main(){


	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	/* Basic initialization */
	HAL_Init();

	UART2_Init();

	/*
	 * Oscillator initialization. We are not actually using HSE because it does not exist
	 * But, we use the bypass mode and is configured in this way
	 */
	memset(&osc_init, 0, sizeof(osc_init)); //Maybe there is garbage values, so memset to 0
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK){

		Error_handler();
	}


	while(1);

	return 0;
}


/*
 * Function to initialize the UART2 peripheral
 */
void UART2_Init(){

	//1. Linking handle struct to base address
	huart2.Instance = USART2;

	//2. Init the handle variable
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;

	//3. Use the API to initialize
	if(HAL_UART_Init(&huart2) != HAL_OK){

		//There is a problem
		Error_handler();
	}
}


void Error_handler(void){
	while(1);
}
