/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           DCC-PROTOCOL
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        dcc_protocol.h
 *
 * DESCRIPTION:
 *      DCC-PROTOCOL HEADER FILE
 *      THIS MODULE READS, FILTERS AND CONVERTS DCC DATA FRAMES FOR THE
 *      USER.
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#ifndef _DCC_PROTOCOL_H_
#define _DCC_PROTOCOL_H_

#include "../DRIVERS/IO_DRIVER/IO_DRIVER.h"
#include "../DRIVERS/TIMER1_DRIVER/TIMER1_DRIVER.h"

/*
 * DEFINE THE PROCESSOR SPEED IN HZ
 */
#define DCC_AVR_SPEED   20000000UL

/*
 * DCC FUNCTIONS
 */
extern void DCC_INITIALIZE(void);
extern void DCC_DEINITIALIZE(void);
extern unsigned char DCC_FETCH(unsigned short *DCC_DATA_FRAME);

#endif
