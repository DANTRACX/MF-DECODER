/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           GPIO
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        gpio.h
 *
 * DESCRIPTION:
 *      THIS MODULE GIVES A LOW LEVEL IO ACCESS TO THE USER
 *      IT CONTROLS INPUT/OUTPUT AND EXTERNAL INTERRUPTS
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#ifndef _GPIO_H_
#define _GPIO_H_

#include "../DRIVERS/IO_DRIVER/IO_DRIVER.h"
#include "../DRIVERS/WATCHDOG_DRIVER/WATCHDOG_DRIVER.h"

/*
 * DEFINES THE PORT BITMASKS FOR THE FADER TO ENABLE/DISABLE SINGLE PORTS
 * LOW-BYTE: PORTB, HIGH-BYTE: PORTC
 */
#define GPIO_OUT_01         0x0004
#define GPIO_OUT_02         0x0008
#define GPIO_OUT_03         0x0010
#define GPIO_OUT_04         0x0020
#define GPIO_OUT_05         0x0800
#define GPIO_OUT_06         0x0400
#define GPIO_OUT_07         0x0200
#define GPIO_OUT_08         0x0100
#define GPIO_OUT_09         0x2000
#define GPIO_OUT_10         0x1000

/*
 * DEFINE FUNCTIONS FOR THE RED STATUS LED
 */
#define GPIO_LED_RD_ON()    __IO_DRIVER_IO_B_SET_OUT_H(0x01);
#define GPIO_LED_RD_OFF()   __IO_DRIVER_IO_B_SET_OUT_L(0x01);
#define GPIO_LED_RD_INIT()  __IO_DRIVER_IO_B_SET_OUT(0x01);

/*
 * DEFINE FUNCTIONS FOR THE YELLOW STATUS LED
 */
#define GPIO_LED_YE_ON()    __IO_DRIVER_IO_D_SET_OUT_H(0x80);
#define GPIO_LED_YE_OFF()   __IO_DRIVER_IO_D_SET_OUT_L(0x80);
#define GPIO_LED_YE_INIT()  __IO_DRIVER_IO_D_SET_OUT(0x80);

/*
 * DEFINE FUNCTIONS FOR THE GREEN STATUS LED
 */
#define GPIO_LED_GN_ON()    __IO_DRIVER_IO_D_SET_OUT_H(0x20);
#define GPIO_LED_GN_OFF()   __IO_DRIVER_IO_D_SET_OUT_L(0x20);
#define GPIO_LED_GN_INIT()  __IO_DRIVER_IO_D_SET_OUT(0x20);

/*
 * PORT FUNCTIONS FOR THE PROGRAMMING SWITCH
 */
extern void GPIO_INITIALIZE(void);
extern void GPIO_SWITCH_SET_SCOPE(unsigned char scope);
extern unsigned char GPIO_SWITCH_GET_CHOICE(void);
extern void GPIO_SWITCH_RESET_CHOICE(void);
extern void GPIO_SET_SOFT_RESET_FUNC(void (*funcptr)(void));
extern void GPIO_SET_HARD_RESET_FUNC(void (*funcptr)(void));

#endif
