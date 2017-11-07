/*
 * inflator_obj.h
 *
 *  Created on: May 19, 2017
 *      Author: root
 */

#ifndef INFLATOR_OBJ_H_
#define INFLATOR_OBJ_H_

#define UPPER_PRESSURE 3150
#define LOWER_PRESSURE 2900

#include "gpio.h"

static int line_inflated[4] = {0,0,0,0};
static int *input_flags;
static int pump_up_flag = 0;

static uint32_t current_pressure;
void inflator_set_current_pressure(uint32_t pressure);

void debug_valve_led_imitation();
void inflate_line(int line_number);

void inflator_turn_motor_on();
void inflator_turn_motor_off();
void inflator_turn_drop_valve_on();
void inflator_turn_drop_valve_off();

void inflator_monitor();

static int inflate_line_flag;
void inflator_drop_inflate_flag();
void inflator_rise_inflate_flag();
int inflator_get_inflate_flag();

#endif /* INFLATOR_OBJ_H_ */
