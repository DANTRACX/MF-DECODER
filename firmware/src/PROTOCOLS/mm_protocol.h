/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           MM-PROTOCOL
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        mm_protocol.h
 *
 * DESCRIPTION:
 *      MM-PROTOCOL HEADER FILE
 *      THIS MODULE READS, FILTERS AND CONVERTS MOTOROLA DATA FRAMES FOR THE
 *      USER. (SWITCHES ONLY, TRAIN COMMANDS WILL BE FILTERED OUT)
 *      FOR MOTOROLA I AND MOTOROLA II
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#ifndef _MM_PROTOCOL_H_
#define _MM_PROTOCOL_H_

#include "../DRIVERS/IO_DRIVER/IO_DRIVER.h"
#include "../DRIVERS/TIMER1_DRIVER/TIMER1_DRIVER.h"

/*
 * DEFINE THE PROCESSOR SPEED IN HZ
 */
#define MM_AVR_SPEED   20000000UL

/*
 * MOTOROLA FUNCTIONS
 */
extern void MM_INITIALIZE(void);
extern void MM_DEINITIALIZE(void);
extern unsigned char MM_FETCH(unsigned short *MM_DATA_FRAME);

#endif
