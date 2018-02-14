/*
 * input_analizer_obj.h
 *
 *  Created on: May 18, 2017
 *      Author: root
 */

#ifndef INPUT_ANALIZER_OBJ_H_
#define INPUT_ANALIZER_OBJ_H_

#include "gpio.h"

//static char message[64];
static int line_flags[8] = {0,0,0,0,0,0,0,0};
static int start_state_line_flags[8] = {0,0,0,0,0,0,0,0};
static uint16_t input_pins[] = {in_left1_Pin,in_left2_Pin,in_left3_Pin,in_left4_Pin,in_right1_Pin,in_right2_Pin,in_right3_Pin,in_right4_Pin};
static char delimeters[] = {'a','b','c','d','e','f','g','h'};

void input_analizer_check_start_state();
void input_analizer_check_lines();
void input_analizer_send_status();
int input_analizer_get_line_flags(int index);

void input_analizer_common_pressure_drop();

int left_hand_condition();
void inflator_set_inflate_left_flag();

#endif /* INPUT_ANALIZER_OBJ_H_ */
