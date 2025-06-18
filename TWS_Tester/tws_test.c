/*
 * tws_test.c
 *
 * Created: 2025-04-09 오전 12:30:30
 *  Author: bruno
 */

#include "tws_test.h"

volatile uint8_t eqSide = 0, testStop = false, ccMode = 0, ledTestFlag = 0;

void Test_Mode(void) {
	volatile uint8_t stepNo, buffer, buffer2;
	volatile uint16_t resultLength = 3, u16Buffer1, u16Buffer2;
	int16_t i16Buffer, maxBuffer, minBuffer;
	uint32_t sumOpen = 0, sumClose = 0;
	uint8_t adBuffer[5], adBufferBk[5];
	cfg_data_t *cfg = &cfgData;
	usart_data_t *usart = &usartData;
	out_state_t *out = &outState;
	hall_sensor_result_t *hall = &hallSensorResult;
	prox_sensor_result_t *prox = &proxSensorResult;

	testStop = false;
	eqSide = usart->data[0];
	stepNo = usart->data[1];
	gpio_set_pin_level(GPIO_BUZ, false);
	switch (stepNo) {
		case STEP_CON_OS:
			if ((out->sol & (OUT_SOL_LED_X | OUT_SOL_POGO_LED_Y))) {
				Solenoid_Valve_Control(SOL_OFF, SOL_POGO_LED_Y);
				delay_ms(200);
				Solenoid_Valve_Control(SOL_OFF, SOL_LED_X);
				delay_ms(100);
			}
			buffer = (cfg->stepLength / 3) + 3;
			Solenoid_Valve_Control(SOL_ON, SOL_FPCB);
			Solenoid_Valve_Control(SOL_ON, SOL_DOWN);
      MUX_Control(SEL_MUX8, MUX8_R_1M);
      delay_ms(1000);
			for (uint8_t i = 3; i < buffer; i++) {
				buffer2 = i - 3;
				MUX_Control(SEL_MUX8, MUX8_R_100K);
				delay_ms(30);
				MUX_Control(SEL_MUX8, MUX8_R_1M);
				delay_ms(10);
        Out_Control(SEL_OUT_NEGATIVE, cfg->conn.negative[buffer2]);
        Out_Control(SEL_OUT_POSITIVE, cfg->conn.positve[buffer2]);
        if(cfg->conn.refernce[buffer2] == MUX8_R_10K) {delay_ms(100);}
        else if(cfg->conn.refernce[buffer2] == MUX8_R_100K) {delay_ms(200);}
        else if(cfg->conn.refernce[buffer2] == MUX8_R_1M) {delay_ms(300);}
        else if(cfg->conn.refernce[buffer2] == MUX8_R_10M) {delay_ms(500);}  
        ADC_Read(SEL_AD_MUX8, MUX8_R_1M, adBuffer);
      	usart->data[i] = adBuffer[3];
				resultLength += 1;
				if (testStop == true) {
					break;
				}
			}
			MUX_Control(SEL_MUX8, MUX_OFF);
			Out_Control(SEL_OUT_POSITIVE, OUT_OFF);
			Out_Control(SEL_OUT_NEGATIVE, OUT_OFF);
			break;
		case STEP_POGO_OS:
			if (!(out->sol & (OUT_SOL_FPCB | OUT_SOL_DOWN))) {
				Solenoid_Valve_Control(SOL_ON, SOL_FPCB);
				Solenoid_Valve_Control(SOL_ON, SOL_DOWN);
				delay_ms(950);
			}
			Solenoid_Valve_Control(SOL_ON, SOL_LED_X);
			delay_ms(450);
			Solenoid_Valve_Control(SOL_ON, SOL_POGO_LED_Y);
			delay_ms(450);
			Out_Control(SEL_OUT_NEGATIVE, cfg->pogo.gnd); // GND
			Out_Control(SEL_OUT_POSITIVE, cfg->pogo.j2); // J2
			MUX_Control(SEL_MUX8, MUX8_R_1M);
			ADC_Read(SEL_AD_MUX8, MUX8_R_1M, adBuffer);
			usart->data[resultLength++] = adBuffer[3];
			Out_Control(SEL_OUT_NEGATIVE, cfg->pogo.pogo); // POGO
			Out_Control(SEL_OUT_POSITIVE, cfg->pogo.j1); // J1
			ADC_Read(SEL_AD_MUX8, MUX8_R_1M, adBuffer);
			usart->data[resultLength++] = adBuffer[3];
			Out_Control(SEL_OUT_POSITIVE, OUT_OFF);
			Out_Control(SEL_OUT_NEGATIVE, OUT_OFF);
			Out_Control(SEL_OUT_POSITIVE, cfg->pogo.gnd); // GND
			Out_Control(SEL_OUT_NEGATIVE, cfg->pogo.j1); // J1
			delay_ms(250);
			ADC_Read(SEL_AD_MUX8, MUX8_R_1M, adBuffer);
			usart->data[resultLength++] = adBuffer[3];
			MUX_Control(SEL_MUX8, MUX_OFF);
			Out_Control(SEL_OUT_POSITIVE, OUT_OFF);
			Out_Control(SEL_OUT_NEGATIVE, OUT_OFF);
			break;
		case STEP_LED:
			if (!(out->sol & (OUT_SOL_FPCB | OUT_SOL_DOWN))) {
				Solenoid_Valve_Control(SOL_ON, SOL_FPCB);
				Solenoid_Valve_Control(SOL_ON, SOL_DOWN);
				delay_ms(950);
			}
			if (!(out->sol & (OUT_SOL_LED_X | OUT_SOL_POGO_LED_Y))) {
				Solenoid_Valve_Control(SOL_ON, SOL_LED_X);
				delay_ms(450);
				Solenoid_Valve_Control(SOL_ON, SOL_POGO_LED_Y);
				delay_ms(450);
			}
			MUX_Control(SEL_MUX8, MUX8_P_3_3V);
			ADC_Read(SEL_AD_MUX8, MUX8_P_3_3V, adBuffer); // v check
			MUX_Control(SEL_MUX8, MUX_OFF);
			u16Buffer1 = (adBuffer[0] << 8) + (adBuffer[1] & 0xFF);
			if ((u16Buffer1 < 3200) || (u16Buffer1 > 3400)) {
				testStop = true;
			} else {
				memcpy(&usart->data[resultLength], &adBuffer[0], 2);
				resultLength += 2;
				Out_Control(SEL_OUT_NEGATIVE, cfg->led.gnd); // led on check
				Out_Control(SEL_OUT_POSITIVE, cfg->led.led);
				MUX_Control(SEL_MUX8, MUX8_P_3_3V);
				ADC_Read(SEL_AD_MUX8, MUX8_P_3_3V, adBuffer); // led on v check	
				memcpy(&adBufferBk[0], &adBuffer[0], 2);
				ADC_Read(SEL_AD_CDS1, 0, adBuffer);
				memcpy(&usart->data[resultLength], &adBuffer[0], 2);
				resultLength += 2;
				MUX_Control(SEL_MUX8, MUX_OFF);
				Out_Control(SEL_OUT_POSITIVE, OUT_OFF); // led off check
				Out_Control(SEL_OUT_NEGATIVE, OUT_OFF);
				ADC_Read(SEL_AD_CDS1, 0, adBuffer);
				MUX_Control(SEL_MUX_AD, SEL_AD_MUX8);
				memcpy(&usart->data[resultLength], &adBuffer[0], 2);
				resultLength += 2;
				memcpy(&usart->data[resultLength], &adBufferBk[0], 2);
				resultLength += 2;
				Out_Control(SEL_OUT_NEGATIVE, cfg->led.gnd); // led gnd short check
				Out_Control(SEL_OUT_POSITIVE, cfg->led.led);
				MUX_Control(SEL_MUX8, MUX8_R_100K);
				delay_ms(20);
				MUX_Control(SEL_MUX8, MUX8_R_1M);
				delay_ms(20);
				ADC_Read(SEL_AD_MUX8, MUX8_R_1M, adBuffer);
        usart->data[resultLength++] = adBuffer[3];
				MUX_Control(SEL_MUX8, MUX_OFF);
				Out_Control(SEL_OUT_POSITIVE, OUT_OFF);
				Out_Control(SEL_OUT_NEGATIVE, OUT_OFF);
				Solenoid_Valve_Control(SOL_OFF, SOL_POGO_LED_Y);
				delay_ms(200);
				Solenoid_Valve_Control(SOL_OFF, SOL_LED_X);
			}
			break;
		case STEP_HALL_SENSOR:
			if ((out->sol & (OUT_SOL_LED_X | OUT_SOL_POGO_LED_Y))) {
				Solenoid_Valve_Control(SOL_OFF, SOL_POGO_LED_Y);
				delay_ms(200);
				Solenoid_Valve_Control(SOL_OFF, SOL_LED_X);
				delay_ms(100);
			}
			if (!(out->sol & (OUT_SOL_FPCB | OUT_SOL_DOWN))) {
				Solenoid_Valve_Control(SOL_ON, SOL_FPCB);
				Solenoid_Valve_Control(SOL_ON, SOL_DOWN);
				delay_ms(950);
			}
			MUX_Control(SEL_MUX8, MUX8_AMP_1_8V);
			ADC_Read(SEL_AD_MUX8, MUX8_AMP_1_8V, adBuffer);
			MUX_Control(SEL_MUX8, MUX_OFF);
			u16Buffer1 = (adBuffer[0] << 8) + (adBuffer[1] & 0xFF);
			if ((u16Buffer1 < 1700) || (u16Buffer1 > 1900)) {
				testStop = true;
			} else {
				memcpy(&usart->data[resultLength], &adBuffer[0], 2);
				resultLength += 2;
				Out_Control(SEL_OUT_ETC_X, cfg->hall.scl); // i2c
				Out_Control(SEL_OUT_ETC_Y, cfg->hall.sda);
				MUX_Control(SEL_MUX2_4, MUX2_4_I2C);
				Out_Control(SEL_OUT_NEGATIVE, cfg->hall.gnd); // 1.8v out
				delay_ms(100);
				ccMode = true;
				MUX_Control(SEL_MUX8, MUX8_AMP_1_8V); // cc power
				ADC_Read(SEL_AD_CC_1_8V, 0, adBuffer); // ad cc
				u16Buffer1 = (adBuffer[0] << 8) + (adBuffer[1] & 0xFF);
				ADC_Read(SEL_AD_MUX8, MUX8_AMP_1_8V, adBuffer); // ad ref
				Out_Control(SEL_OUT_VOUT, cfg->hall.v1_8); // VDD
				MUX_Control(SEL_MUX4, MUX4_P_1_8V);
				MUX_Control(SEL_MUX8, MUX_OFF);
				u16Buffer2 = (adBuffer[0] << 8) + (adBuffer[1] & 0xFF); // ad ref
				ccMode = false;
				buffer = (u16Buffer1 - u16Buffer2);
				if (u16Buffer2 >= u16Buffer1) {
					buffer = 0;
				}
				usart->data[resultLength++] = buffer + cal.cc1_8vOffset;
				delay_ms(100);
				i2cErrCount = 0;
				i2c_m_sync_set_slaveaddr(&I2C, 0x10, I2C_M_SEVEN);
				delay_ms(5);
				Hall_Sensor_Reset();
				delay_ms(100);
				Hall_Sensor_Info_Read(adBuffer);
				delay_ms(10);
				memcpy(&usart->data[resultLength], &adBuffer[0], 2);
				resultLength += 2;
				usart->data[resultLength++] = i2cErrCount;
				Hall_Sensor_Read_SEQ();
				usart->data[resultLength++] = hall->status[4] & HALL_SENSOR_STATUS_DRDY_bm;
				usart->data[resultLength++] = hall->status[4] & HALL_SENSOR_STATUS_ERR_bm;
				Find_Max_Min(hall->hxValue, HALL_SENSOR_READ_CNT, &maxBuffer, &minBuffer); // hx value
				i16Buffer = ((maxBuffer + minBuffer) >> 1) * 10;
				usart->data[resultLength++] = i16Buffer >> 8;
				usart->data[resultLength++] = i16Buffer & 0xFF;
				if (maxBuffer == minBuffer) {
					usart->data[resultLength++] = false;
				} else {
					usart->data[resultLength++] = true;
				}
				Find_Max_Min(hall->hyValue, HALL_SENSOR_READ_CNT, &maxBuffer, &minBuffer); // hy value
				i16Buffer = ((maxBuffer + minBuffer) >> 1) * 10;
				usart->data[resultLength++] = i16Buffer >> 8;
				usart->data[resultLength++] = i16Buffer & 0xFF;
				if (maxBuffer == minBuffer) {
					usart->data[resultLength++] = false;
				} else {
					usart->data[resultLength++] = true;
				}
				Find_Max_Min(hall->hzValue, HALL_SENSOR_READ_CNT, &maxBuffer, &minBuffer); // hz value
				i16Buffer = ((maxBuffer + minBuffer) >> 1) * 10;
				usart->data[resultLength++] = i16Buffer >> 8;
				usart->data[resultLength++] = i16Buffer & 0xFF;
				if (maxBuffer == minBuffer) {
					usart->data[resultLength++] = false;
				} else {
					usart->data[resultLength++] = true;
				}
				usart->data[resultLength++] = i2cErrCount;
				MUX_Control(SEL_MUX2_4, MUX_OFF);
				Out_Control(SEL_OUT_ETC_X, OUT_OFF);
				Out_Control(SEL_OUT_ETC_Y, OUT_OFF);
				MUX_Control(SEL_MUX4, MUX_OFF);
				Out_Control(SEL_OUT_VOUT, OUT_OFF);
				Out_Control(SEL_OUT_NEGATIVE, OUT_OFF);
				Out_Control(SEL_OUT_NEGATIVE, cfg->hall.gnd); // GND
				Out_Control(SEL_OUT_POSITIVE, cfg->hall.v1_8); // 1.8v
				MUX_Control(SEL_MUX8, MUX8_R_100K);
				delay_ms(20);
				MUX_Control(SEL_MUX8, MUX8_R_1M);
				delay_ms(20);
				ADC_Read(SEL_AD_MUX8, MUX8_R_1M, adBuffer);
				usart->data[resultLength++] = adBuffer[3];
				MUX_Control(SEL_MUX8, MUX_OFF);
				Out_Control(SEL_OUT_POSITIVE, OUT_OFF);
				Out_Control(SEL_OUT_NEGATIVE, OUT_OFF);
			}
			break;
		case STEP_VBAT_ID:
			if ((out->sol & (OUT_SOL_LED_X | OUT_SOL_POGO_LED_Y))) {
				Solenoid_Valve_Control(SOL_OFF, SOL_POGO_LED_Y);
				delay_ms(200);
				Solenoid_Valve_Control(SOL_OFF, SOL_LED_X);
				delay_ms(100);
			}
			if (!(out->sol & (OUT_SOL_FPCB | OUT_SOL_DOWN))) {
				Solenoid_Valve_Control(SOL_ON, SOL_FPCB);
				Solenoid_Valve_Control(SOL_ON, SOL_DOWN);
				delay_ms(950);
			}
			Out_Control(SEL_OUT_NEGATIVE, cfg->vbatId.gnd); // vbat id check
			Out_Control(SEL_OUT_POSITIVE, cfg->vbatId.batId);
			Out_Control(SEL_OUT_VOUT, cfg->vbatId.v1_8);
			MUX_Control(SEL_MUX4, MUX4_P_1_8V);
			delay_ms(450);
			ADC_Read(SEL_AD_MUX8, MUX_OFF, adBuffer);
			u16Buffer1 = (adBuffer[0] << 8) | adBuffer[1];
			usart->data[resultLength++] = u16Buffer1 >> 8;
			usart->data[resultLength++] = u16Buffer1 & 0xFF;
			MUX_Control(SEL_MUX4, MUX_OFF);
			Out_Control(SEL_OUT_VOUT, OUT_OFF);
			Out_Control(SEL_OUT_POSITIVE, OUT_OFF);
			Out_Control(SEL_OUT_NEGATIVE, OUT_OFF);
			break;
		case STEP_BATTERY:
			if ((out->sol & (OUT_SOL_LED_X | OUT_SOL_POGO_LED_Y))) {
				Solenoid_Valve_Control(SOL_OFF, SOL_POGO_LED_Y);
				delay_ms(100);
				Solenoid_Valve_Control(SOL_OFF, SOL_LED_X);
				delay_ms(100);
			}
			if (!(out->sol & (OUT_SOL_FPCB | OUT_SOL_DOWN))) {
				Solenoid_Valve_Control(SOL_ON, SOL_FPCB);
				Solenoid_Valve_Control(SOL_ON, SOL_DOWN);
				delay_ms(950);
			}
			Out_Control(SEL_OUT_NEGATIVE, cfg->batt.gnd); // batt v check
			Out_Control(SEL_OUT_POSITIVE, cfg->batt.vbatt);
			ADC_Read(SEL_AD_MUX8, MUX_OFF, adBuffer);
			memcpy(&usart->data[resultLength], &adBuffer[0], 2);
			resultLength += 2;
			Out_Control(SEL_OUT_POSITIVE, OUT_OFF);
			Out_Control(SEL_OUT_NEGATIVE, OUT_OFF);
			break;
		case STEP_PROX:
			if ((out->sol & (OUT_SOL_LED_X | OUT_SOL_POGO_LED_Y))) {
				Solenoid_Valve_Control(SOL_OFF, SOL_POGO_LED_Y);
				delay_ms(200);
				Solenoid_Valve_Control(SOL_OFF, SOL_LED_X);
				delay_ms(100);
			}
			if (!(out->sol & (OUT_SOL_FPCB | OUT_SOL_DOWN))) {
				Solenoid_Valve_Control(SOL_ON, SOL_FPCB);
				Solenoid_Valve_Control(SOL_ON, SOL_DOWN);
				delay_ms(950);
			}
      //delay_ms(1000);
			MUX_Control(SEL_AD_MUX8, MUX8_AMP_1_8V);
			ADC_Read(SEL_AD_MUX8, MUX8_AMP_1_8V, adBuffer); // 1.8v read
			MUX_Control(SEL_AD_MUX8, MUX_OFF);
			u16Buffer1 = (adBuffer[0] << 8) + (adBuffer[1] & 0xFF);
			if ((u16Buffer1 < 1700) || (u16Buffer1 > 1900)) {
				testStop = true;
			} else {
				memcpy(&usart->data[resultLength], &adBuffer[0], 2);
				resultLength += 2;
				MUX_Control(SEL_AD_MUX8, MUX8_P_3_3V);
				ADC_Read(SEL_AD_MUX8, MUX8_P_3_3V, adBuffer); // 3.3v read
				MUX_Control(SEL_AD_MUX8, MUX_OFF);
				u16Buffer1 = (adBuffer[0] << 8) + (adBuffer[1] & 0xFF);
				if ((u16Buffer1 < 3200) || (u16Buffer1 > 3400)) {
					testStop = true;
				} else {
					memcpy(&usart->data[resultLength], &adBuffer[0], 2);
					resultLength += 2;
					Out_Control(SEL_OUT_ETC_X, cfg->prox.scl); // i2c
					Out_Control(SEL_OUT_ETC_Y, cfg->prox.sda);
					MUX_Control(SEL_MUX2_4, MUX2_4_I2C);
					Out_Control(SEL_OUT_NEGATIVE, cfg->prox.gnd);
					Out_Control(SEL_OUT_VOUT, cfg->prox.v1_8); // 1.8v out
					MUX_Control(SEL_MUX4, MUX4_P_1_8V);
					Out_Control(SEL_OUT_POSITIVE, cfg->prox.v3_3); // 3.3v out
					MUX_Control(SEL_MUX8, MUX8_AMP_3_3V);
					delay_ms(100);
					i2c_m_sync_set_slaveaddr(&I2C, 0x39, I2C_M_SEVEN);
					delay_ms(5);
					Prox_Sensor_On();
					delay_ms(10);
					i2cErrCount = 0;
					Prox_Sensor_On();
					delay_ms(10);
					buffer = Prox_Sensor_Info_Read();
					usart->data[resultLength++] = buffer;
					delay_ms(1);
					Prox_Sensor_On();
					delay_ms(10);
					Prox_Sensor_Test_Setting();
					delay_ms(10);
					prox->openOffset = Prox_Sensor_Read_Offset(); // open Offset
					usart->data[resultLength++] = prox->openOffset & 0xFF;
					delay_ms(2);
					for (uint8_t i = 0; i < 15; i++) { // open Value
						prox->openValue[i] = Prox_Sensor_Read_Value(); // debug 용
						sumOpen += prox->openValue[i];
						delay_ms(10);
					}
          Solenoid_Valve_Control(SOL_ON, SOL_PROX);
					delay_ms(500);
					
					// 최대값과 최소값 찾기 (open)
					int16_t openMaxValue, openMinValue;
					Find_Max_Min((int16_t*)prox->openValue, 15, &openMaxValue, &openMinValue);
					
					// 최대값과 최소값을 제외한 평균 계산 (open)
					sumOpen = sumOpen - openMaxValue - openMinValue;
					sumOpen /= 13;
					
					usart->data[resultLength++] = sumOpen >> 8;
					usart->data[resultLength++] = sumOpen & 0xFF;
					prox->closeOffset = Prox_Sensor_Read_Offset(); // close Offset
					usart->data[resultLength++] = prox->closeOffset & 0xFF;
					delay_ms(2);
					sumClose = 0;
					for (uint8_t i = 0; i < 15; i++) { // close Value
						prox->closeValue[i] = Prox_Sensor_Read_Value(); // debug 용
						sumClose += prox->closeValue[i];
						delay_ms(10);
					}
					
					// 최대값과 최소값 찾기 (close)
					int16_t closeMaxValue, closeMinValue;
					Find_Max_Min((int16_t*)prox->closeValue, 15, &closeMaxValue, &closeMinValue);
					
					// 최대값과 최소값을 제외한 평균 계산 (close)
					sumClose = sumClose - closeMaxValue - closeMinValue;
					sumClose /= 13;
					
					usart->data[resultLength++] = sumClose >> 8;
					usart->data[resultLength++] = sumClose & 0xFF;
					if (sumClose < sumOpen) {
						sumClose = sumOpen;
					}
					u16Buffer1 = sumClose - sumOpen;
					usart->data[resultLength++] = u16Buffer1 >> 8;
					usart->data[resultLength++] = u16Buffer1 & 0xFF;
					usart->data[resultLength++] = i2cErrCount;
					MUX_Control(SEL_MUX8, MUX_OFF); // 3.3v off
					MUX_Control(SEL_MUX4, MUX_OFF); // 1.8v off
					MUX_Control(SEL_MUX2_4, MUX_OFF); // i2c off
					Out_Control(SEL_OUT_ETC_X, OUT_OFF);
					Out_Control(SEL_OUT_ETC_Y, OUT_OFF);
					Out_Control(SEL_OUT_VOUT, OUT_OFF);
					Out_Control(SEL_OUT_NEGATIVE, cfg->prox.gnd); // GND
					Out_Control(SEL_OUT_POSITIVE, cfg->prox.v1_8); // 1.8v
					MUX_Control(SEL_MUX8, MUX8_R_100K);
					delay_ms(20);
					MUX_Control(SEL_MUX8, MUX8_R_1M);
					delay_ms(20);
					ADC_Read(SEL_AD_MUX8, MUX8_R_1M, adBuffer);
					usart->data[resultLength++] = adBuffer[3];
					Out_Control(SEL_OUT_NEGATIVE, cfg->prox.gnd); // GND
					Out_Control(SEL_OUT_POSITIVE, cfg->prox.v3_3); // 3.3v
					ADC_Read(SEL_AD_MUX8, MUX8_R_1M, adBuffer);
					usart->data[resultLength++] = adBuffer[3];
					MUX_Control(SEL_MUX8, MUX_OFF); // ref 10k off
					Out_Control(SEL_OUT_POSITIVE, OUT_OFF);
					Out_Control(SEL_OUT_NEGATIVE, OUT_OFF);
					Solenoid_Valve_Control(SOL_OFF, SOL_PROX);
				}
			}
			break;
		case STEP_MIC:
			if ((out->sol & (OUT_SOL_LED_X | OUT_SOL_POGO_LED_Y))) {
				Solenoid_Valve_Control(SOL_OFF, SOL_POGO_LED_Y);
				delay_ms(200);
				Solenoid_Valve_Control(SOL_OFF, SOL_LED_X);
				delay_ms(100);
			}
			if (!(out->sol & (OUT_SOL_FPCB | OUT_SOL_DOWN))) {
				Solenoid_Valve_Control(SOL_ON, SOL_FPCB);
				Solenoid_Valve_Control(SOL_ON, SOL_DOWN);
				delay_ms(950);
			}
			Out_Control(SEL_OUT_NEGATIVE, cfg->mic.gnd); // GND
			Out_Control(SEL_OUT_POSITIVE, cfg->mic.mic); // mic 1.8v
			MUX_Control(SEL_MUX8, MUX8_R_100K);
			delay_ms(20);
			MUX_Control(SEL_MUX8, MUX8_R_1M);
			delay_ms(20);
			ADC_Read(SEL_AD_MUX8, MUX8_R_1M, adBuffer);
			usart->data[resultLength++] = adBuffer[3];
			MUX_Control(SEL_MUX8, MUX_OFF);
			Out_Control(SEL_OUT_POSITIVE, OUT_OFF);
			Out_Control(SEL_OUT_NEGATIVE, OUT_OFF);
			break;
	}

	if (testStop == false) {
		usart->data[0] = stepNo;
		usart->data[1] = ((resultLength - 3) >> 8);
		usart->data[2] = (resultLength - 3) & 0xFF;
		Send_Data(TEST_RESULT, resultLength);
	} else {
		usart->data[0] = 0x2;
		Send_Data(TEST_STOP, 1);
		Board_Reset();
		gpio_set_pin_level(GPIO_BUZ, true);
	}
}