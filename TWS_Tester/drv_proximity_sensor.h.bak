/*
 * drv_proximity_sensor.h
 *
 * Created: 2025-04-09 오전 12:36:00
 *  Author: bruno
 */ 


#ifndef DRV_PROXIMITY_SENSOR_H_
#define DRV_PROXIMITY_SENSOR_H_

#include "main.h"

typedef struct {
	uint16_t closeOffset, openOffset;
	uint16_t closeValue[5], openValue[5];
}prox_sensor_result_t;

extern prox_sensor_result_t proxSensorResult;

void Prox_Sensor_On(void);
uint8_t Prox_Sensor_Info_Read(void);
void Prox_Sensor_Test_Setting(void);
uint16_t Prox_Sensor_Read_Offset(void);
uint16_t Prox_Sensor_Read_Value(void);

#endif /* DRV_PROXIMITY_SENSOR_H_ */