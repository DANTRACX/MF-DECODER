/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           DRIVER
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        TIMER2_DRIVER.c
 *
 * DESCRIPTION:
 *      THIS MODULE REPRESENTS A DRIVER FOR THE 8-BIT TIMER2 HARDWARE
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "TIMER2_DRIVER.h"

/*
 * INTERRUPT FUNCTIONS WITH FUNCTION POINTERS FOR MODULARITY
 */
void (*__TIMER2_DRIVER_INTERRUPT_COMPA_FUNC)(void) = __TIMER2_DRIVER_VEC_DUMMY;
void (*__TIMER2_DRIVER_INTERRUPT_COMPB_FUNC)(void) = __TIMER2_DRIVER_VEC_DUMMY;
void (*__TIMER2_DRIVER_INTERRUPT_OVF_FUNC)(void) = __TIMER2_DRIVER_VEC_DUMMY;

void __TIMER2_DRIVER_COMPA_VECTOR (void)
{
    __TIMER2_DRIVER_INTERRUPT_COMPA_FUNC();
}

void __TIMER2_DRIVER_COMPB_VECTOR (void)
{
    __TIMER2_DRIVER_INTERRUPT_COMPB_FUNC();
}

void __TIMER2_DRIVER_OVF_VECTOR (void)
{
    __TIMER2_DRIVER_INTERRUPT_OVF_FUNC();
}

void __TIMER2_DRIVER_VEC_DUMMY(void)
{
    __asm__("nop");
}
