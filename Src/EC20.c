/******************** (C) COPYRIGHT 2016 CETC27-CETIIC-ZYGD ********************
* File Name          : EC20.c
* Author             : yangchuang
* Version            : V1.0
* Date               : 19/06/2019
* Description        : the file include all ec20 driver and applycation function 
*******************************************************************************/
#ifndef __EC20_H__
#define __EC20_H__

#include "EC20.h"
#include "string.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_usart.h"
#include "cmsis_os.h"
#include "usart.h"

char *strx,*extstrx,*Readystrx;
uint16_t g_time = 0;
extern uint8_t usart2_rx_buf[USART_MAX_DATA_LEN];
extern volatile uint16_t usart2_rx_len;
extern volatile uint16_t usart1_rx_len; 
extern volatile uint8_t usart2_flag;
extern osMutexId RebootEcTimeHandle;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern osMutexId myMutex01Handle;
uint8_t g_fix_data[1];


void TransATcodeToBuf(void)
{
//		sprintf(g_Configure_Context, "AT+QICSGP=1,1,"UNINET","","",1", "%s");
}

void ConfigureContex(void)
{
//		char g_Configure_Context[40];
//		sprintf(g_Configure_Context, "AT+QICSGP=1,1,"UNINET","","",1", "%s");
//		HAL_UART_Transmit(&huart2, g_Configure_Context, 5, 0xFF);
}
/*****************************the below code is for zhaojie board***************************************
void Ec20PowerOn(void)
{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
		osDelay(150);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
}

void Ec20PowerOff(void)
{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
		osDelay(700);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
}

void Ec20Reset(void)
{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
		osDelay(400);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
}
*******************************************************************************************************/

void Ec20PowerOn(void)
{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
		osDelay(150);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
}

void Ec20PowerOff(void)
{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
		osDelay(700);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
}

void Ec20Reset(void)
{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
		osDelay(5);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
		osDelay(400);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
}

void Clear_Buffer(void)//清空缓存
{                              
//		HAL_UART_Transmit(&huart2, usart2_rx_buf, usart2_rx_len, 0xFF);
		memset(usart2_rx_buf, 0, USART_MAX_DATA_LEN);
		usart2_rx_len=0;
}

void CheckWhetherRdy(void)
{
  g_time = 0;
	extstrx = strstr((const char*)usart2_rx_buf,(const char*)"RDY");
	while(extstrx == NULL)
	{
			osDelay(10);
			g_time++;
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"RDY");
			if (g_time >= 30000)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
	}
	printf("CheckWhetherRdy is: %s\r\n", extstrx);
	Clear_Buffer();	
	HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}	
	
void CheckWhetherReturnOK(void)
{
		g_time = 0;
		char str[] = {"AT\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"AT");
		while(extstrx == NULL)
		{
			osDelay(10);
			g_time++;
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"AT");
			if (g_time >= 30000)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
		}
		printf("CheckWhetherReturnOK is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void CloseEchoDisplay(void)
{
		g_time = 0;
		char str[] = {"ATE0\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"ATE0");
		while(extstrx == NULL)
		{
			osDelay(10);
			g_time++;
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"ATE0");
			if (g_time >= 60)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
		}
		printf("CloseEchoDisplay is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void CheckSignalQuality(void)
{
		g_time = 0;
		char str[] = {"AT+CSQ\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
		while(extstrx == NULL)
		{
			osDelay(10);
			g_time++;
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
			if (g_time >= 60)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
		}
		printf("CheckSignalQuality is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void DisplayProductIdInformation(void)
{
		g_time = 0;
		char str[] = {"ATI\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
		while(extstrx == NULL)
		{
			osDelay(10);
			g_time++;
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
			if (g_time >= 60)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
		}
		printf("DisplayProductIdInformation is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void CheckSIM(void)
{
		g_time = 0;
		char str[] = {"AT+CPIN?\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+CPIN: READY");
		while(extstrx == NULL)
		{
			osDelay(10);
			g_time++;
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+CPIN: READY");
			if (g_time >= 30000)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
		}
		printf("CheckSIM is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void NetworkOperatorSelect(void)
{
		g_time = 0;
		char str[] = {"AT+COPS?\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
		while(extstrx == NULL)
		{
			osDelay(10);
			g_time++;
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
			if (g_time >= 30000)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
		}
		printf("NetworkOperatorSelect is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void CloseSocketService(void)
{
		g_time = 0;
		char str[] = {"AT+QICLOSE=0\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
		while(extstrx == NULL)
		{
			osDelay(10);
			g_time++;
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
			if (g_time >= 20000)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
		}
		printf("CloseSocketService is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void CheckGSMInternet(void)
{
		uint8_t time_flag = 0;
		char str[] = {"AT+CREG?\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		osDelay(200);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx=strstr((const char*)usart2_rx_buf,(const char*)"+CREG: 0,1");//返回正常
				extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CREG: 0,5");//返回正常，漫游
				while((strx == NULL)&&(extstrx == NULL))
				{
						Clear_Buffer();
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
						osDelay(200);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"+CREG: 0,1");//返回正常
						extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CREG: 0,5");//返回正常，漫游
						if ((strx == NULL)&&(time_flag == 0))
						{
								time_flag =1;
								osTimerStart(RebootEcTimeHandle,60000);
						}
						else if ((strx != NULL)&&(time_flag == 1))
						{
								osTimerStop(RebootEcTimeHandle);
						}		  
						else 
						{
								//do nothing
						}	  
				}
		}
		else 
		{
				HAL_NVIC_SystemReset();
		}
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void CheckGPRSInternet(void)
{
		uint8_t time_flag = 0;
		char str[] = {"AT+CGREG?\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		osDelay(200);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx=strstr((const char*)usart2_rx_buf,(const char*)"+CGREG: 0,1");//这里重要，只有注册成功，才可以进行GPRS数据传输
				extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CGREG: 0,5");//返回正常，漫游
				while((strx == NULL)&&(extstrx == NULL))
				{
						Clear_Buffer();
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
						osDelay(200);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"+CGREG: 0,1");//这里重要，只有注册成功，才可以进行GPRS数据传输
						extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CGREG: 0,5");//返回正常，漫游
						if ((strx == NULL)&&(time_flag == 0))
						{
								time_flag =1;
								osTimerStart(RebootEcTimeHandle,60000);
						}
						else if ((strx != NULL)&&(time_flag == 1))
						{
								osTimerStop(RebootEcTimeHandle);
						}		  
						else 
						{
								//do nothing
						}	  
				}
		}
		else 
		{
				HAL_NVIC_SystemReset();
		}
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void AccessAPN(void)
{
		g_time = 0;
		char str[] = {"AT+QICSGP=1,1,\042CMNET\042,\042\042,\042\042,0\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
		while(extstrx == NULL)
		{
			g_time++;
			osDelay(10);
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
			if (g_time >= 30000)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
		}
		printf("AccessAPN is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void QueryConfigContex(void)
{
		g_time = 0;
		char str[] = {"AT+QICSGP=1\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
		while(extstrx == NULL)
		{
			osDelay(10);
			g_time++;
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
			if (g_time >= 20000)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
		}
		printf("QueryConfigContex is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}


void DeactivationSystem(void)
{
		g_time = 0;
		char str[] = {"AT+QIDEACT=1\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
		while(extstrx == NULL)
		{
			g_time++;
			osDelay(10);
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
			if (g_time >= 30000)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
		}
		printf("DeactivationSystem is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void ActivationSystem(void)
{
		g_time = 0;
		char str[] = {"AT+QIACT=1\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
		while(extstrx == NULL)
		{
			g_time++;
			osDelay(10);
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
			if (g_time >= 30000)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
		}
		printf("ActivationSystem is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void QureyLastErrorCode(void)
{
		g_time = 0;
		char str[] = {"AT+QIGETERROR\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+QIGETERROR:");
		while(extstrx == NULL)
		{
			g_time++;
			osDelay(10);
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+QIGETERROR:");
			if (g_time >= 30000)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
		}
		printf("QureyLastErrorCode is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void ConfirmSendData(void)
{
		g_fix_data[0] = 0x01A;
	
    HAL_UART_Transmit_DMA(&huart2, g_fix_data, 1);//发送完成函数
    while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void SendDateDone(char *bufferdata)
{
		g_time = 0;
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&bufferdata, usart1_rx_len);
		ConfirmSendData();
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
		while(!strstr((const char*)usart2_rx_buf,(const char*)"SEND OK"))
		{
			osDelay(5);
			g_time++;
			if (g_time >= 30000)
			{
					osTimerStart(RebootEcTimeHandle,10000);
			}
		}
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

//void SendDateDone(char *bufferdata)
//{
//		uint8_t time_flag = 0;

//		printf(bufferdata);
//		ConfirmSendData();
//		osDelay(5);
//		if (usart2_flag)
//		{
//				usart2_flag = 0;
//				strx=strstr((const char*)usart2_rx_buf,(const char*)"SEND OK");//开启成功
//				while(strx == NULL)
//				{
//						Clear_Buffer();	
//						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//						printf(bufferdata);
//						ConfirmSendData();
//						osDelay(5);
//						strx=strstr((const char*)usart2_rx_buf,(const char*)"SEND OK");//开启成功
//						if ((strx == NULL)&&(time_flag == 0))
//						{
//								time_flag =1;
//								osTimerStart(RebootEcTimeHandle,60000);
//						}
//						else if ((strx != NULL)&&(time_flag == 1))
//						{
//								osTimerStop(RebootEcTimeHandle);
//						}		  
//						else 
//						{
//								//do nothing
//						}	  
//				}
//		}
//		else 
//		{
//				HAL_NVIC_SystemReset();
//		}
//		Clear_Buffer();	
//		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//}

void LinkFirstTCPSocket(void)
{
		g_time = 0;
		char str[] = {"AT+QIOPEN=1,0,\042TCP\042,\04239.106.226.214\042,11001,0,1\r\n"};
		char str1[] = {"AT+QICLOSE=0\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
				while(extstrx == NULL)
				{
					osDelay(5);
					g_time++;
					extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
					if (g_time >= 30000)
					{
						Clear_Buffer();	
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						g_time = 0;
						extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
						while(extstrx == NULL)
						{
							osDelay(5);
							g_time++;
							extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
							if (g_time >= 2000)
							{
								osTimerStart(RebootEcTimeHandle,10000);
							}
						}
					}
				}
				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
				g_time = 0;
				extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 0,0");
				while(extstrx == NULL)
				{
						osDelay(5);
						g_time++;
						extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 0,0");
						if (g_time >= 30000)
						{
							Clear_Buffer();	
							HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
							HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
							g_time = 0;
							extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
							while(extstrx == NULL)
							{
								osDelay(5);
								g_time++;
								extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
								if (g_time >= 2000)
								{
									osTimerStart(RebootEcTimeHandle,10000);
								}
							}
						}
				}
		}
		else 
		{
				HAL_NVIC_SystemReset();
		}
		printf("LinkFirstTCPSocket is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void LinkSecondTCPSocket(void)
{
		g_time = 0;
		char str[] = {"AT+QIOPEN=1,1,\042TCP\042,\04239.106.226.214\042,11002,0,1\r\n"};
		char str1[] = {"AT+QICLOSE=1\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
				while(extstrx == NULL)
				{
					osDelay(5);
					g_time++;
					extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
					if (g_time >= 30000)
					{
						Clear_Buffer();	
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						g_time = 0;
						extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
						while(extstrx == NULL)
						{
							osDelay(5);
							g_time++;
							extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
							if (g_time >= 2000)
							{
								osTimerStart(RebootEcTimeHandle,10000);
							}
						}
					}
				}
				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
				g_time = 0;
				extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 1,0");
				while(extstrx == NULL)
				{
						osDelay(5);
						g_time++;
						extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 1,0");
						if (g_time >= 30000)
						{
							Clear_Buffer();	
							HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
							HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
							g_time = 0;
							extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
							while(extstrx == NULL)
							{
								osDelay(5);
								g_time++;
								extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
								if (g_time >= 2000)
								{
									osTimerStart(RebootEcTimeHandle,10000);
								}
							}
						}
				}
		}
		else 
		{
				HAL_NVIC_SystemReset();
		}
		printf("LinkSecondTCPSocket is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void LinkThirdTCPSocket(void)
{
		g_time = 0;
		char str[] = {"AT+QIOPEN=1,2,\042TCP\042,\04239.106.226.214\042,11003,0,1\r\n"};
		char str1[] = {"AT+QICLOSE=2\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
				while(extstrx == NULL)
				{
					osDelay(5);
					g_time++;
					extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
					if (g_time >= 30000)
					{
						Clear_Buffer();	
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						g_time = 0;
						extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
						while(extstrx == NULL)
						{
							osDelay(5);
							g_time++;
							extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
							if (g_time >= 2000)
							{
								osTimerStart(RebootEcTimeHandle,10000);
							}
						}
					}
				}
				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
				g_time = 0;
				extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 2,0");
				while(extstrx == NULL)
				{
						osDelay(5);
						g_time++;
						extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 2,0");
						if (g_time >= 30000)
						{
							Clear_Buffer();	
							HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
							HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
							g_time = 0;
							extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
							while(extstrx == NULL)
							{
								osDelay(5);
								g_time++;
								extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
								if (g_time >= 2000)
								{
									osTimerStart(RebootEcTimeHandle,10000);
								}
							}
						}
				}
		}
		else 
		{
				HAL_NVIC_SystemReset();
		}
		printf("LinkThirdTCPSocket is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void LinkFourthTCPSocket(void)
{
		g_time = 0;
		char str[] = {"AT+QIOPEN=1,3,\042TCP\042,\04239.106.226.214\042,11004,0,1\r\n"};
		char str1[] = {"AT+QICLOSE=3\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
				while(extstrx == NULL)
				{
					osDelay(5);
					g_time++;
					extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
					if (g_time >= 30000)
					{
						Clear_Buffer();	
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						g_time = 0;
						extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
						while(extstrx == NULL)
						{
							osDelay(5);
							g_time++;
							extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
							if (g_time >= 2000)
							{
								osTimerStart(RebootEcTimeHandle,10000);
							}
						}
					}
				}
				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
				g_time = 0;
				extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 3,0");
				while(extstrx == NULL)
				{
						osDelay(5);
						g_time++;
						extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 3,0");
						if (g_time >= 30000)
						{
							Clear_Buffer();	
							HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
							HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
							g_time = 0;
							extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
							while(extstrx == NULL)
							{
								osDelay(5);
								g_time++;
								extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
								if (g_time >= 2000)
								{
									osTimerStart(RebootEcTimeHandle,10000);
								}
							}
						}
				}
		}
		else 
		{
				HAL_NVIC_SystemReset();
		}
		printf("LinkFourthTCPSocket is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void  EC20_Init(void)
{
		CheckWhetherRdy();
		CheckWhetherReturnOK();
		CloseEchoDisplay();
		CheckSignalQuality();
		DisplayProductIdInformation();
		CheckSIM();
//		CheckGSMInternet();
//		CheckGPRSInternet();
		NetworkOperatorSelect();
		CloseSocketService();
		AccessAPN();
		QueryConfigContex();
		DeactivationSystem();
		ActivationSystem();
}

void EC20Send_StrData(char *bufferdata)
{
		uint8_t untildata = 0xff;
		char str0[] = {"AT+QISEND=0\r\n"};
		char str1[] = {"AT+QISEND=0,0\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str0, strlen(str0));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		while(!strstr((char*)usart2_rx_buf,(char*)">"))
		{
			osDelay(5);
		}
		SendDateDone(bufferdata);
    Clear_Buffer();
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
    while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
    strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//·￠?íê￡óà×??úêy?Y
    while(untildata)
    {
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
				while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
				{
				}
				strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//·￠?íê￡óà×??úêy?Y
				strx=strstr((char*)strx,(char*)",");//??è?μúò???
				strx=strstr((char*)(strx+1),(char*)",");//??è?μú?t??
				untildata=*(strx+1)-0x30;
				Clear_Buffer();
				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
    }
}

void EC20Send_MultiStrData(uint8_t channel ,char *bufferdata)
{
		uint8_t untildata = 0xff;
		char str0[] = {"AT+QISEND=channel\r\n"};
		char str1[] = {"AT+QISEND=channel,0\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str0, strlen(str0));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		while(!strstr((char*)usart2_rx_buf,(char*)">"))
		{
			osDelay(5);
		}
		SendDateDone(bufferdata);
    Clear_Buffer();
    HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
    strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//·￠?íê￡óà×??úêy?Y
    while(untildata)
    {
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
				while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
				{
				}
				strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//·￠?íê￡óà×??úêy?Y
				strx=strstr((char*)strx,(char*)",");//??è?μúò???
				strx=strstr((char*)(strx+1),(char*)",");//??è?μú?t??
				untildata=*(strx+1)-0x30;
				Clear_Buffer();
				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
    }
}

//发送十六进制数据
//void EC20Send_HexData(char *bufferdata)
//{
//    uint8_t untildata = 0xff;
//		g_fix_data[0] = 0x1a;
//    printf("AT+QISENDEX=0,\042%s\042\r\n",bufferdata);
//    osDelay(100);
//    HAL_UART_Transmit(&huart2, g_fix_data, 1, 0XFF);//发送完成函数
//    while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
//    {
//    }
//    osDelay(100);
//    strx=strstr((char*)usart2_rx_buf,(char*)"OK");//是否正确发送
//    while(strx==NULL)
//    {
//        strx=strstr((char*)usart2_rx_buf,(char*)"OK");//是否正确发送
//        osDelay(10);
//    }
//		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//    osDelay(100);
//    Clear_Buffer();
//    printf("AT+QISEND=0,0\r\n");
//    osDelay(200);
//    strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//发送剩余字节数据
//    while(untildata)
//    {
//				printf("AT+QISEND=0,0\r\n");
//				osDelay(200);
//				strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//发送剩余字节数据
//				strx=strstr((char*)strx,(char*)",");//获取第一个
//				strx=strstr((char*)(strx+1),(char*)",");//获取第二个
//				untildata=*(strx+1)-0x30;
//				Clear_Buffer();
//    }
//}

//void StartUdpService(void)
//{
//		uint16_t time = 0;
//		printf("AT+QIOPEN=1,4,\042UDP\042,\04239.106.226.214\042,11005,0,1\r\n");
//		while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
//		{
//			osDelay(5);
//			time++;
//			if (time >= 30000)
//			{
//				Clear_Buffer();	
//				printf("AT+QICLOSE=4\r\n");
//				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//				time = 0;
//				while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
//				{
//					osDelay(5);
//					time++;
//					if (time >= 2000)
//					{
//						osTimerStart(RebootEcTimeHandle,10000);
//					}
//				}
//			}
//		}
//		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//		time = 0;
//		while(!strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 4,0"))
//		{
//				osDelay(5);
//				time++;
//				if (time >= 30000)
//				{
//					Clear_Buffer();	
//					printf("AT+QICLOSE=4\r\n");
//					HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//					time = 0;
//					while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
//					{
//						 osDelay(5);
//						 time++;
//						if (time >= 2000)
//						{
//							osTimerStart(RebootEcTimeHandle,10000);
//						}
//					}
//				}
//		}
//		Clear_Buffer();	
//		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//}

void StartUdpService(void)
{
		g_time = 0;
		char str0[] = {"AT+QIOPEN=1,4,\042UDP SERVICE\042,\042127.0.0.1\042,11005,0,0\r\n"};
		char str1[] = {"AT+QICLOSE=4\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str0, strlen(str0));
		while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		osDelay(500);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
		QureyLastErrorCode();
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
		while(extstrx == NULL)
		{
			osDelay(5);
			g_time++;
			extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
			if (g_time >= 30000)
			{
				Clear_Buffer();	
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
				g_time = 0;
				extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
				while(extstrx == NULL)
				{
					osDelay(5);
					g_time++;
					extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
					if (g_time >= 2000)
					{
						osTimerStart(RebootEcTimeHandle,10000);
					}
				}
			}
		}
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
		g_time = 0;
		extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 4,0");
		while(extstrx == NULL)
		{
				osDelay(5);
				g_time++;
				extstrx = strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 4,0");
				if (g_time >= 30000)
				{
					Clear_Buffer();	
					HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
					HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
					g_time = 0;
					extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
					while(extstrx == NULL)
					{
						osDelay(5);
						g_time++;
						extstrx = strstr((const char*)usart2_rx_buf,(const char*)"OK");
						if (g_time >= 2000)
						{
							osTimerStart(RebootEcTimeHandle,10000);
						}
					}
				}
		}
		printf("StartUdpService is: %s\r\n", extstrx);
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void SendUdpDate(void)
{
	uint8_t untildata = 0xff;
	char buffer[5] = {'y','a','n','g'};
	char str0[] = {"AT+QISEND=4,5,\04239.106.226.214\042,11005\r\n"};
	char str1[] = {"AT+QISEND=4,0\r\n"};
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str0, strlen(str0));
	while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
	{
	}
	while(!strstr((char*)usart2_rx_buf,(char*)">"))
	{
		osDelay(5);
	}
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&buffer, strlen(buffer));
	Clear_Buffer();
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
	osDelay(5);
	strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//发送剩余字节数据
	while(untildata)
	{
			HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
			osDelay(20);
			strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//发送剩余字节数据
			strx=strstr((char*)strx,(char*)",");//获取第一个
			strx=strstr((char*)(strx+1),(char*)",");//获取第二个
			untildata=*(strx+1)-0x30;
			Clear_Buffer();
			HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
    }
		printf("UDP send data over\r\n");
}

//void SendUdpDate(void)
//{
//	uint8_t untildata = 0xff;
//	char buffer[5] = {'y','a','n','g'};
//	printf("AT+QISEND=4\r\n");
//	while(!strstr((char*)usart2_rx_buf,(char*)">"))
//	{
//		osDelay(5);
//	}
//	SendDateDone(buffer);
//	Clear_Buffer();
//	printf("AT+QISEND=4,0\r\n");
//	osDelay(5);
//	strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//发送剩余字节数据
//	while(untildata)
//	{
//			printf("AT+QISEND=4,0\r\n");
//			osDelay(20);
//			strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//发送剩余字节数据
//			strx=strstr((char*)strx,(char*)",");//获取第一个
//			strx=strstr((char*)(strx+1),(char*)",");//获取第二个
//			untildata=*(strx+1)-0x30;
//			Clear_Buffer();
//			HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//    }
//}

void ReceiveUdpDate(void)
{
	char str[] = {"AT+QIRD=4\r\n"};
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
	while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
	{
	}
	while(!strstr((char*)usart2_rx_buf,(char*)"+QIRD"))
	{
		osDelay(5);
	}
	Clear_Buffer();	
	HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void CloseUdpService(void)
{
	char str[] = {"AT+CLOSE=4"};
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
	while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
	{
	}
	while(!strstr((char*)usart2_rx_buf,(char*)"OK"))
	{
		osDelay(5);
	}
	Clear_Buffer();	
	HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

#endif

