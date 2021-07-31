/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include <key.h>
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "menu.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_tx;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for display */
osThreadId_t displayHandle;
const osThreadAttr_t display_attributes = {
  .name = "display",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for buttonScan */
osThreadId_t buttonScanHandle;
const osThreadAttr_t buttonScan_attributes = {
  .name = "buttonScan",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for queueMessages */
osMessageQueueId_t queueMessagesHandle;
const osMessageQueueAttr_t queueMessages_attributes = {
  .name = "queueMessages"
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
void StartDefaultTask(void *argument);
void StartDisplay(void *argument);
void StartButtonScan(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of queueMessages */
  queueMessagesHandle = osMessageQueueNew (5, sizeof(Message_TypeDef), &queueMessages_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of display */
  displayHandle = osThreadNew(StartDisplay, NULL, &display_attributes);

  /* creation of buttonScan */
  buttonScanHandle = osThreadNew(StartButtonScan, NULL, &buttonScan_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00300F33;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /** I2C Fast mode Plus enable
  */
  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : BUTTON_LEFT_Pin BUTTON_RIGHT_Pin BUTTON_DOWN_Pin BUTTON_UP_Pin
                           BUTTON_SELECT_Pin */
  GPIO_InitStruct.Pin = BUTTON_LEFT_Pin|BUTTON_RIGHT_Pin|BUTTON_DOWN_Pin|BUTTON_UP_Pin
                          |BUTTON_SELECT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartDisplay */
/**
* @brief Function implementing the display thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDisplay */
void StartDisplay(void *argument)
{
  /* USER CODE BEGIN StartDisplay */
	ssd1306_Init(&hi2c1);
	Rectangle rectMain(0, 0, SSD1306_WIDTH, SSD1306_HEIGHT);
	Rectangle rectButton(0, 0, SSD1306_WIDTH, 32);
	menu::Button buttonHeating("НАГРЕВ", rectButton, FontVerdana_16x16);
	menu::Button buttonMotor("ДВИГАТЕЛЬ", rectButton, FontVerdana_16x16);
	buttonHeating.events.add(menu::BUTTON_SELECT);
	buttonMotor.events.add(menu::BUTTON_SELECT);
	buttonHeating.is_toggle = buttonMotor.is_toggle = true;
	menu::Control *controls[] = { &buttonHeating, &buttonMotor };
	menu::Screen screenMain(rectMain, nullptr, nullptr, controls, 2);

	Message_TypeDef message;
	osStatus_t status;
	/* Infinite loop */
	for(;;)
	{
		uint8_t msg_count = osMessageQueueGetCount(queueMessagesHandle);
		if(msg_count > 0)
		{
			for(uint8_t i = 0; i < msg_count; i++)
			{
				status = osMessageQueueGet(queueMessagesHandle, &message, 0U, 0);
				if(status == osOK)
				{
					switch(message.id)
					{
						case ID_BUTTON:
						{
							uint32_t id_buttons = *((uint32_t*)message.param);

							if(id_buttons > 0)
							{
								for(uint8_t bit_pos = 0; bit_pos < 32; bit_pos++)
								{
									uint8_t bit = (1 << bit_pos);
									if(id_buttons & bit)
									{
										screenMain.onClick((menu::key_t)bit);
										id_buttons &= ~bit;
									}

//									if(id_buttons == 0)
//										break;
								}
							}
						}
						break;
					}
				}
			}
		}

		screenMain.draw();
		osDelay(50);
	}
  /* USER CODE END StartDisplay */
}

/* USER CODE BEGIN Header_StartButtonScan */
/**
* @brief Function implementing the buttonScan thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartButtonScan */
void StartButtonScan(void *argument)
{
  /* USER CODE BEGIN StartButtonScan */
	key::key_set_t set_left =
	{
		BUTTON_LEFT_GPIO_Port,
		BUTTON_LEFT_Pin,
		menu::BUTTON_LEFT,
		GPIO_PIN_RESET,
		key::PRESSED_STATE,
		key::Keyboard::period,
		false
	};
	key::key_set_t set_right =
	{
		BUTTON_RIGHT_GPIO_Port,
		BUTTON_RIGHT_Pin,
		menu::BUTTON_RIGHT,
		GPIO_PIN_RESET,
		key::PRESSED_STATE,
		key::Keyboard::period,
		false
	};
	key::key_set_t set_up =
	{
		BUTTON_UP_GPIO_Port,
		BUTTON_UP_Pin,
		menu::BUTTON_UP,
		GPIO_PIN_RESET,
		key::PRESSED_STATE,
		key::Keyboard::period,
		false
	};
	key::key_set_t set_down =
	{
		BUTTON_DOWN_GPIO_Port,
		BUTTON_DOWN_Pin,
		menu::BUTTON_DOWN,
		GPIO_PIN_RESET,
		key::PRESSED_STATE,
		key::Keyboard::period,
		false
	};
	key::key_set_t set_select =
	{
		BUTTON_SELECT_GPIO_Port,
		BUTTON_SELECT_Pin,
		menu::BUTTON_SELECT,
		GPIO_PIN_RESET,
		key::PRESSED_STATE,
		key::Keyboard::period,
		true
	};
	key::Key button_left(set_left);
	key::Key button_right(set_right);
	key::Key button_up(set_up);
	key::Key button_down(set_down);
	key::Key button_select(set_select);
	key::Key *buttons[] = { &button_left, &button_right, &button_select, &button_up, &button_down };
	key::Keyboard keyboard(buttons, 5);

	Message_TypeDef message;
	uint32_t active_buttons;
	/* Infinite loop */
	for(;;)
	{
		active_buttons = 0;
		keyboard.scan();
		if(button_left.is_active)
		{
			active_buttons |= button_left.id();
			button_left.reset();
		}
		if(button_right.is_active)
		{
			active_buttons |= button_right.id();
			button_right.reset();
		}
		if(button_up.is_active)
		{
			active_buttons |= button_up.id();
			button_up.reset();
		}
		if(button_down.is_active)
		{
			active_buttons |= button_down.id();
			button_down.reset();
		}
		if(button_select.is_active)
		{
			active_buttons |= button_select.id();
			button_select.reset();
		}

		if(active_buttons != 0)
		{
			message.id = ID_BUTTON;
			message.param = (void*)&active_buttons;
			osMessageQueuePut(queueMessagesHandle, &message, 0U, 0);
		}

		osDelay(keyboard.period);
	}
  /* USER CODE END StartButtonScan */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM17 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM17) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
