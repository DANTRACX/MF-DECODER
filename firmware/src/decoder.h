/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           DECODER
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        decoder.h
 *
 * DESCRIPTION:
 *      DECODER HEADER FILE, STORES ALL FUNCTIONS AND DATATYPES FOR
 *      BASIC DECODER OPERATION
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#ifndef _DECODER_H_
#define _DECODER_H_

#include "GPIO/gpio.h"
#include "PWMDIMM/pwmdimm.h"
#include "SCHEDULER/scheduler.h"
#include "DRIVERS/MEMORY_DRIVER/MEMORY_DRIVER.h"
#include "DRIVERS/WATCHDOG_DRIVER/WATCHDOG_DRIVER.h"
#include "DRIVERS/IO_DRIVER/IO_DRIVER.h"
#include "PROTOCOLS/dcc_protocol.h"
#include "PROTOCOLS/mm_protocol.h"
#include "SUBROUTINES/subroutines.h"

/*
 * DEFINE EEPROM ADDRESSES FOR DATA SAVING
 */
#define DECODER_SUBROUTINE_ADDR         0x0001  /* USES 1 BYTE */
#define DECODER_PROTOCOL_ADDR           0x0002  /* USES 1 BYTE */
#define DECODER_FADESPEED_ADDR          0x0003  /* USES 1 BYTE */
#define DECODER_FADEDELAY_ADDR          0x0004  /* USES 1 BYTE */
#define DECODER_MAXPWM_ADDR             0x0005  /* USES 1 BYTE */
#define DECODER_NIGHTPWM_ADDR           0x0006  /* USES 1 BYTE */
#define DECODER_NIGHTPWM_ADDRESS_ADDR   0x0007  /* USES 4 BYTE */
#define DECODER_ADDRESS_ADDR            0x0012  /* USES (ADDRESS_SIZE * 2) BYTE */

/*
 * DATATYPE FOR GENERAL DECODER PURPOSES
 */
typedef struct DECODER_t
{
    unsigned char SUBROUTINE;
    unsigned char PROTOCOL;
    unsigned char FADESPEED;
    unsigned char FADEDELAY;
    unsigned char MAXPWM;
    unsigned char NIGHTPWM;
    unsigned char ADDRESS_SIZE;
    unsigned char (*PROTOCOL_FETCH)(unsigned short *);
}
DECODER_t;

/*
 * DECODER FUNCTIONS
 */
extern DECODER_t DECODER;
extern void DECODER_INIT(void);
extern void DECODER_MAIN(void) __attribute__((noreturn));

#endif
