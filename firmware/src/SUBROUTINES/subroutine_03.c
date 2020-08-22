/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           SUBROUTINE
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        subroutine03.c
 *
 * DESCRIPTION:
 *      TWO SIGNALS WITH FOUR STATES (HP00, HP1, HP2, SH1)
 *      OUTPUT ASSIGNMENTS:
 *
 *      |+|01|02|03|04|05|06|07|08|+|       |-|+|09|10|
 *      |+|R1|R1|G1|Y1|W1|R2|R2|G2|+|       |-|+|Y2|W2|
 *
 *      R = RED LED         1 = SIGNAL1
 *      G = GREEN LED       2 = SIGNAL2
 *      Y = YELLOW LED
 *      W = WHITE LED
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "subroutines.h"

typedef struct SIGNAL_t
{
    enum STATUS_t
    {
        HP00_active,
        HP00_pending,
        HP1_active,
        HP1_pending,
        HP2_active,
        HP2_pending,
        SH1_active,
        SH1_pending
    }
    STATUS;

    PWMDIMM_CHANNEL_t CHANNEL_R0;
    PWMDIMM_CHANNEL_t CHANNEL_R1;
    PWMDIMM_CHANNEL_t CHANNEL_Y;
    PWMDIMM_CHANNEL_t CHANNEL_G;
    PWMDIMM_CHANNEL_t CHANNEL_W;
    unsigned char DELAY;
    unsigned char PWM;
}
SIGNAL_t;

static void SIGNAL_SET(SIGNAL_t *SIGNAL)
{
    if(SIGNAL->STATUS == HP00_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_R0), SIGNAL->PWM);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_R1), SIGNAL->PWM);
        SIGNAL->STATUS = HP00_active;
    }

    else if(SIGNAL->STATUS == HP1_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G), SIGNAL->PWM);
        SIGNAL->STATUS = HP1_active;
    }

    else if(SIGNAL->STATUS == HP2_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G), SIGNAL->PWM);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_Y), SIGNAL->PWM);
        SIGNAL->STATUS = HP2_active;
    }

    else if(SIGNAL->STATUS == SH1_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_R1), SIGNAL->PWM);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_W), SIGNAL->PWM);
        SIGNAL->STATUS = SH1_active;
    }
}

static void SIGNAL_CLEAR(SIGNAL_t *SIGNAL)
{
    if(SIGNAL->STATUS == HP00_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_R0), 0);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_R1), 0);
    }

    else if(SIGNAL->STATUS == HP1_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G), 0);
    }

    else if(SIGNAL->STATUS == HP2_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G), 0);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_Y), 0);
    }

    else if(SIGNAL->STATUS == SH1_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_R1), 0);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_W), 0);
    }
}

static void SIGNAL_UPDATE(SIGNAL_t *SIGNAL, unsigned char PWM)
{
    if(SIGNAL->STATUS == HP00_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_R0), PWM);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_R1), PWM);
    }

    else if(SIGNAL->STATUS == HP1_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G), PWM);
    }

    else if(SIGNAL->STATUS == HP2_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G), PWM);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_Y), PWM);
    }

    else if(SIGNAL->STATUS == SH1_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_R1), PWM);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_W), PWM);
    }
}

void subroutine_03_MAIN()
{
    unsigned short SIGNAL_ADDR[8];
    unsigned short NIGHT_ADDR[2];
    unsigned short currentCmd = 0xFFFF;
    unsigned short prevCmd = 0xFFFF;
    unsigned short UPDATE_COUNTER = 0;
    unsigned char SWITCH_DELAY = (DECODER.FADEDELAY + 1) * 2;
    SIGNAL_t SIGNAL[2];

    SUBROUTINE_GET_ADDR(&(SIGNAL_ADDR[0]), 8);
    SUBROUTINE_GET_NIGHTADDR(&(NIGHT_ADDR[0]));
    SUBROUTINE_GET_MAXPWM(&(SIGNAL[0].PWM));
    SUBROUTINE_GET_MAXPWM(&(SIGNAL[1].PWM));

    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[0].CHANNEL_R0), GPIO_OUT_01);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[0].CHANNEL_R1), GPIO_OUT_02);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[0].CHANNEL_G), GPIO_OUT_03);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[0].CHANNEL_Y), GPIO_OUT_04);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[0].CHANNEL_W), GPIO_OUT_05);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[1].CHANNEL_R0), GPIO_OUT_06);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[1].CHANNEL_R1), GPIO_OUT_07);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[1].CHANNEL_G), GPIO_OUT_08);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[1].CHANNEL_Y), GPIO_OUT_09);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[1].CHANNEL_W), GPIO_OUT_10);

    SIGNAL[0].STATUS = HP00_pending;
    SIGNAL[1].STATUS = HP00_pending;

    SIGNAL_SET(&SIGNAL[0]);
    SIGNAL_SET(&SIGNAL[1]);

    do
    {
        if(UPDATE_COUNTER++ == 65535)
        {
            SIGNAL_UPDATE(&SIGNAL[0], SIGNAL[0].PWM);
            SIGNAL_UPDATE(&SIGNAL[1], SIGNAL[1].PWM);
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
                }

                /* DISABLE NIGHTMODE */
                else if(currentCmd == NIGHT_ADDR[1])
                {
                    SCHEDULER_MONITOR_LED_SET();
                    SUBROUTINE_GET_NIGHTPWM(&(SIGNAL[0].PWM));
                    SUBROUTINE_GET_NIGHTPWM(&(SIGNAL[1].PWM));
                }

                /* SIGNAL1 -> SWITCH-TO-HP00 */
                else if(currentCmd == SIGNAL_ADDR[0])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[0].STATUS != HP00_active) && (SIGNAL[0].STATUS != HP00_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[0]);
                        SIGNAL[0].STATUS = HP00_pending;
                        SCHEDULER_SECONDS_SET_TIME(&(SIGNAL[0].DELAY));
                    }
                }

                /* SIGNAL1 -> SWITCH-TO-HP1 */
                else if(currentCmd == SIGNAL_ADDR[1])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[0].STATUS != HP1_active) &&  (SIGNAL[0].STATUS != HP1_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[0]);
                        SIGNAL[0].STATUS = HP1_pending;
                        SCHEDULER_SECONDS_SET_TIME(&(SIGNAL[0].DELAY));
                    }
                }

                /* SIGNAL1 -> SWITCH-TO-HP2 */
                else if(currentCmd == SIGNAL_ADDR[2])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[0].STATUS != HP2_active) &&  (SIGNAL[0].STATUS != HP2_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[0]);
                        SIGNAL[0].STATUS = HP2_pending;
                        SCHEDULER_SECONDS_SET_TIME(&(SIGNAL[0].DELAY));
                    }
                }

                /* SIGNAL1 -> SWITCH-TO-SH1 */
                else if(currentCmd == SIGNAL_ADDR[3])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[0].STATUS != SH1_active) &&  (SIGNAL[0].STATUS != SH1_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[0]);
                        SIGNAL[0].STATUS = SH1_pending;
                        SCHEDULER_SECONDS_SET_TIME(&(SIGNAL[0].DELAY));
                    }
                }

                /* SIGNAL2 -> SWITCH-TO-HP00 */
                else if(currentCmd == SIGNAL_ADDR[4])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[1].STATUS != HP00_active) && (SIGNAL[1].STATUS != HP00_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[1]);
                        SIGNAL[1].STATUS = HP00_pending;
                        SCHEDULER_SECONDS_SET_TIME(&(SIGNAL[1].DELAY));
                    }
                }

                /* SIGNAL2 -> SWITCH-TO-HP1 */
                else if(currentCmd == SIGNAL_ADDR[5])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[1].STATUS != HP1_active) &&  (SIGNAL[1].STATUS != HP1_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[1]);
                        SIGNAL[1].STATUS = HP1_pending;
                        SCHEDULER_SECONDS_SET_TIME(&(SIGNAL[1].DELAY));
                    }
                }

                /* SIGNAL2 -> SWITCH-TO-HP2 */
                else if(currentCmd == SIGNAL_ADDR[6])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[1].STATUS != HP2_active) &&  (SIGNAL[1].STATUS != HP2_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[1]);
                        SIGNAL[1].STATUS = HP2_pending;
                        SCHEDULER_SECONDS_SET_TIME(&(SIGNAL[1].DELAY));
                    }
                }

                /* SIGNAL2 -> SWITCH-TO-SH1 */
                else if(currentCmd == SIGNAL_ADDR[7])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[1].STATUS != SH1_active) &&  (SIGNAL[1].STATUS != SH1_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[1]);
                        SIGNAL[1].STATUS = SH1_pending;
                        SCHEDULER_SECONDS_SET_TIME(&(SIGNAL[1].DELAY));
                    }
                }
            }
        }

        /* SIGNAL1 -> DELAY-TO-HP00/HP1/HP2/SH1 */
        if((SIGNAL[0].STATUS == HP00_pending) || (SIGNAL[0].STATUS == HP1_pending) || (SIGNAL[0].STATUS == HP2_pending) || (SIGNAL[0].STATUS == SH1_pending))
        {
            if((SCHEDULER_SECONDS_GET_TIME(&(SIGNAL[0].DELAY))) >= SWITCH_DELAY)
            {
                SIGNAL_SET(&SIGNAL[0]);
            }
        }

        /* SIGNAL2 -> DELAY-TO-HP00/HP1/HP2/SH1 */
        if((SIGNAL[1].STATUS == HP00_pending) || (SIGNAL[1].STATUS == HP1_pending) || (SIGNAL[1].STATUS == HP2_pending) || (SIGNAL[1].STATUS == SH1_pending))
        {
            if((SCHEDULER_SECONDS_GET_TIME(&(SIGNAL[1].DELAY))) >= SWITCH_DELAY)
            {
                SIGNAL_SET(&SIGNAL[1]);
            }
        }
    }
    while((GPIO_SWITCH_GET_CHOICE() == 0xFF) || (GPIO_SWITCH_GET_CHOICE() == 0x00));

    PWMDIMM_REINITIALIZE();
}
