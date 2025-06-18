/*
 * drv_proximity_sensor.c
 *
 * Created: 2025-04-09 오전 12:35:39
 *  Author: bruno
 * Modified: SHARP GP2AP130S 센서용으로 변경
 */ 

#include "drv_proximity_sensor.h"

prox_sensor_result_t proxSensorResult = {0,};

void Prox_Sensor_On(void) {						// activate adc
	uint8_t buffer[2] = {0x80, 0xA0};  // 0xA0 = OP_RUN (Sharp 센서)
	
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
}

uint8_t Prox_Sensor_Info_Read(void) {		// device id read
	uint8_t reg = 0xA0;  // Sharp 센서에서는 디바이스 ID 레지스터가 0xA0
	uint8_t buffer = 0;
	uint8_t error = 0; // 에러 발생 플래그 추가

	if(io_write(I2C_io, &reg, 1) < 0) {
		i2cErrCount++;
		error = 1; // 에러 발생 표시
	}
	if(io_read(I2C_io, &buffer, 1) < 0) {
		i2cErrCount++;
		error = 1; // 에러 발생 표시
	}

	// 에러가 발생했으면 buffer를 0으로 초기화
	if(error) {
		buffer = 0;
	}

	return buffer;
}

void Prox_Sensor_Test_Setting(void) {				// test configuration - Sharp 센서용 설정
	uint8_t buffer[2] = {0,};

	buffer[0] = 0x84; buffer[1] = 0x08;	// PWTIME (16ms)
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0x85; buffer[1] = 0x93;	// Ppulse (3ea)
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0x86; buffer[1] = 0xB0;	// Pgain 원래값으로 복원 (0x50 -> 0xB0)
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0x88; buffer[1] = 0x7D;	// PS_LTH Low byte (125)
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0x89; buffer[1] = 0x00;	// PS_LTH High byte (125)
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0x8A; buffer[1] = 0x9B;	// PS_HTH Low byte (155)
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0x8B; buffer[1] = 0x00;	// PS_HTH High byte (155)
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0x8F; buffer[1] = 0x10;	// CLK[2:0]=001, internal clock adjustment (0x70 -> 0x10)
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0xC1; buffer[1] = 0x61;	// 오프셋 원래값으로 복원 (0x41 -> 0x61)
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0xC5; buffer[1] = 0x30;	// Gain_F 원래값으로 복원 (0x10 -> 0x30)
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0xD0; buffer[1] = 0x24;	// IIR_SW[1:0]=10, IIR[2:0]=100, Average 16 times
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0xD1; buffer[1] = 0x3C;	// IIR_THD = 60
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0xF0; buffer[1] = 0x09;	// PLDrive 원래값으로 복원 (0x05 -> 0x09)
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0xF2; buffer[1] = 0x44;	// TC1[2:0]=100, TC0[2:0]=100, Temperature Characteristics Correction
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	
	// 인터럽트 설정
	buffer[0] = 0x82; buffer[1] = 0x00;	// INT_PROX 인터럽트 설정
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0x83; buffer[1] = 0x20;	// INT_PROX 인터럽트 설정
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
	buffer[0] = 0x87; buffer[1] = 0x18;	// INT_PROX 인터럽트 설정
	if(io_write(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
	}
	delay_ms(2);
}

uint16_t Prox_Sensor_Read_Offset(void) {					// read offset - Sharp 센서
	uint8_t buffer[2] = {0,};
	uint8_t error = 0; // 에러 발생 플래그 추가
	buffer[0] = 0xC3;  // Sharp 센서에서는 오프셋 카운트 레지스터가 0xC3
	
	if(io_write(I2C_io, &buffer[0], 1) < 0) {
		i2cErrCount++;
		error = 1; // 에러 발생 표시
	}
	if(io_read(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
		error = 1; // 에러 발생 표시
	}
	
	// 에러가 발생했으면 buffer를 0으로 초기화
	if(error) {
		buffer[0] = 0;
		buffer[1] = 0;
	}
	
	return ((buffer[1] << 8) | buffer[0]) & 0x7FFF;  // 15비트 값
}

uint16_t Prox_Sensor_Read_Value(void) {						// read value - Sharp 센서
	uint8_t buffer[2] = {0,};
	uint8_t error = 0; // 에러 발생 플래그 추가
	buffer[0] = 0x90;  // Sharp 센서에서는 근접 데이터 레지스터가 0x90
	
	if(io_write(I2C_io, &buffer[0], 1) < 0) {
		i2cErrCount++;
		error = 1; // 에러 발생 표시
	}
	if(io_read(I2C_io, buffer, 2) < 0) {
		i2cErrCount++;
		error = 1; // 에러 발생 표시
	}

	// 에러가 발생했으면 buffer를 0으로 초기화
	if(error) {
		buffer[0] = 0;
		buffer[1] = 0;
	}

	return ((buffer[1] << 8) | buffer[0]) & 0x7FFF;  // 15비트 값 (0x7FFF = 0111 1111 1111 1111)
}