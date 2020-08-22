/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           SUBROUTINES
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        subroutines.h
 *
 * DESCRIPTION:
 *      THIS MODULE IS A COLLECTION OF DECODER OPERATING MODES
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#ifndef _SUBROUTINES_H_
#define _SUBROUTINES_H_

#include "../decoder.h"
#include "../GPIO/gpio.h"
#include "../PWMDIMM/pwmdimm.h"
#include "../SCHEDULER/scheduler.h"
#include "../DRIVERS/MEMORY_DRIVER/MEMORY_DRIVER.h"

/*
 * SUBROUTINE FUNCTIONS
 */
extern void SUBROUTINE_GET_ADDR(unsigned short *ADDR, unsigned char COUNT);

/*
 * MAX-PWM FUNCTION
 */
extern void SUBROUTINE_GET_MAXPWM(unsigned char *PWM_LEVEL);

/*
 * NIGHT-PWM FUNCTIONS
 */
extern void SUBROUTINE_GET_NIGHTADDR(unsigned short *ADDR);
extern void SUBROUTINE_GET_NIGHTPWM(unsigned char *PWM_LEVEL);

/*
 * VIER ZWEIBEGRIFFIGE SIGNALE
 */
extern void subroutine_01_MAIN();

/*
 * ZWEI DREIBEGRIFFIGE SIGNALE
 */
extern void subroutine_02_MAIN();

/*
 * ZWEI VIERBEGRIFFIGES SIGNAL
 */
extern void subroutine_03_MAIN();

/*
 * ZWEI VORSIGNALE OHNE DUNKELTASTUNG (NICHT AM MAST / FREISTEHEND)
 */
extern void subroutine_04_MAIN();

/*
 * EIN DREIBEGRIFFIGES SIGNAL + EIN VORSIGNAL OHNE DUNKELTASTUNG
 */
extern void subroutine_05_MAIN();

/*
 * EIN DREIBEGRIFFIGES SIGNAL + EIN VORSIGNAL MIT DUNKELTASTUNG
 */
extern void subroutine_06_MAIN();

/*
 * EIN VIERBEGRIFFIGES SIGNAL + EIN VORSIGNAL OHNE DUNKELTASTUNG
 */
extern void subroutine_07_MAIN();

/*
 * EIN VIERBEGRIFFIGES SIGNAL + EIN VORSIGNAL MIT DUNKELTASTUNG
 */
extern void subroutine_08_MAIN();

/*
 * BAHNÜBERGANG MIT ÜBERWACHUNGSSIGNAL
 */
extern void subroutine_09_MAIN();

/*
 * DREI MAGNETARTIKEL (WEICHEN/MOTOREN)
 */
extern void subroutine_10_MAIN();

/*
 * ZEHN UMSCHALTBARE AUSGÄNGE + ALLE AN/AUS
 */
extern void subroutine_11_MAIN();

#endif
