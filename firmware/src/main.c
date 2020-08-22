/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        main.c
 *
 * DESCRIPTION:
 *      MAIN FILE, INITIALIZES AND START THE DECODER ROUTINE
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "decoder.h"

int main(void)
{
    DECODER_INIT();
    DECODER_MAIN();

    return 0;
}
