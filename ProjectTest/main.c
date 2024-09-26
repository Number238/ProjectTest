#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>

#include "leds.h"
#include "spi_comm.h"

#include "sensors/proximity.h"

#include "epuck1x/uart/e_uart_char.h"
#include "stdio.h"
#include "serial_comm.h"

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    // Your initialisations here
    clear_leds();
    spi_comm_start();
    motors_init();
    messagebus_init(&bus, &bus_lock, &bus_condvar);
    proximity_start(1);
    calibrate_ir();
    serial_start();

   	char str[100];
   	int str_length;

	//waits 1 second
	set_body_led(1);
	chThdSleepMilliseconds(1000);
	set_led(LED1 , 1);
    chThdSleepMilliseconds(1000);
	  set_led(LED3 , 0);
    chThdSleepMilliseconds(1000);
    set_body_led(0);
    chThdSleepMilliseconds(1000);
	  set_led(LED1 , 0);
  chThdSleepMilliseconds(1000);
	  set_led(LED3 , 1);
  chThdSleepMilliseconds(1000);


    /* Infinite loop. */
    while (1) {

      if (get_calibrated_prox(0)>=200 || get_calibrated_prox(7)>=200)
    		  {
    	 set_body_led(0);
   	  left_motor_set_speed(500);
   	  right_motor_set_speed(-500);
    		  }
      else
    	  {
    	  set_body_led(1);
       	  left_motor_set_speed(0);
       	  right_motor_set_speed(0);
       	str_length = sprintf(str, "Hello World\n");
        chThdSleepMilliseconds(1000);
       	e_send_uart1_char(str, str_length);
    	  }


    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
