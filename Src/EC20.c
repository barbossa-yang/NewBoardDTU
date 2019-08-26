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
extern uint8_t usart2_rx_buf[USART_MAX_DATA_LEN];
extern uint16_t usart2_rx_len;
extern volatile uint8_t usart2_flag;
extern osMutexId RebootEcTimeHandle;

uint8_t g_fix_data[1];

/**************************对应at指令中的4.1章节*******************************/
char g_Configure_Context[40];
char g_Activate_Context[20];
char g_Deactivate_Context[20];
/**************************对应at指令中的4.2章节*******************************/
char g_TcpClientConnAccessMode[50];
char g_SendDataInAccessMode[20];
char g_RecDataInAccessMode[50];
char g_CloseConnAccessMode[20];
/**************************对应at指令中的4.3章节*******************************/
char g_TCPClientConnTransMode[20];
uint8_t g_SentDataInTransMode[20];
uint8_t g_RecDataInTransMode[20];
char g_CloseTcpClientTransMode[20];
/**************************对应at指令中的4.4章节*******************************/
char g_SetClientConnInPushMode[20];
char g_SendDataInPushMode[20];
char g_RecDataInPushMode[20];
char g_CloseTcpClientPushMode[20];
/**************************对应at指令中的4.5章节*******************************/
char g_StartTcpServer[50];
char g_AcceptTcpIncomConn[20];
char g_RecDataFromIncomConnect[50];
char g_CloseTcpServ[20];
/**************************对应at指令中的4.6章节*******************************/
char g_StartUdpServ[50];
char g_SendUdpDataToRemote[20];
char g_RecDataFromRemote[50];
char g_CloseUdpServ[20];
/**************************对应at指令中的4.7章节*******************************/
char g_Ping[30];
/**************************对应at指令中的4.8章节*******************************/
char g_SynchronizeLocalTime[30];
/**************************对应at指令中的4.9章节*******************************/
char g_GetLastErrorCode[30];



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
void CheckWhetherReturnOK(void)
{
		uint8_t time_flag = 0;
		printf("AT\r\n"); 
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx = strstr((const char*)usart2_rx_buf,(const char*)"RDY");//返回OK
				while(strx == NULL)
				{
						Clear_Buffer();	
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						printf("AT\r\n"); 
						osDelay(50);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//返回OK
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

void CheckSignalQuality(void)
{
		uint8_t time_flag = 0;
		printf("AT+CSQ\r\n"); //检查CSQ
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx = strstr((const char*)usart2_rx_buf,(const char*)"99");//查看是否返回ready
				extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CSQ: OK");//返回正常，漫游
				while(~((strx == NULL)&&(extstrx != NULL)))
				{
						Clear_Buffer();	
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						printf("AT+CSQ\r\n"); //检查CSQ
						osDelay(50);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"99");//check signal quality 
						extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CSQ: OK");//返回正常，漫游
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

void CheckSIM(void)
{
		uint8_t time_flag = 0;
		printf("AT+CPIN?\r\n");//检查SIM卡是否在位
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx = strstr((const char*)usart2_rx_buf,(const char*)"+CPIN: READY");//查看是否返回ready
				while(strx == NULL)
				{
						Clear_Buffer();	
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						printf("AT+CPIN?\r\n");
						osDelay(50);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"+CPIN: READY");//检查SIM卡是否在位，等待卡在位，如果卡识别不到，剩余的工作就没法做了
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

void CheckGSMInternet(void)
{
		uint8_t time_flag = 0;
		printf("AT+CREG?\r\n");//查看是否注册GSM网络
		osDelay(200);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx=strstr((const char*)usart2_rx_buf,(const char*)"+CREG: 0,1");//返回正常
				extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CREG: 0,5");//返回正常，漫游
				while((strx == NULL)&&(extstrx == NULL))
				{
						Clear_Buffer();
						printf("AT+CREG?\r\n");//查看是否注册GSM网络
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
		printf("AT+CGREG?\r\n");//查看是否注册GPRS网络
		osDelay(200);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx=strstr((const char*)usart2_rx_buf,(const char*)"+CGREG: 0,1");//这里重要，只有注册成功，才可以进行GPRS数据传输
				extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CGREG: 0,5");//返回正常，漫游
				while((strx == NULL)&&(extstrx == NULL))
				{
						Clear_Buffer();
						printf("AT+CGREG?\r\n");//查看是否注册GPRS网络
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
		uint8_t time_flag = 0;
		printf("AT+QICSGP=1,1,\042CMNET\042,\042\042,\042\042,0\r\n");//接入APN,无用户名和密码
		//printf("AT+QICSGP=1,1,UNINET,0,0,0\r\n");//接入APN,无用户名和密码
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//开启成功
				while(strx == NULL)
				{
						Clear_Buffer();	
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						printf("AT+QICSGP=1,1,\042CMNET\042,\042\042,\042\042,0\r\n");//接入APN,无用户名和密码
						//printf("AT+QICSGP=1,1,UNINET,0,0,0\r\n");//接入APN,无用户名和密码
						osDelay(50);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//开启成功
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

void DeactivationSystem(void)
{
		uint8_t time_flag = 0;
		printf("AT+QIDEACT=1\r\n");//去激活
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//开启成功
				while(strx == NULL)
				{
						Clear_Buffer();	
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						printf("AT+QIDEACT=1\r\n");//去激活
						osDelay(50);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//开启成功
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

void ActivationSystem(void)
{
		uint8_t time_flag = 0;
		printf("AT+QIACT=1\r\n");//激活
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//开启成功
				while(strx == NULL)
				{
						Clear_Buffer();	
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						printf("AT+QIACT=1\r\n");//激活
						osDelay(50);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//开启成功
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

void ConfirmSendData(void)
{
		g_fix_data[0] = 0x01A;
	
    HAL_UART_Transmit(&huart2, g_fix_data, 1, 0XFF);//发送完成函数
    while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void SendDateDone(char *bufferdata)
{
		uint8_t time_flag = 0;

		printf(bufferdata);
		ConfirmSendData();
		osDelay(5);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx=strstr((const char*)usart2_rx_buf,(const char*)"SEND OK");//开启成功
				while(strx == NULL)
				{
						Clear_Buffer();	
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						printf(bufferdata);
						ConfirmSendData();
						osDelay(5);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"SEND OK");//开启成功
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


void LinkFirstTCPSocket(void)
{
		uint16_t time = 0;
		printf("AT+QIOPEN=1,0,\042TCP\042,\04239.106.226.214\042,11001,0,1\r\n");//这里是需要登录的IP号码，采用直接吐出模式
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
				{
					osDelay(5);
					time++;
					if (time >= 30000)
					{
						Clear_Buffer();	
						printf("AT+QICLOSE=0\r\n");
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						time = 0;
						while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
						{
							osDelay(5);
							time++;
							if (time >= 2000)
							{
								osTimerStart(RebootEcTimeHandle,10000);
							}
						}
					}
				}
				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
				time = 0;
				while(!strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 0,0"))
				{
						osDelay(5);
						time++;
						if (time >= 30000)
						{
							Clear_Buffer();	
							printf("AT+QICLOSE=0\r\n");
							HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
							time = 0;
							while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
							{
								 osDelay(5);
								 time++;
								if (time >= 2000)
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
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void LinkSecondTCPSocket(void)
{
		uint16_t time = 0;
		printf("AT+QIOPEN=1,1,\042TCP\042,\04239.106.226.214\042,11002,0,1\r\n");//这里是需要登录的IP号码，采用直接吐出模式
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
				{
					osDelay(5);
					time++;
					if (time >= 30000)
					{
						Clear_Buffer();	
						printf("AT+QICLOSE=1\r\n");//关闭socket连接
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						time = 0;
						while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
						{
							osDelay(5);
							time++;
							if (time >= 2000)
							{
								osTimerStart(RebootEcTimeHandle,10000);
							}
						}
					}
				}
				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
				time = 0;
				while(!strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 1,0"))
				{
						osDelay(5);
						time++;
						if (time >= 30000)
						{
							Clear_Buffer();	
							printf("AT+QICLOSE=1\r\n");//关闭socket连接
							HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
							time = 0;
							while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
							{
								 osDelay(5);
								 time++;
								if (time >= 2000)
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
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void LinkThirdTCPSocket(void)
{
		uint16_t time = 0;
		printf("AT+QIOPEN=1,2,\042TCP\042,\04239.106.226.214\042,11003,0,1\r\n");//这里是需要登录的IP号码，采用直接吐出模式
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
				{
					osDelay(5);
					time++;
					if (time >= 30000)
					{
						Clear_Buffer();	
						printf("AT+QICLOSE=2\r\n");//关闭socket连接
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						time = 0;
						while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
						{
							osDelay(5);
							time++;
							if (time >= 2000)
							{
								osTimerStart(RebootEcTimeHandle,10000);
							}
						}
					}
				}
				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
				time = 0;
				while(!strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 2,0"))
				{
						osDelay(5);
						time++;
						if (time >= 30000)
						{
							Clear_Buffer();	
							printf("AT+QICLOSE=2\r\n");//关闭socket连接
							HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
							time = 0;
							while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
							{
								 osDelay(5);
								 time++;
								if (time >= 2000)
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
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void LinkFourthTCPSocket(void)
{
		uint16_t time = 0;
		printf("AT+QIOPEN=1,3,\042TCP\042,\04239.106.226.214\042,11004,0,1\r\n");//这里是需要登录的IP号码，采用直接吐出模式
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
				{
					osDelay(5);
					time++;
					if (time >= 30000)
					{
						Clear_Buffer();	
						printf("AT+QICLOSE=3\r\n");//关闭socket连接
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						time = 0;
						while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
						{
							osDelay(5);
							time++;
							if (time >= 2000)
							{
								osTimerStart(RebootEcTimeHandle,10000);
							}
						}
					}
				}
				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
				time = 0;
				while(!strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 3,0"))
				{
						osDelay(5);
						time++;
						if (time >= 30000)
						{
							Clear_Buffer();	
							printf("AT+QICLOSE=3\r\n");//关闭socket连接
							HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
							time = 0;
							while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
							{
								 osDelay(5);
								 time++;
								if (time >= 2000)
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
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

//void LinkFourthTCPSocket(void)
//{
//				uint8_t time_flag = 0;
//		printf("AT+QIOPEN=1,3,\042TCP\042,\04239.106.226.214\042,11004,0,1\r\n");//这里是需要登录的IP号码，采用直接吐出模式
//		osDelay(50);
//		if (usart2_flag)
//		{
//				usart2_flag = 0;
//				strx=strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 3,0");//检查是否登录成功
//				osDelay(50);
//				while(strx == NULL)
//				{
//						Clear_Buffer();	
//						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//						printf("AT+QICLOSE=3\r\n");//关闭socket连接
//						while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
//						{
//							 osDelay(5);
//						}
//						Clear_Buffer();	
//						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//						printf("AT+QIOPEN=1,3,\042TCP\042,\04239.106.226.214\042,11004,0,1\r\n");//这里是需要登录的IP号码，采用直接吐出模式
//						osDelay(50);
//						strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//检查是否登录成功
//						if (strx != NULL)
//						{
//								HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//								osDelay(300);
//								strx=strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 3,0");//检查是否登录成功
//						}
//						if ((strx == NULL)&&(time_flag == 0))
//						{
//								time_flag =1;
//								osTimerStart(RebootEcTimeHandle,150000);
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

void  EC20_Init(void)
{
		CheckWhetherReturnOK();
		printf("ATE0\r\n"); //关闭回显
		osDelay(50);
		Clear_Buffer();	
		printf("AT+CSQ\r\n"); //检查CSQ
		osDelay(50);
		Clear_Buffer();
		printf("ATI\r\n"); //检查模块的版本号
		osDelay(50);
		Clear_Buffer();	
		CheckSIM();
//		CheckGSMInternet();
//		CheckGPRSInternet();
		printf("AT+COPS?\r\n");//查看注册到哪个运营商，支持移动,联通，电信
		osDelay(50);
		Clear_Buffer();
		printf("AT+QICLOSE=0\r\n");//关闭socket连接
    osDelay(50);
		Clear_Buffer();
		AccessAPN();
		printf("AT+QICSGP=1\r\n");
		osDelay(50);
		Clear_Buffer();
		DeactivationSystem();
		ActivationSystem();
}

//发送字符型数据
void EC20Send_StrData(char *bufferdata)
{
		uint8_t untildata = 0xff;
	
		printf("AT+QISEND=0\r\n");
		osDelay(10);
		SendDateDone(bufferdata);
    Clear_Buffer();
    printf("AT+QISEND=0,0\r\n");
    osDelay(5);
    strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//发送剩余字节数据
    while(untildata)
    {
				printf("AT+QISEND=0,0\r\n");
				osDelay(20);
				strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//发送剩余字节数据
				strx=strstr((char*)strx,(char*)",");//获取第一个
				strx=strstr((char*)(strx+1),(char*)",");//获取第二个
				untildata=*(strx+1)-0x30;
				Clear_Buffer();
				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
    }
    Clear_Buffer();
}

//发送多路字符型数据
void EC20Send_MultiStrData(uint8_t channel ,char *bufferdata)
{
    uint8_t untildata=0xff;
		g_fix_data[0] = 0x1a;
    printf("AT+QISEND=%c\r\n",channel);
    osDelay(100);
    printf(bufferdata);
    osDelay(100);	
    HAL_UART_Transmit(&huart2, g_fix_data, 1, 0XFF);//发送完成函数
    while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
    osDelay(100);
    strx=strstr((char*)usart2_rx_buf,(char*)"SEND OK");//是否正确发送
    while(strx==NULL)
    {
        strx=strstr((char*)usart2_rx_buf,(char*)"SEND OK");//是否正确发送
        osDelay(10);
    }
    osDelay(100);
    Clear_Buffer();
    printf("AT+QISEND=%c,0\r\n",channel);
    osDelay(200);
    strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//发送剩余字节数据
    while(untildata)
    {
        printf("AT+QISEND=%c,0\r\n",channel);
        osDelay(200);
        strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//发送剩余字节数据
        strx=strstr((char*)strx,(char*)",");//获取第一个
        strx=strstr((char*)(strx+1),(char*)",");//获取第二个
        untildata=*(strx+1)-0x30;
        Clear_Buffer();
    }
}

//发送十六进制数据
void EC20Send_HexData(char *bufferdata)
{
    uint8_t untildata = 0xff;
		g_fix_data[0] = 0x1a;
    printf("AT+QISENDEX=0,\042%s\042\r\n",bufferdata);
    osDelay(100);
    HAL_UART_Transmit(&huart2, g_fix_data, 1, 0XFF);//发送完成函数
    while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
    osDelay(100);
    strx=strstr((char*)usart2_rx_buf,(char*)"OK");//是否正确发送
    while(strx==NULL)
    {
        strx=strstr((char*)usart2_rx_buf,(char*)"OK");//是否正确发送
        osDelay(10);
    }
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
    osDelay(100);
    Clear_Buffer();
    printf("AT+QISEND=0,0\r\n");
    osDelay(200);
    strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//发送剩余字节数据
    while(untildata)
    {
				printf("AT+QISEND=0,0\r\n");
				osDelay(200);
				strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//发送剩余字节数据
				strx=strstr((char*)strx,(char*)",");//获取第一个
				strx=strstr((char*)(strx+1),(char*)",");//获取第二个
				untildata=*(strx+1)-0x30;
				Clear_Buffer();
    }
}


#endif

