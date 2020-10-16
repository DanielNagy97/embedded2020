/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <max_display.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//Ezek mehetnének egy structba!
#define MAXPORT GPIOA
#define DATA_PIN GPIO_PIN_7
#define CS_PIN GPIO_PIN_6
#define CLOCK_PIN GPIO_PIN_5

#define NUMBER_OF_CELLS 4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t numbers[10][8]={
{0x1e, 0x33, 0x37, 0x3b, 0x33, 0x33, 0x1e, 0x0},//0
{0xe,  0x1e, 0x6,  0x6,  0x6,  0x6,  0x6,  0x0},//1
{0x1e, 0x33, 0x3,  0x1f, 0x30, 0x33, 0x3f, 0x0},//2
{0x1e, 0x33, 0x3,  0xf,  0x3,  0x33, 0x1e, 0x0},//3
{0x6,  0xe,  0x16, 0x26, 0x3f, 0x6,  0x6,  0x0},//4
{0x3f, 0x30, 0x3e, 0x33, 0x3,  0x33, 0x1e, 0x0},//5
{0x1e, 0x33, 0x30, 0x3f, 0x33, 0x33, 0x1e, 0x0},//6
{0x3f, 0x23, 0x3,  0x6,  0x6,  0xc,  0xc,  0x0},//7
{0x1e, 0x33, 0x33, 0x1e, 0x33, 0x33, 0x1e, 0x0},//8
{0x1e, 0x33, 0x33, 0x1f, 0x3,  0x33, 0x1e, 0x0},//9
};


void write_byte (uint8_t byte){
	for (int i = 0; i<8; i++){
		HAL_GPIO_WritePin (MAXPORT, CLOCK_PIN, 0);
		HAL_GPIO_WritePin (MAXPORT, DATA_PIN, byte&0x80);  // most significant bit
		byte = byte<<1;  // shift left
		HAL_GPIO_WritePin (MAXPORT, CLOCK_PIN, 1);
	}
}

void write_max_cmd (uint8_t address, uint8_t cmd){
	HAL_GPIO_WritePin (MAXPORT, CS_PIN, 0);
	for (int i = 0; i<NUMBER_OF_CELLS; i++){
		write_byte (address);
		write_byte (cmd);
	}
	HAL_GPIO_WritePin (MAXPORT, CS_PIN, 0);
	HAL_GPIO_WritePin (MAXPORT, CS_PIN, 1);
}

void max_clear(){
	for(int i = 0; i<8; i++){
		write_max_cmd(i+1, 0);
	}
}

void max_init (uint8_t brightness){
	max_clear();
	write_max_cmd(0x09, 0x00);// decoding = 0
	write_max_cmd(0x0b, 0x07);// scan limit = 8
	write_max_cmd(0x0c, 0x01);// mode = 1
	write_max_cmd(0x0f, 0x00);// test display = 0

	write_max_cmd(0x0a, brightness);
}

void set_byte_on_matrix(uint8_t byte, uint8_t row, uint8_t col, uint8_t shift){
	//Setting one byte on the 8 rows 4 columns matrix
	HAL_GPIO_WritePin (MAXPORT, CS_PIN, 0);

	for (int i = 0; i<NUMBER_OF_CELLS; i++){
		if(i == col){
			write_byte (row);
			write_byte (byte << shift);
		}
		else{
			write_byte (0);
			write_byte (0);
		}
	}
	HAL_GPIO_WritePin (MAXPORT, CS_PIN, 0);
	HAL_GPIO_WritePin (MAXPORT, CS_PIN, 1);
}

//Displaying a character in a specified cell
void display_character(uint8_t* character, uint8_t column, uint8_t shift){
	for(int j = 0; j<8; j++){
		set_byte_on_matrix(character[j], j+1, column, shift);
	}
}

uint8_t screen_buffer[NUMBER_OF_CELLS][8] = {0};


void display_screen_buffer(){
	for(int i = 0; i<NUMBER_OF_CELLS; i++){
		for(int j = 0; j<8; j++){
			set_byte_on_matrix(screen_buffer[i][j], j+1, i, 0);
		}
	}
}


void shift_screen_buffer_right(){
	for(int k = NUMBER_OF_CELLS; k>=0; k--){
		for(int j = 0; j<8; j++){
			screen_buffer[k][j] = screen_buffer[k][j] >> 1
								  	  | (screen_buffer[k-1][j] & 0x1) << 7;
		}
	}
}

void shift_screen_buffer_left(){
	for(int k = 0; k<NUMBER_OF_CELLS; k++){
		for(int j = 0; j<8; j++){
			screen_buffer[k][j] = screen_buffer[k][j] << 1
								  	  | (screen_buffer[k+1][j] & 0x80) >> 7;
		}
	}
}


//These are useless
void input_screen_buffer_right(uint8_t* character){
	//for(int k = 0; k<5; k++){
		for(int j = 0; j<8; j++){
			for(int i = 0; i<8; i++){
				screen_buffer[0][i] = character[i] << 8 >> j;
			}
			display_screen_buffer();
			HAL_Delay(80);
		}
		for(int j = 0; j<8; j++){
			shift_screen_buffer_right();
			display_screen_buffer();
			HAL_Delay(80);
		}
}

void input_screen_buffer_left(char* mytext){
	for(int k = 0; k<strlen(mytext); k++){
		for(int j = 0; j<=8; j++){
			for(int i = 0; i<8; i++){
				screen_buffer[3][i] = numbers[mytext[k]-0x30][i] << j >> 8;
			}
			display_screen_buffer();
			HAL_Delay(80);
		}

		for(int j = 0; j<8; j++){
			shift_screen_buffer_left();
			display_screen_buffer();
			HAL_Delay(80);
		}
	}
}


void scroll_text_left(char* mytext, uint16_t speed, uint8_t blank_space){
	for(int k = 0; k<strlen(mytext); k++){
		//mivel visszafele nezzuk
		for(int j = 7; j>=0; j--){
			shift_screen_buffer_left();
			for(int i = 0; i<8; i++){
				// n k-adik bitje: (n & ( 1 << k )) >> k
				screen_buffer[3][i] =
				    screen_buffer[3][i]
					    | (numbers[mytext[k]-0x30][i] & ( 1 << j )) >> j;
			}
			display_screen_buffer();
			HAL_Delay(speed);
		}
	}
	//making blank space after the text
	for(int i = 0; i<blank_space; i++){
		shift_screen_buffer_left();
		display_screen_buffer();
		HAL_Delay(speed);
	}
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval intsrand(time(NULL));   // Initialization, should only be called once.
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
  /* USER CODE BEGIN 2 */
  max_init(0x01);

  srand(time(NULL));

  char text_buffer[80] = {0};

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  sprintf(text_buffer, "%d", rand() % 100);
	  scroll_text_left(text_buffer, 40, 6);

	  //scroll_character(numbers[rand() % 10], 200);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, clock_Pin|cs_Pin|data_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : clock_Pin cs_Pin data_Pin */
  GPIO_InitStruct.Pin = clock_Pin|cs_Pin|data_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/