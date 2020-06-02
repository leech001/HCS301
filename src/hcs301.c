/*
 * hcs301.c
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

/*
 * |-----------------------------------------------------------------------------------------------------------------------------------------------
 * | Copyright (C) Bulanov Konstantin,2020
 * |
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * |
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |
 * |------------------------------------------------------------------------------------------------------------------------------------------------
 */

#include <string.h>
#include "hcs301.h"
#include "micros.h"

uint8_t BitArray[66];

uint32_t HCS_last_change = 0;
uint8_t HCS_preamble_count = 0;
uint8_t HCS_header_count = 0;
uint8_t HCS_bit_counter = 0;

extern HCS301_t hcs301;

uint8_t HCS301_Int() {
    uint32_t cur_timestamp = micros();
    uint8_t cur_status = HAL_GPIO_ReadPin(RX_PORT, RX_PIN);
    uint32_t pulse_duration = cur_timestamp - HCS_last_change;
    HCS_last_change = cur_timestamp;

    if (HCS_preamble_count < 12) {
        if (cur_status == 0) {
            if ((pulse_duration > 280) && (pulse_duration < 620)) {
                HCS_preamble_count++;
                if (HCS_preamble_count == 12) {
                    return 0;
                }
            } else {
                HCS_preamble_count = 0;
                HCS_header_count = 0;
                return 0;
            }
        }
    }

    if (HCS_preamble_count == 12) {
        if (cur_status == 1) {
            if ((pulse_duration > 2800) && (pulse_duration < 6200)) {
                HCS_header_count = 1;
                HCS_bit_counter = 0;
                return 0;
            }
        }
    }

    if (HCS_preamble_count == 12 && HCS_header_count == 1) {
        if (cur_status == 0) {
            if ((pulse_duration > 280) && (pulse_duration < 1240)) {
                BitArray[65 - HCS_bit_counter] = (pulse_duration > 560) ? 0 : 1;
                HCS_bit_counter++;

                if (HCS_bit_counter == 66) {
                    hcs301.Repeat = BitArray[0];
                    hcs301.BatteryLow = BitArray[1];
                    hcs301.Btn2 = BitArray[2];
                    hcs301.Btn1 = BitArray[3];
                    hcs301.Btn0 = BitArray[4];
                    hcs301.Btn3 = BitArray[5];

                    for (uint8_t i = 6; i < 34; i++) {
                        hcs301.SerialNum = (hcs301.SerialNum << 1) + BitArray[i];
                    }

                    for (uint8_t i = 34; i < 66; i++) {
                        hcs301.Encrypt = (hcs301.Encrypt << 1) + BitArray[i];
                    }

                    for (uint8_t i = 2; i < 66; i++) {
                        hcs301.code = (hcs301.code << 1) + BitArray[i];
                    }

                    HCS_preamble_count = 0;
                    return 1;
                }
            } else {
                HCS_preamble_count = 0;
                HCS_header_count = 0;
            }
        }
    }
    return 0;
}

void HCS301_Send(uint64_t code) {
    // Repeat 10 times
    for (uint8_t repeat = 0; repeat < 10; repeat++) {
        HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_RESET);
        // Preamble
        for (uint8_t i = 0; i < 23; i++) {
            HAL_GPIO_TogglePin(TX_PORT, TX_PIN);
            delay_us(400);
        }

        // Header
        HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_RESET);
        delay_us(4000);

        // Code
        for (uint8_t i = 0; i < 2; i++) {
            HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_SET);
            delay_us(400);
            HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_RESET);
            delay_us(800);
        }
        for (uint8_t i = 0; i < 64; i++) {
            if (code & (1ULL << i)) {
                HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_SET);
                delay_us(400);
                HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_RESET);
                delay_us(800);
            } else {
                HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_SET);
                delay_us(800);
                HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_RESET);
                delay_us(400);
            }
        }

        // Repeat & vbat
        for (uint8_t i = 0; i < 2; i++) {
            HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_SET);
            delay_us(400);
            HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_RESET);
            delay_us(800);
        }
        HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_RESET);
        delay_us(15600);
    }
}