/******************** (C) COPYRIGHT 2016 CETC27-CETIIC-ZYGD ********************
* File Name          : EC20.c
* Author             : yangchuang
* Version            : V1.0
* Date               : 19/06/2019
* Description        : the file include all ec20 driver and applycation function 
*******************************************************************************/
#ifndef __EC20_H__
#define __EC20_H__

#include "stm32f1xx_hal_usart.h"
#include "usart.h"
#include "gpio.h"

void Ec20PowerOn(void);
void Ec20PowerOff(void);
void Ec20Reset(void);
void Clear_Buffer(void);
void LinkFristTCPSocket(void);
void LinkSecondTCPSocket(void);
void LinkThirdTCPSocket(void);
void LinkFourthTCPSocket(void);

void  EC20_Init(void);
void EC20Send_StrData(char *bufferdata);
void EC20Send_HexData(char *bufferdata);	
void EC20Send_RecAccessMode(void);
//void EC20Send_ChangeMode(u8 data);
void EC20Send_MultiStrData(uint8_t channel ,char *bufferdata);

#endif
