/*
 * bsp.c
 *
 * 모든 내용은 driver_example.c 참조
 * 
 * Created: 2025-04-09 오전 12:31:07
 *  Author: bruno
 */ 

#include "bsp.h"

uint8_t hbCount = 0;
uint8_t spiTxBuffer[40] = {0,};
uint8_t spiRxBuffer[40] = {0,};
cal_data_t cal;
static struct timer_task Timer_3_Task, Timer_RTC_Task;
struct io_descriptor *SPI_io;
struct io_descriptor *I2C_io;

static void Timer_3_COMM_Reset_task(const struct timer_task *const timer_task) {
	Buffer_Clear();
}

static void Timer_RTC_LED_Control_task(const struct timer_task *const timer_task) {
	gpio_toggle_pin_level(GPIO_LED);
	hbCount++;
	if(hbCount == 2) {
		Send_Data(HEART_BEAT, 0);
		hbCount = 0;
	}
}

void COMM_Reset_Task_Start(void) {
	timer_start(&TIMER_3);
}

void COMM_Reset_Task_Stop(void) {
	timer_stop(&TIMER_3);
	TC3->COUNT32.COUNT.reg = 0;
}

void BSP_Initial(void) {
	gpio_set_pin_level(GPIO_LED_READY, true);
	Timer_3_Task.interval = 100;		
	Timer_3_Task.cb       = Timer_3_COMM_Reset_task;
	Timer_3_Task.mode     = TIMER_TASK_REPEAT;
	Timer_RTC_Task.interval = 1;
	Timer_RTC_Task.cb       = Timer_RTC_LED_Control_task;
	Timer_RTC_Task.mode     = TIMER_TASK_REPEAT;	
	timer_add_task(&TIMER_3, &Timer_3_Task);
	timer_add_task(&TIMER_RTC, &Timer_RTC_Task);
	spi_m_sync_get_io_descriptor(&SPI, &SPI_io);
	spi_m_sync_enable(&SPI);
	i2c_m_sync_get_io_descriptor(&I2C, &I2C_io);
	i2c_m_sync_enable(&I2C);
	timer_start(&TIMER_RTC);
}

void Auto_Calibration(void) {
	uint32_t sum = 0;
	uint8_t buffer[3];
	
	ccMode = true;
	MUX_Control(SEL_AD_MUX8, MUX8_AMP_1_8V);
	for(uint8_t i = 0; i < 4; i++) {								// REF 1.8V Cal
		ADC_Read(SEL_AD_MUX8, MUX8_AMP_1_8V, buffer);
		sum += (buffer[0] << 8) + (buffer[1] & 0xFF);
	}
  MUX_Control(SEL_AD_MUX8, MUX_OFF); 
	sum >>= 2;
	cal.ref1_8v = sum;
	sum = 0;
  for(uint8_t i = 0; i < 4; i++) {								// CC 1.8V Cal
		ADC_Read(SEL_AD_CC_1_8V, MUX_OFF, buffer);
		sum += (buffer[0] << 8) + (buffer[1] & 0xFF);
	}
	sum >>= 2;
	cal.cc1_8vOffset = sum - cal.ref1_8v;						// CC 1.8V Offset
  cal.cc1_8vOffset += 5;
	sum = 0;
  cal.ref1_8v /= 10;
  MUX_Control(SEL_AD_MUX8, MUX8_AMP_3_3V);
	for(uint8_t i = 0; i < 4; i++) {								// REF 3.3V Cal
		ADC_Read(SEL_AD_MUX8, MUX8_AMP_3_3V, buffer);
		sum += (buffer[0] << 8) + (buffer[1] & 0xFF);
	}
  MUX_Control(SEL_AD_MUX8, MUX_OFF);
	sum >>= 2;
	cal.ref3_3v = sum;
	sum = 0;
	for(uint8_t i = 0; i < 4; i++) {								// CC 3.3V Cal
		ADC_Read(SEL_AD_CC_3_3V, MUX_OFF, buffer);
		sum += (buffer[0] << 8) + (buffer[1] & 0xFF);
	}
	sum >>= 2;
	cal.cc3_3vOffset = sum - cal.ref3_3v;						// CC 3.3V Offset
  ccMode = false;
  cal.ref3_3v /= 10;
}