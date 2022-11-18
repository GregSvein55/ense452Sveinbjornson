/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t cliBufferTX[40];
uint8_t cliBufferRX[40];
uint8_t inputTXBuffer[40];
int x;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
void resetBuff(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*
 * Purpose: resets the buffer for RX and TX
 */
void resetBuff()//resets tx and rx buffer
{
  for (int i = 0; i < 40; ++i) {
	  cliBufferTX[i] = 0;
	  cliBufferRX[i] = 0;
  }
}


void resetInput()//resets input buffer
{
  for (int i = 0; i < 40; ++i) {
	  inputTXBuffer[i] = 0;
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_UART_Transmit(&huart2, "\x1b[0J", 5, 1000); //clearing screen

  strcpy((char *) inputTXBuffer, " ENSE 452 Lab 3: CLI Pt. 2\r\n\r\n");
  HAL_UART_Transmit(&huart2, inputTXBuffer, strlen((char *) inputTXBuffer), 1000);


  uint8_t commands = 0;
  int x = 0;


  //making scrollable cmd window
  HAL_UART_Transmit(&huart2, "\x1b[8;r", 5, 1000);
  HAL_UART_Transmit(&huart2, "\x1b[8;0H", 6, 1000);
  HAL_UART_Transmit(&huart2, "\x1b[1K", 4, 1000);


  HAL_UART_Receive(&huart2, cliBufferRX, 1, 300);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  resetInput();
  resetBuff();

  uint8_t input[20];


  while (1)
  {


	  	  //adding and updating info at top
	      HAL_UART_Transmit(&huart2, "\x1b[s", 3, 1000);
	 	  HAL_UART_Transmit(&huart2, "\x1b[3;0f", 7, 1000);
	 	  resetInput();
	 	  strcpy((char *) inputTXBuffer, "Number of commands: ");
	 	  HAL_UART_Transmit(&huart2, inputTXBuffer, strlen((char *) inputTXBuffer), 1000);
	 	  sprintf(input, "%d", commands);
	 	  HAL_UART_Transmit(&huart2, input, strlen((char *) input), 1000);
	 	  HAL_UART_Transmit(&huart2, "\x1b[u", 3, 1000);
	 	  HAL_Delay(1000);
	 	  commands += 1;


	 	  //tasks from Lab 2, takes user inputs and completes the commands

	 	  x = 0;

	 		  strcpy((char *)cliBufferTX, "\nEnter a command for the LED:\r\n");
	 		  HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *)cliBufferTX), 1000);

	 		  while(cliBufferTX[0] != '\r')
	 		  {
	 			  if(HAL_UART_Receive(&huart2, cliBufferRX, 1, 300) == HAL_OK)
	 			  {
	 				  if(cliBufferRX[0] == '\b')
	 				  {
	 					  cliBufferTX[0] = cliBufferRX[0];
	 					  cliBufferTX[1] = ' ';
	 					  cliBufferTX[2] = '\b';
	 					  HAL_UART_Transmit(&huart2, cliBufferTX, 3, 1000);
	 					  x--;
	 				  }
	 				  else
	 				  {
	 					  cliBufferTX[0] = cliBufferRX[0];
	 					  input[x] = cliBufferRX[0];
	 					  HAL_UART_Transmit(&huart2, cliBufferTX, 1, 1000);
	 					  x++;
	 				  }
	 			  }
	 		  }

	 		  if(input[0] == 'h' && input[1] == 'e' && input[2] == 'l' && input[3] == 'p' && input[4] == '\r')
	 		  {
	 			  strcpy((char *)cliBufferTX, "\non -> turns the green LED on\r\n");
	 			  HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *)cliBufferTX), 1000);
	 			  strcpy((char *)cliBufferTX, "\noff -> turns the green LED off\r\n");
	 			  HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *)cliBufferTX), 1000);
	 			  strcpy((char *)cliBufferTX, "\nquery -> returns the state of the green LED\r\n");
	 			  HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *)cliBufferTX), 1000);

	 		  }
	 		  else if(input[0] == 'o' && input[1] == 'n' && input[2] == '\r')
	 		  {
	 			  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1)
	 			  {
	 				  strcpy((char *)cliBufferTX, "\nThe LED is already on!\r\n");
	 			  }
	 			  else
	 			  {
	 				  strcpy((char *)cliBufferTX, "\nDone!\r\n");
	 			  }
	 			  HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *)cliBufferTX), 1000);
	 			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);


	 		  }
	 		  else if(input[0] == 'o' && input[1] == 'f' && input[2] == 'f' && input[3] == '\r')
	 		  {
	 			  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1)
	 			  {
	 				  strcpy((char *)cliBufferTX, "\nDone!\r\n");
	 			  }
	 			  else
	 			  {
	 				  strcpy((char *)cliBufferTX, "\nThe LED is already off!\r\n");
	 			  }
	 			  HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *)cliBufferTX), 1000);
	 			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	 		  }
	 		  else if(input[0] == 'q' && input[1] == 'u' && input[2] == 'e' && input[3] == 'r' && input[4] == 'y' && input[5] == '\r')
	 		  {
	 			  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == 1)
	 			  {
	 				  strcpy((char *)cliBufferTX, "\nThe LED is on!\r\n");
	 			  }
	 			  else
	 			  {
	 				  strcpy((char *)cliBufferTX, "\nThe LED is off!\r\n");
	 			  }
	 			  HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *)cliBufferTX), 1000);
	 		  }
	 		  else if(input[0] == '\r')
	 		  {
	 			  continue;
	 		  }
	 		  else
	 		  {
	 			  strcpy((char *)cliBufferTX, "\nError. Try again.\r\n");
	 			  HAL_UART_Transmit(&huart2, cliBufferTX, strlen((char *)cliBufferTX), 1000);
	 		  }

	 	 
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
