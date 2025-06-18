/*
 * drv_ads8681.h
 *
 * Created: 2025-04-09 오전 12:32:41
 *  Author: bruno
 */ 


#ifndef DRV_ADS8681_H_
#define DRV_ADS8681_H_

#include "main.h"

void ADS8681_Reset(void);
void ADS8681_Initial(void);
void ADS8681_Write_Register(uint8_t address, uint16_t data);
uint16_t ADS8681_Read_Conversion_Data(void);
uint16_t AD_CONV(uint16_t *buffer);
uint16_t AD_Voltage(float gain);
uint32_t AD_Resistor(uint8_t outStat);
uint8_t ADC_Read(uint8_t selectAD, uint8_t selectMux8, uint8_t *buffer);
uint8_t AD_Select(uint8_t *buffer);

#endif /* DRV_ADS8681_H_ */