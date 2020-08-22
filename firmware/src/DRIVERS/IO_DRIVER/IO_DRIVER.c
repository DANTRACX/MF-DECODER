/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           DRIVER
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        IO_DRIVER.c
 *
 * DESCRIPTION:
 *      THIS MODULE REPRESENTS A DRIVER FOR I/O FUNCTIONALITY
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "IO_DRIVER.h"

/*
 * INTERRUPT FUNCTIONS WITH FUNCTION POINTERS FOR MODULARITY
 */
void (*__IO_DRIVER_INTERRUPT_INT0_FUNC)(void) = __IO_DRIVER_VEC_DUMMY;
void (*__IO_DRIVER_INTERRUPT_INT1_FUNC)(void) = __IO_DRIVER_VEC_DUMMY;

void __IO_DRIVER_INT0_VECTOR (void)
{
    __IO_DRIVER_INTERRUPT_INT0_FUNC();
}

void __IO_DRIVER_INT1_VECTOR (void)
{
    __IO_DRIVER_INTERRUPT_INT1_FUNC();
}

void __IO_DRIVER_VEC_DUMMY(void)
{
    __asm__("nop");
}
