/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           SUBROUTINE
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        subroutine04.c
 *
 * DESCRIPTION:
 *      TWO VORSIGNALS WITH FOUR STATES EACH(VR0, VR1, VR2, OFF)
 *      WITHOUT BLANKING
 *      OUTPUT ASSIGNMENTS:
 *
 *      |+|01|02|03|04|05|06|07|08|+|
 *      |+|Y1|Y1|G1|G1|G2|G2|Y2|Y2|+|
 *
 *      G = GREEN LED
 *      Y = YELLOW LED
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "subroutines.h"

typedef struct SIGNAL_t
{
    enum STATUS_t
    {
        VR0_active,
        VR0_pending,
        VR1_active,
        VR1_pending,
        VR2_active,
        VR2_pending,
        OFF_active,
        OFF_pending
    }
    STATUS;

    PWMDIMM_CHANNEL_t CHANNEL_Y0;
    PWMDIMM_CHANNEL_t CHANNEL_Y1;
    PWMDIMM_CHANNEL_t CHANNEL_G0;
    PWMDIMM_CHANNEL_t CHANNEL_G1;
    unsigned char DELAY;
    unsigned char PWM;
}
SIGNAL_t;

static void SIGNAL_SET(SIGNAL_t *SIGNAL)
{
    if(SIGNAL->STATUS == VR0_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_Y0), SIGNAL->PWM);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_Y1), SIGNAL->PWM);
        SIGNAL->STATUS = VR0_active;
    }

    else if(SIGNAL->STATUS == VR1_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G0), SIGNAL->PWM);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G1), SIGNAL->PWM);
        SIGNAL->STATUS = VR1_active;
    }

    else if(SIGNAL->STATUS == VR2_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_Y1), SIGNAL->PWM);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G0), SIGNAL->PWM);
        SIGNAL->STATUS = VR2_active;
    }

    else if(SIGNAL->STATUS == OFF_pending)
    {
        SIGNAL->STATUS = OFF_active;
    }
}

static void SIGNAL_CLEAR(SIGNAL_t *SIGNAL)
{
    if(SIGNAL->STATUS == VR0_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_Y0), 0);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_Y1), 0);
    }

    else if(SIGNAL->STATUS == VR1_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G0), 0);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G1), 0);
    }

    else if(SIGNAL->STATUS == VR2_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_Y1), 0);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G0), 0);
    }
}

static void SIGNAL_UPDATE(SIGNAL_t *SIGNAL, unsigned char PWM)
{
    if(SIGNAL->STATUS == VR0_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_Y0), PWM);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_Y1), PWM);
    }

    else if(SIGNAL->STATUS == VR1_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G0), PWM);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G1), PWM);
    }

    else if(SIGNAL->STATUS == VR2_active)
    {
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_Y1), PWM);
        PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL_G0), PWM);
    }
}

void subroutine_04_MAIN()
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

    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[0].CHANNEL_Y0), GPIO_OUT_01);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[0].CHANNEL_Y1), GPIO_OUT_02);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[0].CHANNEL_G0), GPIO_OUT_03);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[0].CHANNEL_G1), GPIO_OUT_04);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[1].CHANNEL_G1), GPIO_OUT_05);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[1].CHANNEL_G0), GPIO_OUT_06);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[1].CHANNEL_Y1), GPIO_OUT_07);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[1].CHANNEL_Y0), GPIO_OUT_08);

    SIGNAL[0].STATUS = VR0_pending;
    SIGNAL[1].STATUS = VR0_pending;

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

                /* SIGNAL1 -> SWITCH-TO-VR0 */
                else if(currentCmd == SIGNAL_ADDR[0])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[0].STATUS != VR0_active) && (SIGNAL[0].STATUS != VR0_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[0]);
                        SIGNAL[0].STATUS = VR0_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[0].DELAY);
                    }
                }

                /* SIGNAL1 -> SWITCH-TO-VR1 */
                else if(currentCmd == SIGNAL_ADDR[1])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[0].STATUS != VR1_active) &&  (SIGNAL[0].STATUS != VR1_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[0]);
                        SIGNAL[0].STATUS = VR1_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[0].DELAY);
                    }
                }

                /* SIGNAL1 -> SWITCH-TO-VR2 */
                else if(currentCmd == SIGNAL_ADDR[2])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[0].STATUS != VR2_active) &&  (SIGNAL[0].STATUS != VR2_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[0]);
                        SIGNAL[0].STATUS = VR2_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[0].DELAY);
                    }
                }

                /* SIGNAL1 -> SWITCH-TO-OFF */
                else if(currentCmd == SIGNAL_ADDR[3])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[0].STATUS != OFF_active) && (SIGNAL[0].STATUS  != OFF_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[0]);
                        SIGNAL[0].STATUS = OFF_active;
                    }
                }

                /* SIGNAL2 -> SWITCH-TO-VR0 */
                else if(currentCmd == SIGNAL_ADDR[4])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[1].STATUS != VR0_active) && (SIGNAL[1].STATUS  != VR0_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[1]);
                        SIGNAL[1].STATUS = VR0_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[1].DELAY);
                    }
                }

                /* SIGNAL2 -> SWITCH-TO-VR1 */
                else if(currentCmd == SIGNAL_ADDR[5])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[1].STATUS  != VR1_active) &&  (SIGNAL[1].STATUS  != VR1_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[1]);
                        SIGNAL[1].STATUS = VR1_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[1].DELAY);
                    }
                }

                /* SIGNAL2 -> SWITCH-TO-VR2 */
                else if(currentCmd == SIGNAL_ADDR[6])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[1].STATUS  != VR2_active) &&  (SIGNAL[1].STATUS  != VR2_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[1]);
                        SIGNAL[1].STATUS = VR2_pending;
                        SCHEDULER_SECONDS_SET_TIME(&SIGNAL[1].DELAY);
                    }
                }

                /* SIGNAL2 -> SWITCH-TO-OFF */
                else if(currentCmd == SIGNAL_ADDR[7])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((SIGNAL[1].STATUS != OFF_active) && (SIGNAL[1].STATUS  != OFF_pending))
                    {
                        SIGNAL_CLEAR(&SIGNAL[1]);
                        SIGNAL[1].STATUS = OFF_active;
                    }
                }
            }
        }

        /* SIGNAL1 -> DELAY-TO-VR0/VR1/VR2 */
        if((SIGNAL[0].STATUS == VR0_pending) || (SIGNAL[0].STATUS == VR1_pending) || (SIGNAL[0].STATUS == VR2_pending))
        {
            if((SCHEDULER_SECONDS_GET_TIME(&SIGNAL[0].DELAY)) >= SWITCH_DELAY)
            {
                SIGNAL_SET(&SIGNAL[0]);
            }
        }

        /* SIGNAL2 -> DELAY-TO-VR0 */
        if((SIGNAL[1].STATUS == VR0_pending) || (SIGNAL[1].STATUS == VR1_pending) || (SIGNAL[1].STATUS == VR2_pending))
        {
            if((SCHEDULER_SECONDS_GET_TIME(&SIGNAL[1].DELAY)) >= SWITCH_DELAY)
            {
                SIGNAL_SET(&SIGNAL[1]);
            }
        }
    }
    while((GPIO_SWITCH_GET_CHOICE() == 0xFF) || (GPIO_SWITCH_GET_CHOICE() == 0x00));

    PWMDIMM_REINITIALIZE();
}
