/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           SUBROUTINE
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        subroutine09.c
 *
 * DESCRIPTION:
 *      RAILROAD CROSSING WITH TWO BLINKNG ANDREAD CROSSES PER CHANNEL
 *      AND TWO BLINKING MONITORING SIGNALS INCLUDING PERMANENT LIGHTS
 *      OUTPUT ASSIGNMENTS:
 *
 *      |+|01|02|03|04|05|06|07|08|+|
 *      |+|A1|A1|A2|A2|M1|P1|M2|P2|+|
 *
 *      A = ANDREAS CROSS
 *      M = MONITORING SIGNAL (BLINKING)
 *      P = PERSISTENT LIGHTS FOR MONITORING SIGNAL
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "subroutines.h"

typedef struct SIGNAL_t
{
    enum STATUS_t
    {
        ON,
        OFF
    }
    STATUS;

    PWMDIMM_CHANNEL_t CHANNEL;
    unsigned char DELAY;
    unsigned char PWM;
}
SIGNAL_t;

static void SIGNAL_SET(SIGNAL_t *SIGNAL)
{
    PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL), SIGNAL->PWM);
}

static void SIGNAL_CLEAR(SIGNAL_t *SIGNAL)
{
    PWMDIMM_CHANNEL_FADE(&(SIGNAL->CHANNEL), 0);
}

void subroutine_09_MAIN()
{
    unsigned short SIGNAL_ADDR[2];
    unsigned short currentCmd = 0xFFFF;
    unsigned short prevCmd = 0xFFFF;
    unsigned char SWITCH_DELAY = (DECODER.FADEDELAY + 1) * 2;
    unsigned char enabled = 0;
    SIGNAL_t SIGNAL[4];

    SUBROUTINE_GET_ADDR(&(SIGNAL_ADDR[0]), 2);
    SUBROUTINE_GET_MAXPWM(&(SIGNAL[0].PWM));
    SUBROUTINE_GET_MAXPWM(&(SIGNAL[1].PWM));
    SUBROUTINE_GET_MAXPWM(&(SIGNAL[2].PWM));
    SUBROUTINE_GET_MAXPWM(&(SIGNAL[3].PWM));

    PWMDIMM_INITIALIZE(1);

    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[0].CHANNEL), GPIO_OUT_01 | GPIO_OUT_02);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[1].CHANNEL), GPIO_OUT_03 | GPIO_OUT_04);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[2].CHANNEL), GPIO_OUT_05 | GPIO_OUT_07);
    PWMDIMM_CHANNEL_REGISTER(&(SIGNAL[3].CHANNEL), GPIO_OUT_06 | GPIO_OUT_08);

    SIGNAL[0].STATUS = OFF;
    SIGNAL[1].STATUS = OFF;
    SIGNAL[2].STATUS = OFF;
    SIGNAL[3].STATUS = ON;

    SIGNAL_SET(&SIGNAL[3]);

    SCHEDULER_SECONDS_SET_TIME(&SIGNAL[0].DELAY);
    SCHEDULER_SECONDS_SET_TIME(&SIGNAL[1].DELAY);
    SCHEDULER_SECONDS_SET_TIME(&SIGNAL[2].DELAY);

    do
    {
        if(GPIO_SWITCH_GET_CHOICE() == 0x00)
        {
            if(enabled == 0)
            {
                enabled = 1;
            }

            else
            {
                enabled = 0;
            }

            GPIO_SWITCH_RESET_CHOICE();
        }

        if(DECODER.PROTOCOL_FETCH(&currentCmd) == 1)
        {
            if(currentCmd != prevCmd)
            {
                prevCmd = currentCmd;

                if((currentCmd == SIGNAL_ADDR[0]))
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if(enabled == 0)
                    {
                        enabled = 1;
                    }
                }

                else if((currentCmd == SIGNAL_ADDR[1]))
                {
                    SCHEDULER_MONITOR_LED_SET();

                    if(enabled == 1)
                    {
                        enabled = 0;
                    }
                }
            }
        }

        /* BAHNÜBERGANG */
        if((SCHEDULER_SECONDS_GET_TIME(&SIGNAL[0].DELAY)) >= SWITCH_DELAY)
        {
            if(SIGNAL[0].STATUS == ON)
            {
                SIGNAL_CLEAR(&SIGNAL[0]);
                SIGNAL[0].STATUS = OFF;

                if(enabled == 1)
                {
                    SIGNAL_SET(&SIGNAL[1]);
                    SIGNAL[1].STATUS = ON;
                }
            }

            else if(enabled == 1)
            {
                SIGNAL_SET(&SIGNAL[0]);
                SIGNAL[0].STATUS = ON;

                if(SIGNAL[1].STATUS == ON)
                {
                    SIGNAL_CLEAR(&SIGNAL[1]);
                    SIGNAL[1].STATUS = OFF;
                }
            }

            else if((enabled == 0) && (SIGNAL[1].STATUS == ON))
            {
                SIGNAL_CLEAR(&SIGNAL[1]);
                SIGNAL[1].STATUS = OFF;
            }

            SCHEDULER_SECONDS_SET_TIME(&SIGNAL[0].DELAY);
        }

        /* ÜBERWACHUNGSSIGNAL */
        if((SCHEDULER_SECONDS_GET_TIME(&SIGNAL[2].DELAY)) >= 5)
        {
            if(SIGNAL[2].STATUS == ON)
            {
                SIGNAL_CLEAR(&SIGNAL[2]);
                SIGNAL[2].STATUS = OFF;
            }

            else if(enabled == 1)
            {
                SIGNAL_SET(&SIGNAL[2]);
                SIGNAL[2].STATUS = ON;
            }

            SCHEDULER_SECONDS_SET_TIME(&SIGNAL[2].DELAY);
        }
    }
    while((GPIO_SWITCH_GET_CHOICE() == 0xFF) || (GPIO_SWITCH_GET_CHOICE() == 0x00));

    PWMDIMM_REINITIALIZE();
}
