/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           DECODER
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        decoder.c
 *
 * DESCRIPTION:
 *      DECODER SOURCE FILE, STORES THE BODY OF ALL FUNCTIONS FOR BASIC
 *      DECODER FUNCTIONALITY
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "decoder.h"

/*
 * NEW GLOBAL DECODER VARIABLE
 */
DECODER_t DECODER;

/*
 * FUNCTION PROTOTYPES
 */
static void DECODER_BLINK_GREEN(unsigned char blinkNumber);
static void DECODER_PRE_RESET_EEMEM_SAVE_SUBROUTINE(void);
static void DECODER_PRE_RESET_EEMEM_SAVE_PROTOCOL(void);
static void DECODER_PRE_RESET_EEMEM_SAVE_FADESPEED(void);
static void DECODER_PRE_RESET_EEMEM_SAVE_FADEDELAY(void);
static void DECODER_PRE_RESET_EEMEM_SAVE_MAXPWM(void);
static void DECODER_PRE_RESET_EEMEM_SAVE_NIGHTPWM(void);
static void DECODER_SOFT_RESET();
static void DECODER_HARD_RESET();

/*
 * INITIALIZE THE DECODER
 * READS ALL DATA FROM EEPROM FOR BASIC OPERATION
 * SET USED PROTOCOL & RESET FUNCTIONS
 */
void DECODER_INIT(void)
{
    __WATCHDOG_DRIVER_STOP();

    GPIO_INITIALIZE();
    GPIO_SWITCH_SET_SCOPE(8);
    GPIO_SET_HARD_RESET_FUNC(DECODER_HARD_RESET);

    SCHEDULER_INITIALIZE();

    DECODER.ADDRESS_SIZE = 0;
    DECODER.SUBROUTINE = __MEMORY_DRIVER_EEMEM_READ(DECODER_SUBROUTINE_ADDR);
    DECODER.PROTOCOL = __MEMORY_DRIVER_EEMEM_READ(DECODER_PROTOCOL_ADDR);
    DECODER.FADESPEED = __MEMORY_DRIVER_EEMEM_READ(DECODER_FADESPEED_ADDR);
    DECODER.FADEDELAY = __MEMORY_DRIVER_EEMEM_READ(DECODER_FADEDELAY_ADDR);
    DECODER.MAXPWM = __MEMORY_DRIVER_EEMEM_READ(DECODER_MAXPWM_ADDR);
    DECODER.NIGHTPWM = __MEMORY_DRIVER_EEMEM_READ(DECODER_NIGHTPWM_ADDR);

    PWMDIMM_INITIALIZE(DECODER.FADESPEED);

    if(DECODER.PROTOCOL == 0x00)
    {
        DCC_INITIALIZE();
        DECODER.PROTOCOL_FETCH = DCC_FETCH;
    }

    else
    {
        MM_INITIALIZE();
        DECODER.PROTOCOL_FETCH = MM_FETCH;
    }
}

/*
 * MAIN DECODER OPERATIONS
 * SELECTS THE CURRENT SUBROUTINE AND SWITCHES TO PROGRAMMING MODE
 */
void DECODER_MAIN(void)
{
    while(1)
    {
        GPIO_LED_RD_OFF();
        GPIO_LED_YE_OFF();
        GPIO_LED_GN_ON();

        switch(DECODER.SUBROUTINE)
        {
            case  0 : subroutine_01_MAIN(); break;
            case  1 : subroutine_02_MAIN(); break;
            case  2 : subroutine_03_MAIN(); break;
            case  3 : subroutine_04_MAIN(); break;
            case  4 : subroutine_05_MAIN(); break;
            case  5 : subroutine_06_MAIN(); break;
            case  6 : subroutine_07_MAIN(); break;
            case  7 : subroutine_08_MAIN(); break;
            case  8 : subroutine_09_MAIN(); break;
            case  9 : subroutine_10_MAIN(); break;
            case 10 : subroutine_11_MAIN(); break;
            default: DECODER_HARD_RESET(); DECODER_SOFT_RESET(); break;
        }

        GPIO_LED_RD_OFF();
        GPIO_LED_YE_OFF();
        GPIO_LED_GN_OFF();

        switch(GPIO_SWITCH_GET_CHOICE())
        {
            // DECODER MODE = SETUP DECODER ADDRESSES
            case 0x01:
            {
                unsigned char addressCounter = 0;
                unsigned short addressOffset = 0;
                unsigned short fetchedData = 0;
                unsigned short previousFetchedData = 0;
                unsigned char blinkCounter = 0;
                unsigned long blinkTimer = 0;
                unsigned char temp = 0;

                GPIO_SWITCH_SET_SCOPE(0);
                GPIO_LED_GN_OFF();

                for(addressCounter = 0; addressCounter < DECODER.ADDRESS_SIZE; addressCounter++)
                {
                    GPIO_SWITCH_RESET_CHOICE();
                    blinkCounter = 0;
                    blinkTimer = 0;

                    while((GPIO_SWITCH_GET_CHOICE() != 0x00) && (DECODER.PROTOCOL_FETCH(&fetchedData) == 0x00))
                    {
                        if((++blinkTimer) == 150000)
                        {
                            GPIO_LED_GN_ON();
                        }

                        else if(blinkTimer == 300000)
                        {
                            GPIO_LED_GN_OFF();
                            blinkCounter++;

                            if(blinkCounter != (addressCounter + 1))
                            {
                                blinkTimer = 0;
                            }
                        }

                        else if(blinkTimer == 1000000)
                        {
                            blinkTimer = 0;
                            blinkCounter = 0;
                        }
                    }

                    GPIO_LED_GN_OFF();
                    SCHEDULER_MONITOR_LED_SET();

                    if(GPIO_SWITCH_GET_CHOICE() == 0x00)
                    {
                        __MEMORY_DRIVER_EEMEM_WRITE((DECODER_ADDRESS_ADDR + addressOffset + 0), 0xFF);
                        __MEMORY_DRIVER_EEMEM_WRITE((DECODER_ADDRESS_ADDR + addressOffset + 1), 0xFF);
                        addressOffset += 2;
                    }

                    else
                    {
                        if(fetchedData != previousFetchedData)
                        {
                            temp = (unsigned char)((fetchedData >> 0) & 0xFF);
                            __MEMORY_DRIVER_EEMEM_WRITE((DECODER_ADDRESS_ADDR + addressOffset + 0), temp);

                            temp = (unsigned char)((fetchedData >> 8) & 0xFF);
                            __MEMORY_DRIVER_EEMEM_WRITE((DECODER_ADDRESS_ADDR + addressOffset + 1), temp);
                            previousFetchedData = fetchedData;
                            addressOffset += 2;
                        }

                        else
                        {
                            addressCounter--;
                        }
                    }
                }

                DECODER_SOFT_RESET();
                break;
            }

            // DECODER MODE = SELECT SUBROUTINE
            case 0x02:
            {
                GPIO_SET_SOFT_RESET_FUNC(DECODER_PRE_RESET_EEMEM_SAVE_SUBROUTINE);

                while(1)
                {
                    DECODER_BLINK_GREEN((DECODER.SUBROUTINE) + 1);

                    if((++(DECODER.SUBROUTINE)) > 10)
                    {
                        DECODER.SUBROUTINE = 0;
                    }
                }

                break;
            }

            // DECODER MODE = SELECT PROTOCOL
            case 0x03:
            {
                GPIO_SET_SOFT_RESET_FUNC(DECODER_PRE_RESET_EEMEM_SAVE_PROTOCOL);

                while(1)
                {
                    DECODER_BLINK_GREEN((DECODER.PROTOCOL) + 1);

                    if((++(DECODER.PROTOCOL)) > 1)
                    {
                        DECODER.PROTOCOL = 0;
                    }
                }

                break;
            }

            // DECODER MODE = SELECT FADE SPEED
            case 0x04:
            {
                GPIO_SET_SOFT_RESET_FUNC(DECODER_PRE_RESET_EEMEM_SAVE_FADESPEED);

                while(1)
                {
                    DECODER_BLINK_GREEN((DECODER.FADESPEED) + 1);

                    if((++(DECODER.FADESPEED)) > 9)
                    {
                        DECODER.FADESPEED = 0;
                    }
                }

                break;
            }

            // DECODER MODE = SELECT FADE DELAY
            case 0x05:
            {
                GPIO_SET_SOFT_RESET_FUNC(DECODER_PRE_RESET_EEMEM_SAVE_FADEDELAY);

                while(1)
                {
                    DECODER_BLINK_GREEN((DECODER.FADEDELAY) + 1);

                    if((++(DECODER.FADEDELAY)) > 9)
                    {
                        DECODER.FADEDELAY = 0;
                    }
                }

                break;
            }

            // DECODER MODE = SELECT MAX PWM
            case 0x06:
            {
                PWMDIMM_CHANNEL_t OUTPUT;

                PWMDIMM_INITIALIZE(0);
                PWMDIMM_CHANNEL_REGISTER(&OUTPUT, (GPIO_OUT_01 | GPIO_OUT_02 | GPIO_OUT_03 | GPIO_OUT_04 | GPIO_OUT_05 | GPIO_OUT_06 | GPIO_OUT_07 | GPIO_OUT_08 | GPIO_OUT_09 | GPIO_OUT_10));
                GPIO_SET_SOFT_RESET_FUNC(DECODER_PRE_RESET_EEMEM_SAVE_MAXPWM);

                while(1)
                {
                    PWMDIMM_CHANNEL_SET(&OUTPUT, (110 - (((DECODER.MAXPWM) + 1) * 10)));
                    DECODER_BLINK_GREEN((DECODER.MAXPWM) + 1);

                    if((++(DECODER.MAXPWM)) > 9)
                    {
                        DECODER.MAXPWM = 0;
                    }
                }

                break;
            }

            // DECODER MODE = SETUP NIGHT MODE
            case 0x07:
            {
                unsigned char addressCounter = 0;
                unsigned short addressOffset = 0;
                unsigned short fetchedData = 0;
                unsigned short previousFetchedData = 0;
                unsigned char blinkCounter = 0;
                unsigned long blinkTimer = 0;
                unsigned char temp = 0;
                PWMDIMM_CHANNEL_t OUTPUT;

                GPIO_SWITCH_SET_SCOPE(0);
                GPIO_LED_GN_OFF();

                for(addressCounter = 0; addressCounter < 2; addressCounter++)
                {
                    GPIO_SWITCH_RESET_CHOICE();
                    blinkCounter = 0;
                    blinkTimer = 0;

                    while((GPIO_SWITCH_GET_CHOICE() != 0x00) && (DECODER.PROTOCOL_FETCH(&fetchedData) == 0x00))
                    {
                        if((++blinkTimer) == 150000)
                        {
                            GPIO_LED_GN_ON();
                        }

                        else if(blinkTimer == 300000)
                        {
                            GPIO_LED_GN_OFF();
                            blinkCounter++;

                            if(blinkCounter != (addressCounter + 1))
                            {
                                blinkTimer = 0;
                            }
                        }

                        else if(blinkTimer == 1000000)
                        {
                            blinkTimer = 0;
                            blinkCounter = 0;
                        }
                    }

                    GPIO_LED_GN_OFF();
                    SCHEDULER_MONITOR_LED_SET();

                    if(GPIO_SWITCH_GET_CHOICE() == 0x00)
                    {
                        addressOffset += 2;
                    }

                    else
                    {
                        if(fetchedData != previousFetchedData)
                        {
                            temp = (unsigned char)((fetchedData >> 0) & 0xFF);
                            __MEMORY_DRIVER_EEMEM_WRITE((DECODER_NIGHTPWM_ADDRESS_ADDR + addressOffset + 0), temp);

                            temp = (unsigned char)((fetchedData >> 8) & 0xFF);
                            __MEMORY_DRIVER_EEMEM_WRITE((DECODER_NIGHTPWM_ADDRESS_ADDR + addressOffset + 1), temp);
                            previousFetchedData = fetchedData;
                            addressOffset += 2;
                        }

                        else
                        {
                            addressCounter--;
                        }
                    }
                }

                PWMDIMM_INITIALIZE(0);
                PWMDIMM_CHANNEL_REGISTER(&OUTPUT, (GPIO_OUT_01 | GPIO_OUT_02 | GPIO_OUT_03 | GPIO_OUT_04 | GPIO_OUT_05 | GPIO_OUT_06 | GPIO_OUT_07 | GPIO_OUT_08 | GPIO_OUT_09 | GPIO_OUT_10));
                GPIO_SET_SOFT_RESET_FUNC(DECODER_PRE_RESET_EEMEM_SAVE_NIGHTPWM);

                while(1)
                {
                    PWMDIMM_CHANNEL_SET(&OUTPUT, (110 - (((DECODER.NIGHTPWM) + 1) * 10)));
                    DECODER_BLINK_GREEN((DECODER.NIGHTPWM) + 1);

                    if((++(DECODER.NIGHTPWM)) > 9)
                    {
                        DECODER.NIGHTPWM = 0;
                    }
                }

                break;
            }

            // DECODER MODE = PORT TEST
            case 0x08:
            {
                unsigned char counter = 0;
                unsigned short OUTPUTS[10] = {GPIO_OUT_01, GPIO_OUT_02, GPIO_OUT_03, GPIO_OUT_04, GPIO_OUT_05, GPIO_OUT_06, GPIO_OUT_07, GPIO_OUT_08, GPIO_OUT_09, GPIO_OUT_10};

                for(counter = 0; counter < 9; counter++)
                {
                    __IO_DRIVER_IO_B_SET_OUT_L((unsigned char)(OUTPUTS[counter] >> 0));
                    __IO_DRIVER_IO_C_SET_OUT_L((unsigned char)(OUTPUTS[counter] >> 8));
                }

                GPIO_SWITCH_SET_SCOPE(0);
                __IO_DRIVER_IO_B_SET_OUT_H((unsigned char)(OUTPUTS[0] >> 0));
                __IO_DRIVER_IO_C_SET_OUT_H((unsigned char)(OUTPUTS[0] >> 8));

                for(counter = 0; counter < 9; counter++)
                {
                    while(GPIO_SWITCH_GET_CHOICE() != 0x00);
                    GPIO_SWITCH_RESET_CHOICE();
                    __IO_DRIVER_IO_B_SET_OUT_L((unsigned char)(OUTPUTS[counter] >> 0));
                    __IO_DRIVER_IO_C_SET_OUT_L((unsigned char)(OUTPUTS[counter] >> 8));

                    __IO_DRIVER_IO_B_SET_OUT_H((unsigned char)(OUTPUTS[counter + 1] >> 0));
                    __IO_DRIVER_IO_C_SET_OUT_H((unsigned char)(OUTPUTS[counter + 1] >> 8));
                }

                while(GPIO_SWITCH_GET_CHOICE() != 0x00);
                DECODER_SOFT_RESET();
                break;
            }
        }
    }
}

/*
 * FUNCTION FOR LED BLINKING
 * INDICATES THE CURRENTLY SELECTED MODE/SETTING
 */
static void DECODER_BLINK_GREEN(unsigned char blinkNumber)
{
    unsigned char blinkCounter = 0;
    unsigned long blinkTimer = 0;

    GPIO_SWITCH_SET_SCOPE(0);
    GPIO_LED_GN_OFF();

    while(GPIO_SWITCH_GET_CHOICE() != 0x00)
    {
        if((++blinkTimer) == 150000)
        {
            GPIO_LED_GN_ON();
        }

        else if(blinkTimer == 300000)
        {
            GPIO_LED_GN_OFF();
            blinkCounter++;

            if(blinkCounter != blinkNumber)
            {
                blinkTimer = 0;
            }
        }

        else if(blinkTimer == 1000000)
        {
            blinkTimer = 0;
            blinkCounter = 0;
        }
    }

    GPIO_LED_GN_OFF();
}

/*
 * FUNCTION FOR SAVING PROGRAMED DATA TO EEPROM AFTER USER SELECTION
 * (ENHANCES EEPROM LIFETIME INSTEAD OF PERMANENTLY WRITING TO IT)
 */
static void DECODER_PRE_RESET_EEMEM_SAVE_SUBROUTINE(void)
{
    __MEMORY_DRIVER_EEMEM_WRITE(DECODER_SUBROUTINE_ADDR, DECODER.SUBROUTINE);
}

static void DECODER_PRE_RESET_EEMEM_SAVE_PROTOCOL(void)
{
    __MEMORY_DRIVER_EEMEM_WRITE(DECODER_PROTOCOL_ADDR, DECODER.PROTOCOL);
}

static void DECODER_PRE_RESET_EEMEM_SAVE_FADESPEED(void)
{
    __MEMORY_DRIVER_EEMEM_WRITE(DECODER_FADESPEED_ADDR, DECODER.FADESPEED);
}

static void DECODER_PRE_RESET_EEMEM_SAVE_FADEDELAY(void)
{
    __MEMORY_DRIVER_EEMEM_WRITE(DECODER_FADEDELAY_ADDR, DECODER.FADEDELAY);
}

static void DECODER_PRE_RESET_EEMEM_SAVE_MAXPWM(void)
{
    __MEMORY_DRIVER_EEMEM_WRITE(DECODER_MAXPWM_ADDR, DECODER.MAXPWM);
}

static void DECODER_PRE_RESET_EEMEM_SAVE_NIGHTPWM(void)
{
    __MEMORY_DRIVER_EEMEM_WRITE(DECODER_NIGHTPWM_ADDR, DECODER.NIGHTPWM);
}

/*
 * DECODER REBOOT
 */
static void DECODER_SOFT_RESET(void)
{
    __WATCHDOG_DRIVER_START();
    for(;;){}
}

/*
 * RESTORE DECODER FACTORY DEFAULTS
 */
static void DECODER_HARD_RESET(void)
{
    unsigned char offsetCounter = 0;

    DECODER.FADEDELAY = 0x00;
    DECODER.FADESPEED = 0x00;
    DECODER.PROTOCOL = 0x00;
    DECODER.SUBROUTINE = 0x00;
    DECODER.MAXPWM = 0x00;
    DECODER.NIGHTPWM = 0x00;

    DECODER_PRE_RESET_EEMEM_SAVE_SUBROUTINE();
    DECODER_PRE_RESET_EEMEM_SAVE_PROTOCOL();
    DECODER_PRE_RESET_EEMEM_SAVE_FADESPEED();
    DECODER_PRE_RESET_EEMEM_SAVE_FADEDELAY();
    DECODER_PRE_RESET_EEMEM_SAVE_MAXPWM();
    DECODER_PRE_RESET_EEMEM_SAVE_NIGHTPWM();

    __MEMORY_DRIVER_EEMEM_WRITE((DECODER_NIGHTPWM_ADDRESS_ADDR + 0), 0xFF);
    __MEMORY_DRIVER_EEMEM_WRITE((DECODER_NIGHTPWM_ADDRESS_ADDR + 1), 0xFF);
    __MEMORY_DRIVER_EEMEM_WRITE((DECODER_NIGHTPWM_ADDRESS_ADDR + 2), 0xFF);
    __MEMORY_DRIVER_EEMEM_WRITE((DECODER_NIGHTPWM_ADDRESS_ADDR + 3), 0xFF);

    for(offsetCounter = 0; offsetCounter < 100; offsetCounter++)
    {
        __MEMORY_DRIVER_EEMEM_WRITE((DECODER_ADDRESS_ADDR + offsetCounter), 0xFF);
    }
}
