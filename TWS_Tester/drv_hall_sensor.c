/*
 * drv_hall_sensor.c
 *
 * Created: 2025-04-09 오전 12:34:46
 *  Author: bruno
 */ 

#include "drv_hall_sensor.h"

uint8_t i2cErrCount = 0;
hall_sensor_result_t hallSensorResult = {0,};

void Hall_Sensor_Reset(void) {		// reset
	uint8_t buffer[2] = {0x30, 0x1};

	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
}

void Hall_Sensor_Info_Read(uint8_t *buffer) {		// company & device id read
	uint8_t reg = 0;
		
	if(io_write(I2C_io, &reg, 1) < 0) {
		i2cErrCount++;
	}
	if(io_read(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
}

uint8_t Hall_Sensor_Status_Check(uint8_t status_bm) {				// status read
	uint8_t reg = 0x10;
	uint8_t buffer[2] = {0,};
			
	if(io_write(I2C_io, &reg, 1) < 0) {
		i2cErrCount++;
	}
	if(io_read(I2C_io, buffer, 1) < 0) {
		i2cErrCount++;
	}

	return buffer[0] & status_bm;
}

void Hall_Sensor_Test_Mode(uint8_t data) {					// test mode control
	uint8_t buffer[2] = {0x21, 0};
	
	buffer[1] = data;
	
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
}

void Hall_Sensor_Read_Data(uint8_t count) {								// read data
	uint8_t reg = 0x17;
	uint8_t buffer[7] = {0,};
	hall_sensor_result_t *result = &hallSensorResult;

	if(io_write(I2C_io, &reg, 1) < 0) {
		i2cErrCount++;
	}
	if(io_read(I2C_io, buffer, 7) < 0) {
		i2cErrCount++;
	}
	result->status[count] = buffer[0];
	result->hzValue[count] = (buffer[1] << 8) | (buffer[2] & 0xFF);
	result->hyValue[count] = (buffer[3] << 8) | (buffer[4] & 0xFF);
	result->hxValue[count] = (buffer[5] << 8) | (buffer[6] & 0xFF);
}

void Hall_Sensor_Read_SEQ(void) {									// read seq
	uint8_t reg = 0x21;
	uint8_t buffer = 0;
	
	for(uint8_t i = 0; i < HALL_SENSOR_READ_CNT; i++) {
		Hall_Sensor_Test_Mode(0x81);
		delay_ms(100);
		buffer = Hall_Sensor_Status_Check(HALL_SENSOR_STATUS_DRDY_bm);
		delay_ms(1);
		Hall_Sensor_Read_Data(i);
		delay_ms(1);
		buffer = Hall_Sensor_Status_Check(HALL_SENSOR_STATUS_DRDY_bm);
		delay_ms(1);
		io_write(I2C_io, &reg, 1);
		io_read(I2C_io, &buffer, 1);
		delay_ms(1);
		Hall_Sensor_Test_Mode(0);
		delay_ms(10);		
	}
}

void Find_Max_Min(int16_t *buffer, uint8_t count, int16_t *max, int16_t *min) {
  *max = buffer[0];
  *min = buffer[0];

  for (int i = 1; i < count; i++) {
    if (buffer[i] > *max) {
      *max = buffer[i];
    }
    if (buffer[i] < *min) {
      *min = buffer[i];
    }
  }
}
