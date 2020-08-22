/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           DRIVER
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        WATCHDOG.h
 *
 * DESCRIPTION:
 *      THIS MODULE REPRESENTS A DRIVER FOR THE WATCHDOG HARDWARE
 *      WHICH IN THIS CASE IS ONLY USED FOR A SOFTWARE RESET
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#ifndef __WATCHDOG_DRIVER_H__
#define __WATCHDOG_DRIVER_H__

/*
 * WATCHDOG MCU STATUS REGISTER
 */
#define __WATCHDOG_DRIVER_MCUSR     (*(volatile unsigned char *)(0x54))
#define __WATCHDOG_DRIVER_WDRF      3

/*
 * WATCHDOG TIMER CONTROL REGISTER
 */
#define __WATCHDOG_DRIVER_WDTCSR    (*(volatile unsigned char *)(0x60))
#define __WATCHDOG_DRIVER_WDP0      0
#define __WATCHDOG_DRIVER_WDP1      1
#define __WATCHDOG_DRIVER_WDP2      2
#define __WATCHDOG_DRIVER_WDE       3
#define __WATCHDOG_DRIVER_WDCE      4
#define __WATCHDOG_DRIVER_WDP3      5
#define __WATCHDOG_DRIVER_WDIE      6
#define __WATCHDOG_DRIVER_WDIF      7

/*
 * WATCHDOG INTERRUPT SREG
 */
#define __WATCHDOG_DRIVER_SREG_SET()      __asm__ __volatile__ ("sei" ::: "memory")
#define __WATCHDOG_DRIVER_SREG_CLEAR()    __asm__ __volatile__ ("cli" ::: "memory")

/*
 * WATCHDOG INTERRUPT VECTORS
 */
#define __WATCHDOG_DRIVER_TIMEOUT_VECTOR     __vector_6


/*
 * ONLY BASIC FUNCTIONS FOR SOFTWARE RESET FEATURE
 */
__attribute__((always_inline))
static inline void __WATCHDOG_DRIVER_START()
{
    __WATCHDOG_DRIVER_WDTCSR |= (1 << __WATCHDOG_DRIVER_WDE);
}

__attribute__((always_inline))
static inline void __WATCHDOG_DRIVER_STOP()
{
    __WATCHDOG_DRIVER_MCUSR &= ~(1 << __WATCHDOG_DRIVER_WDRF);
    __WATCHDOG_DRIVER_WDTCSR |= (1 << __WATCHDOG_DRIVER_WDCE) | (1 << __WATCHDOG_DRIVER_WDE);
    __WATCHDOG_DRIVER_WDTCSR = 0x00;
}

#endif
