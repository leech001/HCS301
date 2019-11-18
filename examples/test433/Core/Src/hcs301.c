/*
 * hcs301.c
 *
 *  Created on: Nov 18, 2019
 *      Author: user
 */

/*
 * |-----------------------------------------------------------------------------------------------------------------------------------------------
 * | Copyright (C) Bulanov Konstantin,2019
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
 * | HCS301 algorithm used from http://arduino.ru/forum/apparatnye-voprosy/biblioteka-chteniya-id-brelkov-signalizatsiii-hcs301-keeloq#comment-35573
 * |------------------------------------------------------------------------------------------------------------------------------------------------
 */


#include "hcs301.h"
#include "micros.h"		// https://github.com/leech001/micros

uint8_t	HCS_Listening = 1;

void HCS_interrupt(HCS301_t* data){

	if(HCS_Listening == 0){
		return;
	}

	uint32_t cur_timestamp = micros();
	uint8_t  cur_status = HAL_GPIO_ReadPin(HCS_RECIEVER_PORT, HCS_RECIEVER_PIN);
	uint32_t pulse_duration = cur_timestamp - HCS_last_change;
	HCS_last_change	= cur_timestamp;

	if(HCS_preamble_count < 12){
		if(cur_status == 1){
			if( ((pulse_duration > 150) && (pulse_duration < 500)) || HCS_preamble_count == 0){
			} else {
				HCS_preamble_count = 0;
				return;

			}
		} else {
			if((pulse_duration > 300) && (pulse_duration < 600)){
				HCS_preamble_count ++;
				if(HCS_preamble_count == 12){
					HCS_bit_counter = 0;
					return;
				}
			} else {
				HCS_preamble_count = 0;
				return;
			}
		}
	}

	if(HCS_preamble_count == 12){
		if(cur_status == 1){
			if(((pulse_duration > 250) && (pulse_duration < 900)) || HCS_bit_counter == 0){
			} else {
				HCS_preamble_count = 0;
				return;
			}
		} else {
			if((pulse_duration > 250) && (pulse_duration < 900)){
				HCS_bit_array[65 - HCS_bit_counter] = (pulse_duration > HCS_Te2_3) ? 0 : 1;
				HCS_bit_counter++;
				if(HCS_bit_counter == 66){
					HCS_Listening = 0;
					HCS_preamble_count = 0;

					data->Repeat = HCS_bit_array[0];
					data->BatteryLow = HCS_bit_array[1];
					data->Btn2 = HCS_bit_array[2];
					data->Btn1 = HCS_bit_array[3];
					data->Btn0 = HCS_bit_array[4];
					data->Btn3 = HCS_bit_array[5];

					data->SerialNum = 0;
					for(int i = 6; i < 34;i++){
						data->SerialNum = (data->SerialNum << 1) + HCS_bit_array[i];
					};

					uint32_t Encrypt = 0;
					for(int i = 34; i < 66;i++){
						 Encrypt = (Encrypt << 1) + HCS_bit_array[i];
					};
					data->Encrypt = Encrypt;
					HCS_Listening = 1;
					// Return code
					return;
				}
			} else {
				HCS_preamble_count = 0;
				return;
			}
		}
	}
}

