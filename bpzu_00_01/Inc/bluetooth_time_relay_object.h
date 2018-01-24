knjum<F8><F6>6y hgt<F5>5r<F4>4<F3>3<F2>/`<F11><F10>p0<F9>9o8i<F7>j vn
 * bluetooth_time_relay_object.h
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
static int bluetooth_seconds_time_value = 0;
static int bluetooth_on_flag = 1;




void bluetooth_time_relay_counter_increment();
void bluetooth_time_relay_counter_reset();
void bluetooth_off();

int get_bluetooth_off_flag();
int set_bluetooth_off_flag();
int get_bluetooth_on_flag();
int reset_bluetooth_on_flag();


#endif /* INC_BLUETOOTH_TIME_RELAY_OBJECT_H_ */
