/*
 * conversion.h
 *
 * Created: 2023-01-25 오후 08:31:07
 *  Author: bruno
 */ 


#ifndef CONVERSION_H_
#define CONVERSION_H_

#include "def.h"
#include "main.h"

typedef enum {
	STEP_CON_OS = 1, STEP_POGO_OS, STEP_LED, STEP_HALL_SENSOR,
	STEP_VBAT_ID, STEP_BATTERY, STEP_PROX, STEP_MIC
}step_name_e;

typedef struct {
	uint8_t positve[MAX_CONNECTOR];
	uint8_t negative[MAX_CONNECTOR];
	uint8_t refernce[MAX_CONNECTOR];
}connector_t;

typedef struct {
	uint8_t j1;
	uint8_t j2;
	uint8_t pogo;
	uint8_t gnd;
}pogo_t;

typedef struct {
	uint8_t led;
	uint8_t gnd;
}led_t;

typedef struct {
	uint8_t v1_8;
	uint8_t scl;
	uint8_t sda;
	uint8_t gnd;
}hall_sensor_t;

typedef struct {
	uint8_t v1_8;
	uint8_t batId;
	uint8_t gnd;
}vbat_id_t;

typedef struct {
	uint8_t vbatt;
	uint8_t gnd;
}battery_t;

typedef struct {
	uint8_t v3_3;
	uint8_t v1_8;
	uint8_t scl;
	uint8_t sda;
	uint8_t gnd;
}prox_test_t;

typedef struct {
	uint8_t mic;
	uint8_t gnd;
}mic_t;

typedef struct {
	uint16_t stepLength;
	connector_t conn;
	pogo_t pogo;
	led_t led;
	hall_sensor_t hall;
	vbat_id_t vbatId;
	battery_t batt;
	prox_test_t prox;
	mic_t mic;
}cfg_data_t;

extern cfg_data_t cfgData;

void CFG_Conversion(void);

#endif /* CONVERSION_H_ */