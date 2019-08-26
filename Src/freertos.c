/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "string.h"
#include "stdio.h"
#include "usart.h"
#include "ec20.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern volatile uint8_t usart1_flag;
extern volatile uint8_t usart2_flag;
extern volatile uint8_t usart4_flag;
extern volatile uint16_t usart1_rx_len; 
extern volatile uint16_t usart2_rx_len;
extern volatile uint16_t usart4_rx_len;
extern uint8_t usart1_rx_buf[USART_MAX_DATA_LEN];
extern uint8_t usart2_rx_buf[USART_MAX_DATA_LEN];
extern uint8_t usart4_rx_buf[USART_MAX_DATA_LEN];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osMutexId RebootEcTimeHandle;

/* USER CODE END Variables */
osThreadId DUTTaskHandle;
osThreadId LightTaskHandle;
osMessageQId DTUTaskQueueHandle;
osTimerId myTimer01Handle;
osTimerId LedBlinkHandle;
osTimerId myTimer03Handle;
osMutexId myMutex01Handle;
osSemaphoreId myBinarySem01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void RebootEc20(void const * argument);
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void Callback01(void const * argument);
void Callback02(void const * argument);
void Callback03(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Pre/Post sleep processing prototypes */
void PreSleepProcessing(uint32_t *ulExpectedIdleTime);
void PostSleepProcessing(uint32_t *ulExpectedIdleTime);

/* USER CODE BEGIN PREPOSTSLEEP */
__weak void PreSleepProcessing(uint32_t *ulExpectedIdleTime)
{
/* place for user code */ 
}

__weak void PostSleepProcessing(uint32_t *ulExpectedIdleTime)
{
/* place for user code */
}
/* USER CODE END PREPOSTSLEEP */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of myMutex01 */
  osMutexDef(myMutex01);
  myMutex01Handle = osMutexCreate(osMutex(myMutex01));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of myBinarySem01 */
  osSemaphoreDef(myBinarySem01);
  myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of myTimer01 */
  osTimerDef(myTimer01, Callback01);
  myTimer01Handle = osTimerCreate(osTimer(myTimer01), osTimerPeriodic, NULL);

  /* definition and creation of myTimer02 */
  osTimerDef(myTimer02, Callback02);
  LedBlinkHandle = osTimerCreate(osTimer(myTimer02), osTimerOnce, NULL);

  /* definition and creation of myTimer03 */
  osTimerDef(myTimer03, Callback03);
  myTimer03Handle = osTimerCreate(osTimer(myTimer03), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
	osTimerDef(rebootec20, RebootEc20);
	RebootEcTimeHandle = osTimerCreate(osTimer(rebootec20), osTimerOnce, NULL);
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of DTUTaskQueue */
  osMessageQDef(DTUTaskQueue, 16, uint16_t);
  DTUTaskQueueHandle = osMessageCreate(osMessageQ(DTUTaskQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of DUTTask */
  osThreadDef(DUTTask, StartDefaultTask, osPriorityNormal, 0, 1024);
  DUTTaskHandle = osThreadCreate(osThread(DUTTask), NULL);

  /* definition and creation of LightTask */
  osThreadDef(LightTask, StartTask02, osPriorityIdle, 0, 256);
  LightTaskHandle = osThreadCreate(osThread(LightTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the DUTTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
//	char buf[5] = {'a','b','c','d','e'};
	Ec20PowerOn();
	osDelay(15000);
	EC20_Init();
	LinkFirstTCPSocket();
	LinkSecondTCPSocket();
	LinkThirdTCPSocket();
	LinkFourthTCPSocket();
	
  for(;;)
  {
//			printf("AT+QIACT?\r\n");//获取当前卡的IP地址
//			osDelay(20);
//			Clear_Buffer();	
//			printf("AT+CSQ\r\n"); //检查CSQ
//			osDelay(50);
//			Clear_Buffer();
//			LinkFirstTCPSocket();
//			printf("AT+QISTATE?\r\n");
//			osDelay(50);
//			Clear_Buffer();
		if(usart1_flag == 1)
		{
				usart1_flag = 0;
				EC20Send_StrData((char *)usart1_rx_buf);
				ClearUart1Buffer();
				HAL_UART_Receive_DMA(&huart1, usart1_rx_buf, USART_MAX_DATA_LEN);
		}
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the LightTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
	osTimerStart(myTimer01Handle, 1000);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartTask02 */
}

/* Callback01 function */
void Callback01(void const * argument)
{
  /* USER CODE BEGIN Callback01 */
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
  /* USER CODE END Callback01 */
}

/* Callback02 function */
void Callback02(void const * argument)
{
  /* USER CODE BEGIN Callback02 */
  
  /* USER CODE END Callback02 */
}

/* Callback03 function */
void Callback03(void const * argument)
{
  /* USER CODE BEGIN Callback03 */
  
  /* USER CODE END Callback03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void RebootEc20(void const * argument)
{
		Ec20Reset();
		osDelay(15000);
		EC20_Init();
		LinkFirstTCPSocket();
		LinkSecondTCPSocket();
		LinkThirdTCPSocket();
		LinkFourthTCPSocket();
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
