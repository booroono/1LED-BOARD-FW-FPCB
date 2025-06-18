/*
 * drv_ads8681.c
 *
 * Created: 2025-04-09 오전 12:28:49
 *  Author: bruno
 */ 

#include "drv_ads8681.h"

void ADS8681_Reset(void) {										// ad reset
	gpio_set_pin_level(GPIO_AD_RST, false);
	delay_ms(30);
	gpio_set_pin_level(GPIO_AD_RST, true);
	delay_ms(30);
}

void ADS8681_Initial(void) {										// ad initial
	ADS8681_Reset();
	ADS8681_Write_Register(0x14, 0x000b);					// ref 1.25V
}

void ADS8681_Write_Register(uint8_t address, uint16_t data) {
	uint16_t retry = 0;
	
	spiTxBuffer[0] = 0xD0;
	spiTxBuffer[1] = address;
	spiTxBuffer[2] = (data >> 8) & 0xFF;
	spiTxBuffer[3] = data & 0xFF;

	gpio_set_pin_level(GPIO_AD_CS, true);
	delay_us(1);
	gpio_set_pin_level(GPIO_AD_CS, false);
	delay_us(1);

	while(gpio_get_pin_level(GPIO_AD_RVS)) {
		retry++;
		if(retry >= 0XFFFE) {break;}
	}
	io_write(SPI_io, spiTxBuffer, 4);
	gpio_set_pin_level(GPIO_AD_CS, true);
}

uint16_t ADS8681_Read_Conversion_Data(void) {
	uint16_t data, retry = 0;
	
	gpio_set_pin_level(GPIO_AD_CS, false);
	delay_us(1);
	gpio_set_pin_level(GPIO_AD_CS, true);
	delay_us(1);
	while(!gpio_get_pin_level(GPIO_AD_RVS)) {
		retry++;
		if(retry >= 0XFFFE) {break;}
	}
	gpio_set_pin_level(GPIO_AD_CS, false);
	delay_us(1);
	retry = 0;
	while(gpio_get_pin_level(GPIO_AD_RVS)) {
		retry++;
		if(retry >= 0XFFFE) {break;}
	}
	io_read(SPI_io, spiRxBuffer, 4);
	gpio_set_pin_level(GPIO_AD_CS, true);

	data = (spiRxBuffer[0] << 8) | spiRxBuffer[1];

	return data;
}

uint16_t AD_CONV (uint16_t *buffer) {				// ad filter
	uint8_t i = 0, count = AD_CNT;
	uint16_t maxValue1 = 0x0, maxValue2 = 0x0/*, MAX_Value3 = 0x0*/;
	uint16_t minValue1 = 0xFFFF, minValue2 = 0xFFFF/*, MIN_Value3 = 0xFFFF*/;
	uint16_t readAdcValue;
	uint32_t sum = 0;
	
	for(i = 0; i < count; i++) {
		readAdcValue = *buffer++;
		sum += readAdcValue;
 		if(readAdcValue > maxValue1) {maxValue1 = readAdcValue;}
		else if(readAdcValue > maxValue2) {maxValue2 = readAdcValue;}
		else if(readAdcValue < minValue1) {minValue1 = readAdcValue;}
		else if(readAdcValue < minValue2) {minValue2 = readAdcValue;}
	}
	
	if(maxValue1 != 0x0) {count--;}
	else {maxValue1 = 0;}
 	if(maxValue2 != 0x0) {count--;}
 	else {maxValue2 = 0;}
	if(minValue1 != 0xFFFF) {count--;}
	else {minValue1 = 0;}		
 	if(minValue2 != 0xFFFF) {count--;}
 	else {minValue2 = 0;}	
		
	sum = ((((sum - maxValue1) - maxValue2) - minValue1) - minValue2) / count;
	
 	return (uint16_t)sum; 
}

uint16_t AD_Voltage(float gain) {
	volatile uint16_t readValue;
	uint16_t readBuffer[AD_CNT];

	for(uint8_t i = 0; i < AD_CNT; i++) {
		readBuffer[i] =  ADS8681_Read_Conversion_Data();
		delay_ms(1);
	}
	readValue = AD_CONV(readBuffer);
	readValue = (float)(readValue) * gain;

	return readValue;
}

uint32_t AD_Resistor(uint8_t outStat) {
	volatile uint16_t readValue, refV = 5000, offset;
	volatile uint32_t refR = 1;
	volatile float adValue, span;
	uint16_t readBuffer[AD_CNT];
  
  if(outStat == MUX8_R_1K) {refR = AD_REF_R_1K;}
	else if(outStat == MUX8_R_10K) {refR = AD_REF_R_10K;}
	else if(outStat == MUX8_R_100K) {refR = AD_REF_R_100K;}
	else if(outStat == MUX8_R_1M) {refR = AD_REF_R_1M;}
	else if(outStat == MUX8_R_10M) {refR = AD_REF_R_10M;}
  
  for(uint8_t i = 0; i < AD_CNT; i++) {
		readBuffer[i] =  ADS8681_Read_Conversion_Data();
	}	
	readValue = AD_CONV(readBuffer);
	adValue = readValue * AD_GAIN_V_5_1_2;
	adValue = adValue / ((refV - adValue) / refR);
  if(adValue > 10000000) {adValue = 10000000;}    
  if(adValue <= 100) {
    span = AD_R_SPAN_100;
    offset = AD_R_OFFSET_100;
  } else if((adValue > 100) && (adValue <= 4500)) {
    span = AD_R_SPAN_4500;
    offset = AD_R_OFFSET_4500;
  } else if((adValue > 4500) && (adValue <= 119000)) {
    span = AD_R_SPAN_119K;
    offset = AD_R_OFFSET_119K;
  } else if((adValue > 119000) && (adValue <= 318000)) {
    span = AD_R_SPAN_318K;
    offset = AD_R_OFFSET_318K;
  } else if((adValue > 318000) && (adValue <= 2000000)) {
    span = AD_R_SPAN_2M;
    offset = AD_R_OFFSET_2M;
  } else if((adValue > 2000000)) {
    span = AD_R_SPAN_10M;
    offset = AD_R_OFFSET_10M;
  }
  adValue = ((float)adValue * span) + offset;    
    
 	return (uint32_t)adValue;
}

uint8_t ADC_Read(uint8_t selectAD, uint8_t mux8, uint8_t *buffer) {
	uint8_t resultLength = 2;
	uint16_t u16Buffer = 0;
	
	MUX_Control(SEL_MUX_AD, selectAD);
	switch (selectAD) {
		case SEL_AD_MUX8 :
			resultLength = AD_Select(buffer);
			break;
		case SEL_AD_MUX4 :
		case SEL_AD_ETC :
		case SEL_AD_CC_1_8V :
		case SEL_AD_CC_3_3V :
		case SEL_AD_CDS1 :
		case SEL_AD_CDS2 :
		case SEL_AD_CDS3 :
			if(ccMode == true) {u16Buffer = AD_Voltage(AD_GAIN_V_5_1_2_0);}
			else {u16Buffer = AD_Voltage(AD_GAIN_V_5_1_2);}
			buffer[0] = u16Buffer >> 8;
			buffer[1] = u16Buffer & 0xFF;
			break;
		default:
			break;
	}

	return resultLength;
}

uint8_t AD_Select(uint8_t *buffer) {				// mux8 switch
	uint8_t resultLength = 4;
	uint16_t result16;
	uint32_t result32;
	out_state_t *out = &outState;

	switch (out->ref1) {
		case MUX8_R_1K :
		case MUX8_R_10K :
		case MUX8_R_100K :
		case MUX8_R_1M :
		case MUX8_R_10M :
			result32 = AD_Resistor(out->ref1);
			buffer[0] = result32 >> 24;
			buffer[1] = result32 >> 16;
			buffer[2] = result32 >> 8;
			buffer[3] = result32 & 0xFF;
			break;
	 default :
			resultLength = 2;
			if(ccMode == true) {result16 = AD_Voltage(AD_GAIN_V_5_1_2_0);}
			else {result16 = AD_Voltage(AD_GAIN_V_5_1_2);}
			buffer[0] = result16 >> 8;
			buffer[1] = result16 & 0xFF;
			break;
	}
	
	return resultLength;
}