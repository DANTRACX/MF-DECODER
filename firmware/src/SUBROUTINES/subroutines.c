/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           SUBROUTINES
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        subroutines.c
 *
 * DESCRIPTION:
 *      THIS MODULE IS A COLLECTION OF DECODER OPERATING MODES
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "subroutines.h"

/*
 * FUNCTION FOR READING ALL NEEDED ADDRESSES FROM THE EEPROM
 */
void SUBROUTINE_GET_ADDR(unsigned short *ADDR, unsigned char COUNT)
{
    unsigned temp = 0;
    unsigned char counter = 0;
    unsigned char OFFSET_ADDR = 0;

    DECODER.ADDRESS_SIZE = COUNT;

    for(counter = 0; counter < COUNT; counter++)
    {
        temp = __MEMORY_DRIVER_EEMEM_READ(DECODER_ADDRESS_ADDR + OFFSET_ADDR + 0);
        ADDR[counter] = temp;
        temp = __MEMORY_DRIVER_EEMEM_READ(DECODER_ADDRESS_ADDR + OFFSET_ADDR + 1);
        ADDR[counter] |= ((unsigned short)temp << 8) & 0xFF00;
        OFFSET_ADDR = OFFSET_ADDR + 2;
    }
}

/*
 * FUNCTION FOR READING ALL NEEDED NIGHTMODE ADDRESSES FROM THE EEPROM
 */
void SUBROUTINE_GET_NIGHTADDR(unsigned short *ADDR)
{
    unsigned temp = 0;

    temp = __MEMORY_DRIVER_EEMEM_READ(DECODER_NIGHTPWM_ADDRESS_ADDR + 0);
    ADDR[0] = temp;
    temp = __MEMORY_DRIVER_EEMEM_READ(DECODER_NIGHTPWM_ADDRESS_ADDR + 1);
    ADDR[0] |= ((unsigned short)temp << 8) & 0xFF00;
    temp = __MEMORY_DRIVER_EEMEM_READ(DECODER_NIGHTPWM_ADDRESS_ADDR + 2);
    ADDR[1] = temp;
    temp = __MEMORY_DRIVER_EEMEM_READ(DECODER_NIGHTPWM_ADDRESS_ADDR + 3);
    ADDR[1] |= ((unsigned short)temp << 8) & 0xFF00;
}

/*
 * FUNCTION FOR CONVERTING MAX PWM LEVEL
 */
void SUBROUTINE_GET_MAXPWM(unsigned char *PWM_LEVEL)
{
   *PWM_LEVEL = (110 - (((DECODER.MAXPWM) + 1) * 10));
}

/*
 * FUNCTION FOR CONVERTING NIGHTMODE PWM LEVEL
 */
void SUBROUTINE_GET_NIGHTPWM(unsigned char *PWM_LEVEL)
{
    *PWM_LEVEL = (110 - (((DECODER.NIGHTPWM) + 1) * 10));
}
