/*
 * inflator_obj.h
 *
 *  Created on: May 19, 2017
 *      Author: root
 */

#ifndef INFLATOR_OBJ_H_
#define INFLATOR_OBJ_H_

#define UPPER_PRESSURE 270
#define LOWER_PRESSURE 245
#define PRESSURE_GAP 25

#include "gpio.h"

static int line_inflated[4] = {0,0,0,0};
static int pump_up_flag = 0;

static uint32_t left_lower_pressure = LOWER_PRESSURE;
static uint32_t left_upper_pressure = UPPER_PRESSURE;
void set_left_lower_pressure(uint32_t pressure);
uint32_t inflator_get_left_lower_pressure();

static uint32_t right_lower_pressure = LOWER_PRESSURE;
static uint32_t right_upper_pressure = UPPER_PRESSURE;
void set_right_lower_pressure(uint32_t pressure);
uint32_t inflator_get_right_lower_pressure();

static uint32_t current_pressure;
void inflator_set_current_pressure(uint32_t pressure);
uint32_t inflator_get_current_pressure();

void debug_valve_led_imitation();
void inflate_line(int line_number);

void inflator_turn_motor_on();
void inflator_turn_motor_off();
static int motor_on_flag = 0;
int inflator_get_motor_flag();

void inflator_turn_drop_valve_on();
void inflator_turn_drop_valve_off();

void inflator_monitor();

static int inflate_line_flag;
void inflator_drop_inflate_flag();
void inflator_rise_inflate_flag();
int inflator_get_inflate_flag();

void drop_pressure();
void inflate_right_arm();
void inflate_left_arm();
void inflate_both_arms();

static int inflate_right_flag = 0;
void inflator_set_inflate_right_flag();
static int inflate_left_flag = 0;
void inflator_set_inflate_left_flag();

#endif /* INFLATOR_OBJ_H_ */
