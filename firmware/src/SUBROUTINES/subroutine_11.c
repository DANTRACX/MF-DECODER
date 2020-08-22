/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           SUBROUTINE
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        subroutine11.c
 *
 * DESCRIPTION:
 *      EACH ADDRESS SWITCHES OHNE OUTPUT
 *      ADDITIONALLY OHNE ADDRESS IS CONFIGURED FOR GENERAL OFF AND ONE
 *      ADDRESS FOR GENERAL ON
 *
 *      |+|01|02|03|04|05|06|07|08|+|       |-|+|09|010|
 *      |+|O1|O2|O3|O4|O5|O6|O7|O8|+|       |-|+|O9|O10|
 *
 *      O = OUTPUT
 *
 *      USE POR 9 AND 10 ONLY WITH EXTERNAL HARDWARE E.G. A TRANSISTOR
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "subroutines.h"

typedef struct OUTPUT_t
{
    enum STATUS_t
    {
        ON,
        OFF,
    }
    STATUS;

    enum BLINKING_t
    {
        ENABLED,
        DISABLED
    }
    BLINKING;

    unsigned char PWM;
    PWMDIMM_CHANNEL_t CHANNEL;
}
OUTPUT_t;

static void OUTPUT_SET(OUTPUT_t *OUTPUT)
{
    PWMDIMM_CHANNEL_FADE(&(OUTPUT->CHANNEL), OUTPUT->PWM);
}

static void OUTPUT_CLEAR(OUTPUT_t *OUTPUT)
{
    PWMDIMM_CHANNEL_FADE(&(OUTPUT->CHANNEL), 0);
}

static void OUTPUT_UPDATE(OUTPUT_t *OUTPUT, unsigned char PWM)
{
    PWMDIMM_CHANNEL_FADE(&(OUTPUT->CHANNEL), PWM);
}

void subroutine_11_MAIN()
{
    unsigned short SIGNAL_ADDR[26];
    unsigned short NIGHT_ADDR[2];
    unsigned short currentCmd = 0xFFFF;
    unsigned short prevCmd = 0xFFFF;
    unsigned short UPDATE_COUNTER = 0;
    unsigned char counter = 0;
    unsigned char DELAY = 0;
    unsigned char SWITCH_DELAY = (DECODER.FADEDELAY + 1) * 2;
    OUTPUT_t OUTPUT[10];

    SUBROUTINE_GET_ADDR(&(SIGNAL_ADDR[0]), 26);
    SUBROUTINE_GET_NIGHTADDR(&(NIGHT_ADDR[0]));
    SUBROUTINE_GET_MAXPWM(&(OUTPUT[1].PWM));
    SUBROUTINE_GET_MAXPWM(&(OUTPUT[2].PWM));
    SUBROUTINE_GET_MAXPWM(&(OUTPUT[3].PWM));
    SUBROUTINE_GET_MAXPWM(&(OUTPUT[4].PWM));
    SUBROUTINE_GET_MAXPWM(&(OUTPUT[5].PWM));
    SUBROUTINE_GET_MAXPWM(&(OUTPUT[6].PWM));
    SUBROUTINE_GET_MAXPWM(&(OUTPUT[7].PWM));
    SUBROUTINE_GET_MAXPWM(&(OUTPUT[8].PWM));
    SUBROUTINE_GET_MAXPWM(&(OUTPUT[9].PWM));

    for(counter = 0; counter < 8; counter++)
    {
        OUTPUT[counter].STATUS = OFF;
        OUTPUT[counter].BLINKING = DISABLED;
    }

    SCHEDULER_SECONDS_SET_TIME(&DELAY);

    do
    {
        if(UPDATE_COUNTER++ == 65535)
        {
            for(counter = 0; counter < 8; counter++)
            {
                if((OUTPUT[counter].STATUS == ON) && (OUTPUT[counter].BLINKING == DISABLED))
                {
                    OUTPUT_UPDATE(&(OUTPUT[counter]), OUTPUT[counter].PWM);
                }
            }
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

                    for(counter = 0; counter < 8; counter++)
                    {
                        SUBROUTINE_GET_MAXPWM(&(OUTPUT[counter].PWM));
                    }
                }

                /* DISABLE NIGHTMODE */
                else if(currentCmd == NIGHT_ADDR[1])
                {
                    SCHEDULER_MONITOR_LED_SET();

                    for(counter = 0; counter < 8; counter++)
                    {
                        SUBROUTINE_GET_NIGHTPWM(&(OUTPUT[counter].PWM));
                    }
                }

                /* CMD ALL ON */
                else if((currentCmd == SIGNAL_ADDR[0]))
                {
                    SCHEDULER_MONITOR_LED_SET();

                    for(counter = 0; counter < 8; counter++)
                    {
                        OUTPUT[counter].BLINKING = DISABLED;

                        if(OUTPUT[counter].STATUS != ON)
                        {
                            OUTPUT[counter].STATUS = ON;
                            OUTPUT_SET(&OUTPUT[counter]);
                        }
                    }
                }

                /* CMD ALL OFF */
                else if((currentCmd == SIGNAL_ADDR[1]))
                {
                    SCHEDULER_MONITOR_LED_SET();

                    for(counter = 0; counter < 8; counter++)
                    {
                        OUTPUT[counter].BLINKING = DISABLED;

                        if(OUTPUT[counter].STATUS != OFF)
                        {
                            OUTPUT[counter].STATUS = OFF;
                            OUTPUT_CLEAR(&OUTPUT[counter]);
                        }
                    }
                }

                else
                {
                    for(counter = 0; counter < 8; counter++)
                    {
                        /* CMD ON */
                        if(currentCmd == SIGNAL_ADDR[(3 * counter) + 2])
                        {
                            SCHEDULER_MONITOR_LED_SET();
                            OUTPUT[counter].BLINKING = DISABLED;

                            if(OUTPUT[counter].STATUS != ON)
                            {
                                OUTPUT[counter].STATUS = ON;
                                OUTPUT_SET(&OUTPUT[counter]);
                            }

                            break;
                        }

                        /* CMD BLINK*/
                        else if(currentCmd == SIGNAL_ADDR[(3 * counter) + 3])
                        {
                            SCHEDULER_MONITOR_LED_SET();
                            OUTPUT[counter].BLINKING = ENABLED;
                            break;
                        }

                        /* CMD OFF */
                        else if(currentCmd == SIGNAL_ADDR[(3 * counter) + 4])
                        {
                            SCHEDULER_MONITOR_LED_SET();
                            OUTPUT[counter].BLINKING = DISABLED;

                            if(OUTPUT[counter].STATUS != OFF)
                            {
                                OUTPUT[counter].STATUS = OFF;
                                OUTPUT_CLEAR(&OUTPUT[counter]);
                            }

                            break;
                        }
                    }
                }
            }
        }

        if((SCHEDULER_SECONDS_GET_TIME(&DELAY)) >= SWITCH_DELAY)
        {
            for(counter = 0; counter < 8; counter++)
            {
                if(OUTPUT[counter].BLINKING == ENABLED)
                {
                    if(OUTPUT[counter].STATUS == ON)
                    {
                        OUTPUT[counter].STATUS = OFF;
                        OUTPUT_CLEAR(&OUTPUT[counter]);
                    }

                    else
                    {
                        OUTPUT[counter].STATUS = ON;
                        OUTPUT_SET(&OUTPUT[counter]);
                    }
                }
            }

            SCHEDULER_SECONDS_SET_TIME(&DELAY);
        }
    }
    while((GPIO_SWITCH_GET_CHOICE() == 0xFF) || (GPIO_SWITCH_GET_CHOICE() == 0x00));

    PWMDIMM_REINITIALIZE();
}

