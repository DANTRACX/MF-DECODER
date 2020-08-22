/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           SUBROUTINE
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        subroutine10.c
 *
 * DESCRIPTION:
 *      THREE SWITCHES WITH SCALEABLE ON-TIME
 *      !! THE PORTS SHOULD BE BRIDGED TO SUSTAIN A SAVE OPERATION !!
 *      OUTPUT ASSIGNMENTS:
 *
 *      |+| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |+|       |-|+| 9 | 10|
 *      |+|S1L|S1L|S1R|S1R|S2L|S2L|S2R|S2R|+|       |-|+|S3L|S3R|
 *          |___|   |___|   |___|   |___|
 *            B       B       B       B
 *
 *      S = SWITCH
 *      L = LEFT SWITCH
 *      R = RIGHT SWITCH
 *      B = BRIDGE (EXTERNAL)
 *
 *      USE POR 9 AND 10 ONLY WITH EXTERNAL HARDWARE E.G. A TRANSISTOR
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "subroutines.h"

#define SOFT_PWM_CMP 2500
#define SOFT_PWM_TOP 5000

typedef struct WEICHE_t
{
    enum STATUS_t
    {
        ENABLED,
        STANDBY
    }
    STATUS;

    enum POSITION_t
    {
        EVEN,
        ODD,
        FLOATING,
        EMPTY
    }
    POSITION, QUEUE;

    PWMDIMM_CHANNEL_t CHANNEL_EVEN;
    PWMDIMM_CHANNEL_t CHANNEL_ODD;
    unsigned char ON_TIME;
}
WEICHE_t;

static void WEICHE_SET(WEICHE_t *WEICHE, unsigned char DIRECTION)
{
    if(DIRECTION == 0)
    {
        PWMDIMM_CHANNEL_STANDALONE_SET(&(WEICHE->CHANNEL_EVEN));
    }

    else
    {
        PWMDIMM_CHANNEL_STANDALONE_SET(&(WEICHE->CHANNEL_ODD));
    }
}

static void WEICHE_CLEAR(WEICHE_t *WEICHE, unsigned char DIRECTION)
{
    if(DIRECTION == 0)
    {
        PWMDIMM_CHANNEL_STANDALONE_CLEAR(&(WEICHE->CHANNEL_EVEN));
    }

    else
    {
        PWMDIMM_CHANNEL_STANDALONE_CLEAR(&(WEICHE->CHANNEL_ODD));
    }
}

void subroutine_10_MAIN()
{
    unsigned short SIGNAL_ADDR[6];
    unsigned short currentCmd = 0xFFFF;
    unsigned short prevCmd = 0xFFFF;
    unsigned char SWITCH_DELAY = (DECODER.FADEDELAY + 1) * 5;
    unsigned long PWM_COUNTER = 0;
    WEICHE_t WEICHE[3];
    SUBROUTINE_GET_ADDR(&(SIGNAL_ADDR[0]), 6);

    PWMDIMM_INITIALIZE(0);
    PWMDIMM_CHANNEL_STANDALONE(&(WEICHE[0].CHANNEL_EVEN), GPIO_OUT_01 | GPIO_OUT_02);
    PWMDIMM_CHANNEL_STANDALONE(&(WEICHE[0].CHANNEL_ODD), GPIO_OUT_03 | GPIO_OUT_04);
    PWMDIMM_CHANNEL_STANDALONE(&(WEICHE[1].CHANNEL_EVEN), GPIO_OUT_05 | GPIO_OUT_06);
    PWMDIMM_CHANNEL_STANDALONE(&(WEICHE[1].CHANNEL_ODD), GPIO_OUT_07 | GPIO_OUT_08);
    PWMDIMM_CHANNEL_STANDALONE(&(WEICHE[2].CHANNEL_EVEN), GPIO_OUT_09);
    PWMDIMM_CHANNEL_STANDALONE(&(WEICHE[2].CHANNEL_ODD), GPIO_OUT_10);

    WEICHE[0].STATUS = STANDBY;
    WEICHE[0].POSITION = FLOATING;
    WEICHE[0].QUEUE = EMPTY;
    WEICHE[1].STATUS = STANDBY;
    WEICHE[1].POSITION = FLOATING;
    WEICHE[1].QUEUE = EMPTY;
    WEICHE[2].STATUS = STANDBY;
    WEICHE[2].POSITION = FLOATING;
    WEICHE[1].QUEUE = EMPTY;

    do
    {
        if(DECODER.PROTOCOL_FETCH(&currentCmd) == 1)
        {
            if(currentCmd != prevCmd)
            {
                prevCmd = currentCmd;

                if((currentCmd == SIGNAL_ADDR[0]))
                {
                    SCHEDULER_MONITOR_LED_SET();
                    WEICHE[0].QUEUE = EVEN;
                }

                else if((currentCmd == SIGNAL_ADDR[1]))
                {
                    SCHEDULER_MONITOR_LED_SET();
                    WEICHE[0].QUEUE = ODD;
                }

                else if((currentCmd == SIGNAL_ADDR[2]))
                {
                    SCHEDULER_MONITOR_LED_SET();
                    WEICHE[1].QUEUE = EVEN;
                }

                else if((currentCmd == SIGNAL_ADDR[3]))
                {
                    SCHEDULER_MONITOR_LED_SET();
                    WEICHE[1].QUEUE = ODD;
                }

                else if((currentCmd == SIGNAL_ADDR[4]))
                {
                    SCHEDULER_MONITOR_LED_SET();
                    WEICHE[2].QUEUE = EVEN;
                }

                else if((currentCmd == SIGNAL_ADDR[5]))
                {
                    SCHEDULER_MONITOR_LED_SET();
                    WEICHE[2].QUEUE = ODD;
                }
            }
        }

        if((WEICHE[0].QUEUE != EMPTY) && (WEICHE[0].QUEUE != FLOATING) && (WEICHE[0].STATUS == STANDBY))
        {
            WEICHE[0].POSITION = WEICHE[0].QUEUE;
            WEICHE[0].QUEUE = EMPTY;
            WEICHE[0].STATUS = ENABLED;

            if(WEICHE[0].POSITION == EVEN)
            {
                WEICHE_SET(&(WEICHE[0]), 0);
            }

            else if(WEICHE[0].POSITION == ODD)
            {
                WEICHE_SET(&(WEICHE[0]), 1);
            }

            SCHEDULER_SECONDS_SET_TIME(&(WEICHE[0].ON_TIME));
        }

        if((WEICHE[1].QUEUE != EMPTY) && (WEICHE[1].QUEUE != FLOATING) && (WEICHE[1].STATUS == STANDBY))
        {
            WEICHE[1].POSITION = WEICHE[1].QUEUE;
            WEICHE[1].QUEUE = EMPTY;
            WEICHE[1].STATUS = ENABLED;

            if(WEICHE[1].POSITION == EVEN)
            {
                WEICHE_SET(&(WEICHE[1]), 0);
            }

            else if(WEICHE[1].POSITION == ODD)
            {
                WEICHE_SET(&(WEICHE[1]), 1);
            }

            SCHEDULER_SECONDS_SET_TIME(&(WEICHE[1].ON_TIME));
        }

        if((WEICHE[2].QUEUE != EMPTY) && (WEICHE[2].QUEUE != FLOATING) && (WEICHE[2].STATUS == STANDBY))
        {
            WEICHE[2].POSITION = WEICHE[2].QUEUE;
            WEICHE[2].QUEUE = EMPTY;
            WEICHE[2].STATUS = ENABLED;

            if(WEICHE[2].POSITION == EVEN)
            {
                WEICHE_SET(&(WEICHE[2]), 0);
            }

            else if(WEICHE[2].POSITION == ODD)
            {
                WEICHE_SET(&(WEICHE[2]), 1);
            }

            SCHEDULER_SECONDS_SET_TIME(&(WEICHE[2].ON_TIME));
        }

        if(++PWM_COUNTER == SOFT_PWM_TOP)
        {
            PWM_COUNTER = 0;
        }

        if(WEICHE[0].STATUS != STANDBY)
        {
            if(PWM_COUNTER == 0)
            {
                if(WEICHE[0].POSITION == EVEN)
                {
                    WEICHE_SET(&(WEICHE[0]), 0);
                }

                else if(WEICHE[0].POSITION == ODD)
                {
                    WEICHE_SET(&(WEICHE[0]), 1);
                }
            }

            else if(PWM_COUNTER == SOFT_PWM_CMP)
            {
                if(WEICHE[0].POSITION == EVEN)
                {
                    WEICHE_CLEAR(&(WEICHE[0]), 0);
                }

                else if(WEICHE[0].POSITION == ODD)
                {
                    WEICHE_CLEAR(&(WEICHE[0]), 1);
                }
            }

            if((SCHEDULER_SECONDS_GET_TIME(&WEICHE[0].ON_TIME)) >= SWITCH_DELAY)
            {
                WEICHE[0].STATUS = STANDBY;

                if(WEICHE[0].POSITION == EVEN)
                {
                    WEICHE_CLEAR(&WEICHE[0], 0);
                }

                else if(WEICHE[0].POSITION == ODD)
                {
                    WEICHE_CLEAR(&WEICHE[0], 1);
                }
            }
        }

        if(WEICHE[1].STATUS != STANDBY)
        {
            if(PWM_COUNTER == SOFT_PWM_CMP)
            {
                if(WEICHE[1].POSITION == EVEN)
                {
                    WEICHE_SET(&(WEICHE[1]), 0);
                }

                else if(WEICHE[1].POSITION == ODD)
                {
                    WEICHE_SET(&(WEICHE[1]), 1);
                }
            }

            else if(PWM_COUNTER == 0)
            {
                if(WEICHE[1].POSITION == EVEN)
                {
                    WEICHE_CLEAR(&(WEICHE[1]), 0);
                }

                else if(WEICHE[1].POSITION == ODD)
                {
                    WEICHE_CLEAR(&(WEICHE[1]), 1);
                }
            }

            if((SCHEDULER_SECONDS_GET_TIME(&WEICHE[1].ON_TIME)) >= SWITCH_DELAY)
            {
                WEICHE[1].STATUS = STANDBY;

                if(WEICHE[1].POSITION == EVEN)
                {
                    WEICHE_CLEAR(&WEICHE[1], 0);
                }

                else if(WEICHE[1].POSITION == ODD)
                {
                    WEICHE_CLEAR(&WEICHE[1], 1);
                }
            }
        }

        if(WEICHE[2].STATUS != STANDBY)
        {
            if(PWM_COUNTER == 0)
            {
                if(WEICHE[2].POSITION == EVEN)
                {
                    WEICHE_SET(&(WEICHE[2]), 0);
                }

                else if(WEICHE[2].POSITION == ODD)
                {
                    WEICHE_SET(&(WEICHE[2]), 1);
                }
            }

            else if(PWM_COUNTER == SOFT_PWM_CMP)
            {
                if(WEICHE[2].POSITION == EVEN)
                {
                    WEICHE_CLEAR(&(WEICHE[2]), 0);
                }

                else if(WEICHE[2].POSITION == ODD)
                {
                    WEICHE_CLEAR(&(WEICHE[2]), 1);
                }
            }

            if((SCHEDULER_SECONDS_GET_TIME(&WEICHE[2].ON_TIME)) >= SWITCH_DELAY)
            {
                WEICHE[2].STATUS = STANDBY;

                if(WEICHE[2].POSITION == EVEN)
                {
                    WEICHE_CLEAR(&WEICHE[2], 0);
                }

                else if(WEICHE[2].POSITION == ODD)
                {
                    WEICHE_CLEAR(&WEICHE[2], 1);
                }
            }
        }
    }
    while((GPIO_SWITCH_GET_CHOICE() == 0xFF) || (GPIO_SWITCH_GET_CHOICE() == 0x00));

    PWMDIMM_REINITIALIZE();
}
