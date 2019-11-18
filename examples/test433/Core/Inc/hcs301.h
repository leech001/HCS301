/*
 * hcs301.h
 *
 *  Created on: Nov 18, 2019
 *      Author: user
 */

#ifndef INC_HCS301_H_
#define INC_HCS301_H_

#include "main.h"

// ======= Config section =============

#define HCS_RECIEVER_PORT	GPIOC
#define HCS_RECIEVER_PIN  	GPIO_PIN_15

// ====================================

#define	HCS_TE		400
#define	HCS_Te2_3	600

typedef struct {
	uint8_t Repeat;
	uint8_t BatteryLow;
	uint8_t Btn2;
	uint8_t Btn1;
	uint8_t Btn0;
	uint8_t Btn3;
	uint32_t SerialNum;
	uint32_t Encrypt;
} HCS301_t;

uint8_t		HCS_preamble_count;
uint32_t	HCS_last_change;
uint8_t		HCS_bit_counter;
uint8_t		HCS_bit_array[66];

void HCS_interrupt(HCS301_t* data);


#endif /* INC_HCS301_H_ */
