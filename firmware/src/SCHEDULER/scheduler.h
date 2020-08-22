/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           SCHEDULER
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        scheduler.h
 *
 * DESCRIPTION:
 *      SCHEDULER HEADER FILE
 *      THE SCHEDULER GIVES A BASIC TIMING FUNCTIONALITY TO THE USER
 *      WITH A RESOLUTION AT 0,1 SECONDS AND THE OPPORTUNITY FOR SETTING
 *      A MONITOR LED WITH AUTOMATIC FALLBACK
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "../GPIO/gpio.h"
#include "../DRIVERS/MEMORY_DRIVER/MEMORY_DRIVER.h"
#include "../DRIVERS/TIMER0_DRIVER/TIMER0_DRIVER.h"

/*
 * SCHEDULER FUNCTIONS
 */
extern void SCHEDULER_INITIALIZE(void);
extern void SCHEDULER_MONITOR_LED_SET(void);
extern void SCHEDULER_SECONDS_SET_TIME(unsigned char *TimeStart);
extern unsigned char SCHEDULER_SECONDS_GET_TIME(unsigned char *TimeStart);

#endif
