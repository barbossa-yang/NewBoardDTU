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
extern volatile uint16_t usart2_rx_len;
extern volatile uint16_t usart1_rx_len; 
extern volatile uint8_t usart2_flag;
extern osMutexId RebootEcTimeHandle;

uint8_t g_fix_data[1];

/**************************��Ӧatָ���е�4.1�½�*******************************/
char g_Configure_Context[40];
char g_Activate_Context[20];
char g_Deactivate_Context[20];
/**************************��Ӧatָ���е�4.2�½�*******************************/
char g_TcpClientConnAccessMode[50];
char g_SendDataInAccessMode[20];
char g_RecDataInAccessMode[50];
char g_CloseConnAccessMode[20];
/**************************��Ӧatָ���е�4.3�½�*******************************/
char g_TCPClientConnTransMode[20];
uint8_t g_SentDataInTransMode[20];
uint8_t g_RecDataInTransMode[20];
char g_CloseTcpClientTransMode[20];
/**************************��Ӧatָ���е�4.4�½�*******************************/
char g_SetClientConnInPushMode[20];
char g_SendDataInPushMode[20];
char g_RecDataInPushMode[20];
char g_CloseTcpClientPushMode[20];
/**************************��Ӧatָ���е�4.5�½�*******************************/
char g_StartTcpServer[50];
char g_AcceptTcpIncomConn[20];
char g_RecDataFromIncomConnect[50];
char g_CloseTcpServ[20];
/**************************��Ӧatָ���е�4.6�½�*******************************/
char g_StartUdpServ[50];
char g_SendUdpDataToRemote[20];
char g_RecDataFromRemote[50];
char g_CloseUdpServ[20];
/**************************��Ӧatָ���е�4.7�½�*******************************/
char g_Ping[30];
/**************************��Ӧatָ���е�4.8�½�*******************************/
char g_SynchronizeLocalTime[30];
/**************************��Ӧatָ���е�4.9�½�*******************************/
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

void Clear_Buffer(void)//��ջ���
{                              
//		HAL_UART_Transmit(&huart2, usart2_rx_buf, usart2_rx_len, 0xFF);
		memset(usart2_rx_buf, 0, USART_MAX_DATA_LEN);
		usart2_rx_len=0;
}
void CheckWhetherReturnOK(void)
{
		uint8_t time_flag = 0;
		char str[] = {"AT\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx = strstr((const char*)usart2_rx_buf,(const char*)"RDY");//����OK
				while(strx == NULL)
				{
						Clear_Buffer();	
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
						osDelay(50);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//����OK
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
		char str[] = {"AT+CSQ\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx = strstr((const char*)usart2_rx_buf,(const char*)"99");//�鿴�Ƿ񷵻�ready
				extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CSQ: OK");//��������������
				while(~((strx == NULL)&&(extstrx != NULL)))
				{
						Clear_Buffer();	
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
						osDelay(50);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"99");//check signal quality 
						extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CSQ: OK");//��������������
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
		char str[] = {"AT+CPIN?\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx = strstr((const char*)usart2_rx_buf,(const char*)"+CPIN: READY");//�鿴�Ƿ񷵻�ready
				while(strx == NULL)
				{
						Clear_Buffer();	
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
						osDelay(50);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"+CPIN: READY");//���SIM���Ƿ���λ���ȴ�����λ�������ʶ�𲻵���ʣ��Ĺ�����û������
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
		char str[] = {"AT+CREG?\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		osDelay(200);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx=strstr((const char*)usart2_rx_buf,(const char*)"+CREG: 0,1");//��������
				extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CREG: 0,5");//��������������
				while((strx == NULL)&&(extstrx == NULL))
				{
						Clear_Buffer();
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
						osDelay(200);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"+CREG: 0,1");//��������
						extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CREG: 0,5");//��������������
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
				strx=strstr((const char*)usart2_rx_buf,(const char*)"+CGREG: 0,1");//������Ҫ��ֻ��ע��ɹ����ſ��Խ���GPRS���ݴ���
				extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CGREG: 0,5");//��������������
				while((strx == NULL)&&(extstrx == NULL))
				{
						Clear_Buffer();
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
						osDelay(200);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"+CGREG: 0,1");//������Ҫ��ֻ��ע��ɹ����ſ��Խ���GPRS���ݴ���
						extstrx=strstr((const char*)usart2_rx_buf,(const char*)"+CGREG: 0,5");//��������������
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
		char str[] = {"AT+QICSGP=1,1,\042CMNET\042,\042\042,\042\042,0\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		//printf("AT+QICSGP=1,1,UNINET,0,0,0\r\n");//����APN,���û���������
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//�����ɹ�
				while(strx == NULL)
				{
						Clear_Buffer();	
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
						//printf("AT+QICSGP=1,1,UNINET,0,0,0\r\n");//����APN,���û���������
						osDelay(50);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//�����ɹ�
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
		char str[] = {"AT+QIDEACT=1\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//�����ɹ�
				while(strx == NULL)
				{
						Clear_Buffer();	
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
						osDelay(50);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//�����ɹ�
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
		char str[] = {"AT+QIACT=1\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
		osDelay(50);
		if (usart2_flag)
		{
				usart2_flag = 0;
				strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//�����ɹ�
				while(strx == NULL)
				{
						Clear_Buffer();	
						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
						osDelay(50);
						strx=strstr((const char*)usart2_rx_buf,(const char*)"OK");//�����ɹ�
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
	
    HAL_UART_Transmit_DMA(&huart2, g_fix_data, 1);//������ɺ���
    while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
    {
    }
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

void SendDateDone(char *bufferdata)
{
		uint16_t time = 0;
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&bufferdata, usart1_rx_len);
		ConfirmSendData();
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
		while(!strstr((const char*)usart2_rx_buf,(const char*)"SEND OK"))
		{
			osDelay(5);
			time++;
			if (time >= 30000)
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
//				strx=strstr((const char*)usart2_rx_buf,(const char*)"SEND OK");//�����ɹ�
//				while(strx == NULL)
//				{
//						Clear_Buffer();	
//						HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//						printf(bufferdata);
//						ConfirmSendData();
//						osDelay(5);
//						strx=strstr((const char*)usart2_rx_buf,(const char*)"SEND OK");//�����ɹ�
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
		uint16_t time = 0;
		char str[] = {"AT+QIOPEN=1,0,\042TCP\042,\04239.106.226.214\042,11001,0,1\r\n"};
		char str1[] = {"AT+QICLOSE=0\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
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
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
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
							HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
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
		printf("the first channel connect\r\n");
}

void LinkSecondTCPSocket(void)
{
		uint16_t time = 0;
		char str[] = {"AT+QIOPEN=1,1,\042TCP\042,\04239.106.226.214\042,11002,0,1\r\n"};
		char str1[] = {"AT+QICLOSE=1\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
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
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
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
							HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
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
		printf("the second channel connect\r\n");
}

void LinkThirdTCPSocket(void)
{
		uint16_t time = 0;
		char str[] = {"AT+QIOPEN=1,2,\042TCP\042,\04239.106.226.214\042,11003,0,1\r\n"};
		char str1[] = {"AT+QICLOSE=2\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
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
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
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
							HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
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
		printf("the third channel connect\r\n");
}

void LinkFourthTCPSocket(void)
{
		uint16_t time = 0;
		char str[] = {"AT+QIOPEN=1,3,\042TCP\042,\04239.106.226.214\042,11004,0,1\r\n"};
		char str1[] = {"AT+QICLOSE=3\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
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
						HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
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
							HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
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
		printf("the fourth channel connect\r\n");
}

void  EC20_Init(void)
{
	char str0[] = {"ATE0\r\n"};
	char str1[] = {"AT+CSQ\r\n"};
	char str2[] = {"ATI\r\n"};
	char str3[] = {"AT+COPS?\r\n"};
	char str4[] = {"AT+QICLOSE=0\r\n"};
	char str5[] = {"AT+QICSGP=1\r\n"};
	
		CheckWhetherReturnOK();
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str0, strlen(str0));
		osDelay(50);
		Clear_Buffer();	
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
		osDelay(50);
		Clear_Buffer();
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str2, strlen(str2));
		osDelay(50);
		Clear_Buffer();	
		CheckSIM();
//		CheckGSMInternet();
//		CheckGPRSInternet();
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str3, strlen(str3));
		osDelay(50);
		Clear_Buffer();
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str4, strlen(str4));
    osDelay(50);
		Clear_Buffer();
		AccessAPN();
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str5, strlen(str5));
		osDelay(50);
		Clear_Buffer();
		DeactivationSystem();
		ActivationSystem();
}

void EC20Send_StrData(char *bufferdata)
{
		uint8_t untildata = 0xff;
		char str0[] = {"AT+QISEND=0\r\n"};
		char str1[] = {"AT+QISEND=0,0\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str0, strlen(str0));
		while(!strstr((char*)usart2_rx_buf,(char*)">"))
		{
			osDelay(5);
		}
		SendDateDone(bufferdata);
    Clear_Buffer();
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
    osDelay(5);
    strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//����ʣ���ֽ�����
    while(untildata)
    {
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
				osDelay(20);
				strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//����ʣ���ֽ�����
				strx=strstr((char*)strx,(char*)",");//��ȡ��һ��
				strx=strstr((char*)(strx+1),(char*)",");//��ȡ�ڶ���
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
		while(!strstr((char*)usart2_rx_buf,(char*)">"))
		{
			osDelay(5);
		}
		SendDateDone(bufferdata);
    Clear_Buffer();
   HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
    osDelay(5);
    strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//����ʣ���ֽ�����
    while(untildata)
    {
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
				osDelay(20);
				strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//����ʣ���ֽ�����
				strx=strstr((char*)strx,(char*)",");//��ȡ��һ��
				strx=strstr((char*)(strx+1),(char*)",");//��ȡ�ڶ���
				untildata=*(strx+1)-0x30;
				Clear_Buffer();
				HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
    }
}

//����ʮ����������
//void EC20Send_HexData(char *bufferdata)
//{
//    uint8_t untildata = 0xff;
//		g_fix_data[0] = 0x1a;
//    printf("AT+QISENDEX=0,\042%s\042\r\n",bufferdata);
//    osDelay(100);
//    HAL_UART_Transmit(&huart2, g_fix_data, 1, 0XFF);//������ɺ���
//    while(__HAL_UART_GET_FLAG(&huart2, USART_FLAG_TC) == RESET)
//    {
//    }
//    osDelay(100);
//    strx=strstr((char*)usart2_rx_buf,(char*)"OK");//�Ƿ���ȷ����
//    while(strx==NULL)
//    {
//        strx=strstr((char*)usart2_rx_buf,(char*)"OK");//�Ƿ���ȷ����
//        osDelay(10);
//    }
//		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//    osDelay(100);
//    Clear_Buffer();
//    printf("AT+QISEND=0,0\r\n");
//    osDelay(200);
//    strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//����ʣ���ֽ�����
//    while(untildata)
//    {
//				printf("AT+QISEND=0,0\r\n");
//				osDelay(200);
//				strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//����ʣ���ֽ�����
//				strx=strstr((char*)strx,(char*)",");//��ȡ��һ��
//				strx=strstr((char*)(strx+1),(char*)",");//��ȡ�ڶ���
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
		uint16_t time = 0;
		char str0[] = {"AT+QIOPEN=1,4,\042UDP SERVICE\042,\042127.0.0.1\042,11005,0,1\r\n"};
		char str1[] = {"AT+QICLOSE=4\r\n"};
		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str0, strlen(str0));
		while(!strstr((const char*)usart2_rx_buf,(const char*)"OK"))
		{
			osDelay(5);
			time++;
			if (time >= 30000)
			{
				Clear_Buffer();	
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
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
		while(!strstr((const char*)usart2_rx_buf,(const char*)"+QIOPEN: 4,0"))
		{
				osDelay(5);
				time++;
				if (time >= 30000)
				{
					Clear_Buffer();	
					HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
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
		Clear_Buffer();	
		HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
		printf("UDP channel start\r\n");
}

void SendUdpDate(void)
{
	uint8_t untildata = 0xff;
	char buffer[5] = {'y','a','n','g'};
	char str0[] = {"AT+QISEND=4,5,\04239.106.226.214\042,11005\r\n"};
	char str1[] = {"AT+QISEND=4,0\r\n"};
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str0, strlen(str0));
	while(!strstr((char*)usart2_rx_buf,(char*)">"))
	{
		osDelay(5);
	}
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&buffer, strlen(buffer));
	Clear_Buffer();
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
	osDelay(5);
	strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//����ʣ���ֽ�����
	while(untildata)
	{
			HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str1, strlen(str1));
			osDelay(20);
			strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//����ʣ���ֽ�����
			strx=strstr((char*)strx,(char*)",");//��ȡ��һ��
			strx=strstr((char*)(strx+1),(char*)",");//��ȡ�ڶ���
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
//	strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//����ʣ���ֽ�����
//	while(untildata)
//	{
//			printf("AT+QISEND=4,0\r\n");
//			osDelay(20);
//			strx=strstr((char*)usart2_rx_buf,(char*)"+QISEND:");//����ʣ���ֽ�����
//			strx=strstr((char*)strx,(char*)",");//��ȡ��һ��
//			strx=strstr((char*)(strx+1),(char*)",");//��ȡ�ڶ���
//			untildata=*(strx+1)-0x30;
//			Clear_Buffer();
//			HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
//    }
//}

void ReceiveUdpDate(void)
{
	char str[] = {"AT+QIRD=4\r\n"};
	HAL_UART_Transmit_DMA(&huart2, (uint8_t *)&str, strlen(str));
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
	while(!strstr((char*)usart2_rx_buf,(char*)"OK"))
	{
		osDelay(5);
	}
	Clear_Buffer();	
	HAL_UART_Receive_DMA(&huart2, usart2_rx_buf, USART_MAX_DATA_LEN);
}

#endif

