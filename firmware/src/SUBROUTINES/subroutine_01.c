/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           SUBROUTINE
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        subroutine01.c
 *
 * DESCRIPTION:
 *      FOUR SIGNALS WITH TWO STATES EACH (HP0, HP1)
 *      OUTPUT ASSIGNMENTS:
 *
 *      |+|01|02|03|04|05|06|07|08|+|
 *      |+|R1|G1|R2|G2|R3|G3|R4|G4|+|
 *
 *      R = RED LED
 *      G = GREEN LED
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "subroutines.h"

typedef struct SIGNAL_t
{
    enum STATUS_t
    {
        HP0_active,
        HP0_pending,
        HP1_active,
        HP1_pending
    }
    STATUS;

    PWMDIMM_CHANNEL_t CHANNEL_R;
    PWMDIMM_CHANNEL_t CHANNEL_G;
    unsigned char DELAY;
    unsigned char PWM;
}
SIGNAL_t;

static void SIGNAL_SET(SIGNAL_t *SIGNAL)
{
    if(SIGNAL->STATUS == HP0_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_R), SIGNAL->PWM);
        SIGNAL->STATUS = HP0_active;
    }

    else if(SIGNAL->STATUS == HP1_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G), SIGNAL->PWM);
        SIGNAL->STATUS = HP1_active;
    }
}

static void SIGNAL_CLEAR(SIGNAL_t *SIGNAL)
{
    if(SIGNAL->STATUS == HP0_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_R), 0);
    }

    else if(SIGNAL->STATUS == HP1_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G), 0);
    }
}

static void SIGNAL_UPDATE(SIGNAL_t *SIGNAL, unsigned char PWM)
{
    if(SIGNAL->STATUS == HP0_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_R), PWM);
    }

    else if(SIGNAL->STATUS == HP1_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G), PWM);
    }
}

void subroutine_01_MAIN()
{
    unsigned short SIGNAL_ADDR[8];
    unsigned short NIGHT_ADDR[2];
    unsigned short currentCmd = 0xFFFF;
    unsigned short prevCmd = 0xFFFF;
    unsigned short UPDATE_COUNTER = 0;
    unsigned char SWITCH_DELAY = (DECODER.FADEDELAY + 1) * 2;
    SIGNAL_t SIGNAL[4];

    SUBROUTINE_GET_ADDR(&(SIGNAL_ADDR[0]), 8);
    SUBROUTINE_GET_NIGHTADDR(&(NIGHT_ADDR[0]));
    SUBROUTINE_GET_MAXPWM(&(SIGNAL[0].PWM));
    SUBROUTINE_GET_MAXPWM(&(SIGNAL[1].PWM));
    SUBROUTINE_GET_MAXPWM(&(SIGNAL[2].PWM));
    SUBROUTINE_GET_MAXPWM(&(SIGNAL[3].PWM));

    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[0].CHANNEL_R), GPIO_OUT_01);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[0].CHANNEL_G), GPIO_OUT_02);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[1].CHANNEL_R), GPIO_OUT_03);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[1].CHANNEL_G), GPIO_OUT_04);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[2].CHANNEL_R), GPIO_OUT_05);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[2].CHANNEL_G), GPIO_OUT_06);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[3].CHANNEL_R), GPIO_OUT_07);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[3].CHANNEL_G), GPIO_OUT_08);

    SIGNAL[0].STATUS = HP0_pending;
    SIGNAL[1].STATUS = HP0_pending;
    SIGNAL[2].STATUS = HP0_pending;
    SIGNAL[3].STATUS = HP0_pending;

    SIGNAL_SET(&(SIGNAL[0]));
    SIGNAL_SET(&(SIGNAL[1]));
    SIGNAL_SET(&(SIGNAL[2]));
    SIGNAL_SET(&(SIGNAL[3]));

    do
    {
        if(UPDATE_COUNTER++ == 65535)
        {
            SIGNAL_UPDATE(&SIGNAL[0], SIGNAL[0].PWM);
            SIGNAL_UPDATE(&SIGNAL[1], SIGNAL[1].PWM);
            SIGNAL_UPDATE(&SIGNAL[2], SIGNAL[2].PWM);
            SIGNAL_UPDATE(&SIGNAL[3], SIGNAL[3].PWM);
        }

        if(DECODER.PROTOCOL_FETCH(&currentCmd) == 1)
        {
            if(currentCmd != prevCmd)
            {
                prevCmd = currentCmd;

                /* DISABLE NIGHTMODE */
                if(currentCmd == NIGHT_ADDR[0])
                {
                    SCHEDULER_MONITOR_LED_SET();
                    SUBROUTINE_GET_MAXPWM(&(SIGNAL[0].PWM));
                    SUBROUTINE_GET_MAXPWM(&(SIGNAL[1].PWM));
                    SUBROUTINE_GET_MAXPWM(&(SIGNAL[2].PWM));
                    SUBROUTINE_GET_MAXPWM(&(SIGNAL[3].PWM));
                }

                /* DISABLE NIGHTMODE */
                else if(currentCmd == NIGHT_ADDR[1])
                {
                    SCHEDULER_MONITOR_LED_SET();
                    SUBROUTINE_GET_NIGHTPWM(&(SIGNAL[0].PWM));
                    SUBROUTINE_GET_NIGHTPWM(&(SIGNAL[1].PWM));
                    SUBROUTINE_GET_NIGHTPWM(&(SIGNAL[2].PWM));
                    SUBROUTINE_GET_NIGHTPWM(&(SIGNAL[3].PWM));
                }

                /* SIGNAL1 -> SWITCH-TO-HP0 */
                else if(currentCmd == SIGNAL_ADDR[0])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[0].STATUS != HP0_active) && (SIGNAL[0].STATUS != HP0_pending))
                    {
                        SIGNAL_CLEAR(&(SIGNAL[0]));
                        SIGNAL[0].STATUS = HP0_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[0].DELAY);
                    }
                }

                /* SIGNAL1 -> SWITCH-TO-HP1 */
                else if(currentCmd == SIGNAL_ADDR[1])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[0].STATUS != HP1_active) &&  (SIGNAL[0].STATUS != HP1_pending))
                    {
                        SIGNAL_CLEAR(&(SIGNAL[0]));
                        SIGNAL[0].STATUS = HP1_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[0].DELAY);
                    }
                }

                /* SIGNAL2 -> SWITCH-TO-HP0 */
                else if(currentCmd == SIGNAL_ADDR[2])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[1].STATUS != HP0_active) && (SIGNAL[1].STATUS  != HP0_pending))
                    {
                        SIGNAL_CLEAR(&(SIGNAL[1]));
                        SIGNAL[1].STATUS = HP0_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[1].DELAY);
                    }
                }

                /* SIGNAL2 -> SWITCH-TO-HP1 */
                else if(currentCmd == SIGNAL_ADDR[3])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[1].STATUS  != HP1_active) &&  (SIGNAL[1].STATUS  != HP1_pending))
                    {
                        SIGNAL_CLEAR(&(SIGNAL[1]));
                        SIGNAL[1].STATUS = HP1_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[1].DELAY);
                    }
                }

                /* SIGNAL3 -> SWITCH-TO-HP0 */
                else if(currentCmd == SIGNAL_ADDR[4])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[2].STATUS  != HP0_active) && (SIGNAL[2].STATUS != HP0_pending))
                    {
                        SIGNAL_CLEAR(&(SIGNAL[2]));
                        SIGNAL[2].STATUS = HP0_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[2].DELAY);
                    }
                }

                /* SIGNAL3 -> SWITCH-TO-HP1 */
                else if(currentCmd == SIGNAL_ADDR[5])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[2].STATUS != HP1_active) &&  (SIGNAL[2].STATUS != HP1_pending))
                    {
                        SIGNAL_CLEAR(&(SIGNAL[2]));
                        SIGNAL[2].STATUS = HP1_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[2].DELAY);
                    }
                }

                /* SIGNAL4 -> SWITCH-TO-HP0 */
                else if(currentCmd == SIGNAL_ADDR[6])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[3].STATUS != HP0_active) && (SIGNAL[3].STATUS != HP0_pending))
                    {
                        SIGNAL_CLEAR(&(SIGNAL[3]));
                        SIGNAL[3].STATUS = HP0_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[3].DELAY);
                    }
                }

                /* SIGNAL4 -> SWITCH-TO-HP1 */
                else if(currentCmd == SIGNAL_ADDR[7])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[3].STATUS != HP1_active) &&  (SIGNAL[3].STATUS != HP1_pending))
                    {
                        SIGNAL_CLEAR(&(SIGNAL[3]));
                        SIGNAL[3].STATUS = HP1_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[3].DELAY);
                    }
                }
            }
        }

        /* SIGNAL1 -> DELAY-TO-HP0/HP1 */
        if((SIGNAL[0].STATUS == HP0_pending) || (SIGNAL[0].STATUS == HP1_pending))
        {
            if((SCHEDULER_SECONDS_GET_TIME(&SIGNAL[0].DELAY)) >= SWITCH_DELAY)
            {
                SIGNAL_SET(&(SIGNAL[0]));
            }
        }

        /* SIGNAL2 -> DELAY-TO-HP0/HP1 */
        if((SIGNAL[1].STATUS == HP0_pending) || (SIGNAL[1].STATUS == HP1_pending))
        {
            if((SCHEDULER_SECONDS_GET_TIME(&SIGNAL[1].DELAY)) >= SWITCH_DELAY)
            {
                SIGNAL_SET(&(SIGNAL[1]));
            }
        }

        /* SIGNAL3 -> DELAY-TO-HP0/HP1 */
        if((SIGNAL[2].STATUS == HP0_pending) || (SIGNAL[2].STATUS == HP1_pending))
        {
            if((SCHEDULER_SECONDS_GET_TIME(&SIGNAL[2].DELAY)) >= SWITCH_DELAY)
            {
                SIGNAL_SET(&(SIGNAL[2]));
            }
        }

        /* SIGNAL4 -> DELAY-TO-HP0/HP1 */
        if((SIGNAL[3].STATUS == HP0_pending) || (SIGNAL[3].STATUS == HP1_pending))
        {
            if((SCHEDULER_SECONDS_GET_TIME(&SIGNAL[3].DELAY)) >= SWITCH_DELAY)
            {
                SIGNAL_SET(&(SIGNAL[3]));
            }
        }
    }
    while((GPIO_SWITCH_GET_CHOICE() == 0xFF) || (GPIO_SWITCH_GET_CHOICE() == 0x00));

    PWMDIMM_REINITIALIZE();
}
