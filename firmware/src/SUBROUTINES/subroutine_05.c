/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           SUBROUTINE
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        subroutine05.c
 *
 * DESCRIPTION:
 *      ONE SIGNAL WITH THREE STATES (HP0, HP1, HP2)
 *      AND ONE VORSIGNAL WITH WITH FOUR STATES (VR0, VR1, VR2, OFF)
 *      WITHOUT BLANKING
 *      OUTPUT ASSIGNMENTS:
 *
 *      |+|01|02|03|04|05|06|07|08|+|
 *      |+|HR|HG|HY|00|VY|VY|VG|VG|+|
 *
 *      R  = RED LED            H = HAUPTSIGNAL
 *      G  = GREEN LED          V = VORSIGNAL
 *      Y  = YELLOW LED
 *      00 = NOT CONNECTED
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "subroutines.h"

typedef struct HAUPTSIGNAL_t
{
    enum HAUPTSTATUS_t
    {
        HP0_active,
        HP0_pending,
        HP1_active,
        HP1_pending,
        HP2_active,
        HP2_pending
    }
    STATUS;

    PWMDIMM_CHANNEL_t CHANNEL_R;
    PWMDIMM_CHANNEL_t CHANNEL_Y;
    PWMDIMM_CHANNEL_t CHANNEL_G;
    unsigned char DELAY;
    unsigned char PWM;
}
HAUPTSIGNAL_t;

typedef struct VORSIGNAL_t
{
    enum VORSTATUS_t
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
VORSIGNAL_t;

static void HAUPSIGNAL_SET(HAUPTSIGNAL_t *HAUPTSIGNAL)
{
    if(HAUPTSIGNAL->STATUS == HP0_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(HAUPTSIGNAL->CHANNEL_R), HAUPTSIGNAL->PWM);
        HAUPTSIGNAL->STATUS = HP0_active;
    }

    else if(HAUPTSIGNAL->STATUS == HP1_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(HAUPTSIGNAL->CHANNEL_G), HAUPTSIGNAL->PWM);
        HAUPTSIGNAL->STATUS = HP1_active;
    }

    else if(HAUPTSIGNAL->STATUS == HP2_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(HAUPTSIGNAL->CHANNEL_G), HAUPTSIGNAL->PWM);
        PWMDIMM_CHANNEL_FADE(&(HAUPTSIGNAL->CHANNEL_Y), HAUPTSIGNAL->PWM);
        HAUPTSIGNAL->STATUS = HP2_active;
    }
}

static void HAUPTSIGNAL_CLEAR(HAUPTSIGNAL_t *HAUPTSIGNAL)
{
    if(HAUPTSIGNAL->STATUS == HP0_active)
    {
        PWMDIMM_CHANNEL_FADE(&(HAUPTSIGNAL->CHANNEL_R), 0);
    }

    else if(HAUPTSIGNAL->STATUS == HP1_active)
    {
        PWMDIMM_CHANNEL_FADE(&(HAUPTSIGNAL->CHANNEL_G), 0);
    }

    else if(HAUPTSIGNAL->STATUS == HP2_active)
    {
        PWMDIMM_CHANNEL_FADE(&(HAUPTSIGNAL->CHANNEL_G), 0);
        PWMDIMM_CHANNEL_FADE(&(HAUPTSIGNAL->CHANNEL_Y), 0);
    }
}

static void HAUPTSIGNAL_UPDATE(HAUPTSIGNAL_t *HAUPTSIGNAL, unsigned char PWM)
{
    if(HAUPTSIGNAL->STATUS == HP0_active)
    {
        PWMDIMM_CHANNEL_FADE(&(HAUPTSIGNAL->CHANNEL_R), PWM);
    }

    else if(HAUPTSIGNAL->STATUS == HP1_active)
    {
        PWMDIMM_CHANNEL_FADE(&(HAUPTSIGNAL->CHANNEL_G), PWM);
    }

    else if(HAUPTSIGNAL->STATUS == HP2_active)
    {
        PWMDIMM_CHANNEL_FADE(&(HAUPTSIGNAL->CHANNEL_G), PWM);
        PWMDIMM_CHANNEL_FADE(&(HAUPTSIGNAL->CHANNEL_Y), PWM);
    }
}

static void VORSIGNAL_SET(VORSIGNAL_t *VORSIGNAL)
{
    if(VORSIGNAL->STATUS == VR0_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_Y0), VORSIGNAL->PWM);
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_Y1), VORSIGNAL->PWM);
        VORSIGNAL->STATUS = VR0_active;
    }

    else if(VORSIGNAL->STATUS == VR1_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_G0), VORSIGNAL->PWM);
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_G1), VORSIGNAL->PWM);
        VORSIGNAL->STATUS = VR1_active;
    }

    else if(VORSIGNAL->STATUS == VR2_pending)
    {
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_Y1), VORSIGNAL->PWM);
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_G0), VORSIGNAL->PWM);
        VORSIGNAL->STATUS = VR2_active;
    }

    else if(VORSIGNAL->STATUS == OFF_pending)
    {
        VORSIGNAL->STATUS = OFF_active;
    }
}

static void VORSIGNAL_CLEAR(VORSIGNAL_t *VORSIGNAL)
{
    if(VORSIGNAL->STATUS == VR0_active)
    {
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_Y0), 0);
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_Y1), 0);
    }

    else if(VORSIGNAL->STATUS == VR1_active)
    {
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_G0), 0);
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_G1), 0);
    }

    else if(VORSIGNAL->STATUS == VR2_active)
    {
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_Y1), 0);
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_G0), 0);
    }
}

static void VORSIGNAL_UPDATE(VORSIGNAL_t *VORSIGNAL, unsigned char PWM)
{
    if(VORSIGNAL->STATUS == VR0_active)
    {
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_Y0), PWM);
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_Y1), PWM);
    }

    else if(VORSIGNAL->STATUS == VR1_active)
    {
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_G0), PWM);
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_G1), PWM);
    }

    else if(VORSIGNAL->STATUS == VR2_active)
    {
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_Y1), PWM);
        PWMDIMM_CHANNEL_FADE(&(VORSIGNAL->CHANNEL_G0), PWM);
    }
}

void subroutine_05_MAIN()
{
    unsigned short SIGNAL_ADDR[7];
    unsigned short NIGHT_ADDR[2];
    unsigned short currentCmd = 0xFFFF;
    unsigned short prevCmd = 0xFFFF;
    unsigned short UPDATE_COUNTER = 0;
    unsigned char SWITCH_DELAY = (DECODER.FADEDELAY + 1) * 2;
    HAUPTSIGNAL_t HAUPTSIGNAL;
    VORSIGNAL_t VORSIGNAL;

    SUBROUTINE_GET_ADDR(&(SIGNAL_ADDR[0]), 7);
    SUBROUTINE_GET_NIGHTADDR(&(NIGHT_ADDR[0]));
    SUBROUTINE_GET_MAXPWM(&(HAUPTSIGNAL.PWM));
    SUBROUTINE_GET_MAXPWM(&(VORSIGNAL.PWM));

    PWMDIMM_CHANNEL_REGISTER(&(HAUPTSIGNAL.CHANNEL_R), GPIO_OUT_01);
    PWMDIMM_CHANNEL_REGISTER(&(HAUPTSIGNAL.CHANNEL_G), GPIO_OUT_02);
    PWMDIMM_CHANNEL_REGISTER(&(HAUPTSIGNAL.CHANNEL_Y), GPIO_OUT_03);
    PWMDIMM_CHANNEL_REGISTER(&(VORSIGNAL.CHANNEL_Y0), GPIO_OUT_05);
    PWMDIMM_CHANNEL_REGISTER(&(VORSIGNAL.CHANNEL_Y1), GPIO_OUT_06);
    PWMDIMM_CHANNEL_REGISTER(&(VORSIGNAL.CHANNEL_G0), GPIO_OUT_07);
    PWMDIMM_CHANNEL_REGISTER(&(VORSIGNAL.CHANNEL_G1), GPIO_OUT_08);

    HAUPTSIGNAL.STATUS = HP0_pending;
    VORSIGNAL.STATUS = VR0_pending;

    HAUPSIGNAL_SET(&HAUPTSIGNAL);
    VORSIGNAL_SET(&VORSIGNAL);

    do
    {
        if(UPDATE_COUNTER++ == 65535)
        {
            HAUPTSIGNAL_UPDATE(&HAUPTSIGNAL, HAUPTSIGNAL.PWM);
            VORSIGNAL_UPDATE(&VORSIGNAL, VORSIGNAL.PWM);
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
                    SUBROUTINE_GET_MAXPWM(&(HAUPTSIGNAL.PWM));
                    SUBROUTINE_GET_MAXPWM(&(VORSIGNAL.PWM));
                }

                /* DISABLE NIGHTMODE */
                else if(currentCmd == NIGHT_ADDR[1])
                {
                    SCHEDULER_MONITOR_LED_SET();
                    SUBROUTINE_GET_NIGHTPWM(&(HAUPTSIGNAL.PWM));
                    SUBROUTINE_GET_NIGHTPWM(&(VORSIGNAL.PWM));
                }

                /* HAUPTSIGNAL -> SWITCH-TO-HP0 */
                else if(currentCmd == SIGNAL_ADDR[0])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((HAUPTSIGNAL.STATUS != HP0_active) &&  (HAUPTSIGNAL.STATUS != HP0_pending))
                    {
                        HAUPTSIGNAL_CLEAR(&HAUPTSIGNAL);
                        HAUPTSIGNAL.STATUS = HP0_pending;
                        SCHEDULER_SECONDS_SET_TIME(&HAUPTSIGNAL.DELAY);
                    }
                }

                /* HAUPTSIGNAL -> SWITCH-TO-HP1 */
                else if(currentCmd == SIGNAL_ADDR[1])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((HAUPTSIGNAL.STATUS != HP1_active) &&  (HAUPTSIGNAL.STATUS != HP1_pending))
                    {
                        HAUPTSIGNAL_CLEAR(&HAUPTSIGNAL);
                        HAUPTSIGNAL.STATUS = HP1_pending;
                        SCHEDULER_SECONDS_SET_TIME(&HAUPTSIGNAL.DELAY);
                    }
                }

                /* HAUPTSIGNAL -> SWITCH-TO-HP2 */
                else if(currentCmd == SIGNAL_ADDR[2])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((HAUPTSIGNAL.STATUS != HP2_active) &&  (HAUPTSIGNAL.STATUS != HP2_pending))
                    {
                        HAUPTSIGNAL_CLEAR(&HAUPTSIGNAL);
                        HAUPTSIGNAL.STATUS = HP2_pending;
                        SCHEDULER_SECONDS_SET_TIME(&HAUPTSIGNAL.DELAY);
                    }
                }

                /* VORSIGNAL -> SWITCH-TO-VR0 */
                else if(currentCmd == SIGNAL_ADDR[3])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((VORSIGNAL.STATUS != VR0_active) && (VORSIGNAL.STATUS != VR0_pending))
                    {
                        VORSIGNAL_CLEAR(&VORSIGNAL);
                        VORSIGNAL.STATUS = VR0_pending;
                        SCHEDULER_SECONDS_SET_TIME(&VORSIGNAL.DELAY);
                    }
                }

                /* VORSIGNAL -> SWITCH-TO-VR1 */
                else if(currentCmd == SIGNAL_ADDR[4])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((VORSIGNAL.STATUS != VR1_active) &&  (VORSIGNAL.STATUS != VR1_pending))
                    {
                        VORSIGNAL_CLEAR(&VORSIGNAL);
                        VORSIGNAL.STATUS = VR1_pending;
                        SCHEDULER_SECONDS_SET_TIME(&VORSIGNAL.DELAY);
                    }
                }

                /* VORSIGNAL -> SWITCH-TO-VR2 */
                else if(currentCmd == SIGNAL_ADDR[5])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((VORSIGNAL.STATUS != VR2_active) &&  (VORSIGNAL.STATUS != VR2_pending))
                    {
                        VORSIGNAL_CLEAR(&VORSIGNAL);
                        VORSIGNAL.STATUS = VR2_pending;
                        SCHEDULER_SECONDS_SET_TIME(&VORSIGNAL.DELAY);
                    }
                }

                /* VORSIGNAL -> SWITCH-TO-OFF */
                else if(currentCmd == SIGNAL_ADDR[6])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if((VORSIGNAL.STATUS != OFF_active) && (VORSIGNAL.STATUS != OFF_pending))
                    {
                        VORSIGNAL_CLEAR(&VORSIGNAL);
                        VORSIGNAL.STATUS = OFF_active;
                    }
                }
            }
        }

        /* HAUPTSIGNAL -> DELAY-TO-HP0 */
        if((HAUPTSIGNAL.STATUS == HP0_pending) || (HAUPTSIGNAL.STATUS == HP1_pending) || (HAUPTSIGNAL.STATUS == HP2_pending))
        {
            if((SCHEDULER_SECONDS_GET_TIME(&HAUPTSIGNAL.DELAY)) >= SWITCH_DELAY)
            {
                HAUPSIGNAL_SET(&HAUPTSIGNAL);
            }
        }

        /* VORSIGNAL -> DELAY-TO-VR0 */
        if((VORSIGNAL.STATUS == VR0_pending) || (VORSIGNAL.STATUS == VR1_pending) || (VORSIGNAL.STATUS == VR2_pending) || (VORSIGNAL.STATUS == VR2_pending))
        {
            if((SCHEDULER_SECONDS_GET_TIME(&VORSIGNAL.DELAY)) >= SWITCH_DELAY)
            {
                VORSIGNAL_SET(&VORSIGNAL);
            }
        }
    }
    while((GPIO_SWITCH_GET_CHOICE() == 0xFF) || (GPIO_SWITCH_GET_CHOICE() == 0x00));

    PWMDIMM_REINITIALIZE();
}
