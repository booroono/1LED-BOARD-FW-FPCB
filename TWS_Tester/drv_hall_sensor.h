/*
 * drv_hall_sensor.h
 *
 * Created: 2025-04-09 오전 12:34:59
 *  Author: bruno
 */ 


#ifndef DRV_HALL_SENSOR_H_
#define DRV_HALL_SENSOR_H_

#include "main.h"

typedef struct {
	uint8_t status[HALL_SENSOR_READ_CNT];
	int16_t hxValue[HALL_SENSOR_READ_CNT];
	int16_t hyValue[HALL_SENSOR_READ_CNT];
	int16_t hzValue[HALL_SENSOR_READ_CNT];
}hall_sensor_result_t;

extern uint8_t i2cErrCount;
extern hall_sensor_result_t hallSensorResult;

void Hall_Sensor_Reset(void);
void Hall_Sensor_Info_Read(uint8_t *buffer);
uint8_t Hall_Sensor_Status_Check(uint8_t status_bm);
void Hall_Sensor_Test_Mode(uint8_t data);
void Hall_Sensor_Read_Data(uint8_t count);
void Hall_Sensor_Read_SEQ(void);
void Find_Max_Min(int16_t *buffer, uint8_t count, int16_t *max, int16_t *min);

#endif /* DRV_HALL_SENSOR_H_ */