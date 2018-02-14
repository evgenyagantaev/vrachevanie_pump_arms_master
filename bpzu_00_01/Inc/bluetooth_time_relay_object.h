/* bluetooth_time_relay_object.h
 *
 *  Created on: Jan 22, 2018
 *      Author: root
 */

#ifndef INC_BLUETOOTH_TIME_RELAY_OBJECT_H_
#define INC_BLUETOOTH_TIME_RELAY_OBJECT_H_

#define BLUETOOTH_OFF_TIMEOUT 300  // 300 seconds bluetooth silence timeout

static int old_one_hz_counter = 0;
static int bluetooth_off_counter = 0;
static int bluetooth_off_flag = 0;
static int bluetooth_on_flag = 1;




void bluetooth_time_relay_counter_increment();
void bluetooth_time_relay_counter_reset();
void bluetooth_off();

int get_bluetooth_off_flag();
void set_bluetooth_off_flag();
int get_bluetooth_on_flag();
void reset_bluetooth_on_flag();

void bluetooth_time_relay_task();

#endif /* INC_BLUETOOTH_TIME_RELAY_OBJECT_H_ */
