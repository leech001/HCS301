/*
 * hcs301.h
 *
 *  Created on: Jun 1, 2020
 *      Author: Bulanov Konstantin
 *
 *  Contact information
 *  -------------------
 *
 * e-mail   :  leech001@gmail.com
 *
 *
 */

#ifndef MBF401_HCS301_H
#define MBF401_HCS301_H

#include "main.h"

#define RX_PORT         RX433_GPIO_Port
#define RX_PIN          RX433_Pin

#define TX_PORT         TX433_GPIO_Port
#define TX_PIN          TX433_Pin

typedef struct {
    uint8_t Repeat;
    uint8_t BatteryLow;
    uint8_t Btn0;
    uint8_t Btn1;
    uint8_t Btn2;
    uint8_t Btn3;
    uint32_t SerialNum;
    uint32_t Encrypt;
    uint64_t code;
} HCS301_t;

uint8_t HCS301_Int();

void HCS301_Send(uint64_t code);

#endif //MBF401_HCS301_H
