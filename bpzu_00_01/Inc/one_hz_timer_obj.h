/*
 * one_hz_timer_obj.h
 *
 *  Created on: Jul 25, 2017
 *      Author: root
 */

#ifndef INC_ONE_HZ_TIMER_OBJ_H_
#define INC_ONE_HZ_TIMER_OBJ_H_



static int one_hz_counter = 0;
static int systick_counter = 0;
static int one_hz_timer_flag = 0;

static char *message[128];

void one_hz_timer_tick(void);
void one_hz_timer_poll(void);
void one_hz_schedule_task(void);

int get_one_hz_counter();

#endif /* INC_ONE_HZ_TIMER_OBJ_H_ */
