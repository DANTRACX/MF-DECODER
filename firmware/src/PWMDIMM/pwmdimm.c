/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           PWMDIMM
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        pwmdimm.c
 *
 * DESCRIPTION:
 *      THIS MODULE IS A BASIC FADER AND DIMMER FOR LEDS WHICH EMITS THE
 *      LIGHT BULB EFFECT.
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "pwmdimm.h"

/*
 * SET THE "ALLOWED TO FADE" PORTS WITH THESE MASKS
 */
#define PWMDIMM_ENABLED_PORTB_MASK  0x3C
#define PWMDIMM_ENABLED_PORTC_MASK  0x3F

/*
 * NULLPOINTER
 */
#define PWMDIMM_QUEUE_EMPTY         (void *)0
#define PWMDIMM_QUEUE_END           (void *)0

/*
 * PWM VALUES FOR THE COUNTER
 * CALIBRATED TO EMIT A LIGHT BULB WITH AN LED
 */
/*
const unsigned char PWMDIMM_TABLE[51] =
{
    0 ,
    0 , 1  , 1  , 2  , 2  , 2  , 2  , 2  , 3  , 3  ,
    3 , 4  , 4  , 5  , 5  , 6  , 7  , 7  , 8  , 9  ,
    10, 11 , 13 , 14 , 16 , 18 , 20 , 22 , 25 , 28 ,
    31, 35 , 39 , 43 , 48 , 54 , 60 , 67 , 75 , 84 ,
    93, 104, 117, 130, 145, 163, 182, 203, 226, 253
};
*/
const unsigned char PWMDIMM_TABLE[51] =
{
    0,
    0 , 0  , 0  , 1  , 1  , 1  , 1  , 1  , 2  , 2  ,
    2 , 3  , 3  , 4  , 4  , 5  , 6  , 6  , 7  , 8  ,
    9 , 10 , 12 , 13 , 15 , 17 , 19 , 21 , 24 , 27 ,
    30, 34 , 38 , 42 , 47 , 53 , 59 , 66 , 74 , 83 ,
    93, 104, 116, 130, 145, 162, 181, 203, 226, 253
};

/*
 * DATATYPE FOR STORING FADER INFORMATIONS
 */
static struct PWMDIMM_t
{
    unsigned char FADE_SPEED;
    unsigned char FADE_COUNTER;
    PWMDIMM_CHANNEL_t *onQueue;
    PWMDIMM_CHANNEL_t *offQueue;
}
PWMDIMM = {0, 0, PWMDIMM_QUEUE_EMPTY, PWMDIMM_QUEUE_EMPTY};

/*
 * FUNCTION PROTOTYPES FOR THE INTERRUPT FUNCTIONS
 */
static void PWMDIMM_PWM_INTERRUPT_ON(void);
static void PWMDIMM_PWM_INTERRUPT_OFF(void);

/*
 * INITIALIZE THE FADER WITH DEFAULT SETTINGS AND DISABLE IT BY DEFAULT
 */
void PWMDIMM_INITIALIZE(unsigned char FADER_SPEED)
{
    __IO_DRIVER_IO_B_SET_OUT(PWMDIMM_ENABLED_PORTB_MASK);
    __IO_DRIVER_IO_C_SET_OUT(PWMDIMM_ENABLED_PORTC_MASK);

    PWMDIMM.FADE_SPEED = FADER_SPEED;
    PWMDIMM.FADE_COUNTER = 1;
    PWMDIMM.offQueue = PWMDIMM_QUEUE_EMPTY;
    PWMDIMM.onQueue = PWMDIMM_QUEUE_EMPTY;

    __TIMER2_DRIVER_MODE_NORMAL();
    __TIMER2_DRIVER_INTERRUPT_COMPA_SETFUNC(PWMDIMM_PWM_INTERRUPT_OFF);
    __TIMER2_DRIVER_INTERRUPT_OVF_SETFUNC(PWMDIMM_PWM_INTERRUPT_ON);
    __TIMER2_DRIVER_INTERRUPT_COMPA_ENABLE(0x00);
    __TIMER2_DRIVER_INTERRUPT_OVF_ENABLE();
    __TIMER2_DRIVER_CLOCK_OFF();
}

/*
 * REINITIALIZE THE FADER STRUCTURE IN CASE OF MISSING CHANNELS
 */
void PWMDIMM_REINITIALIZE(void)
{
    PWMDIMM_INITIALIZE(PWMDIMM.FADE_SPEED);
}

/*
 * STANDALONE FUNCTIONS FOR DIMM-LESS PORT OPERATION
 */
void PWMDIMM_CHANNEL_STANDALONE(PWMDIMM_CHANNEL_t *pwmChannel, unsigned short cb_mask)
{
    pwmChannel->PORTB_MASK = (unsigned char)((cb_mask >> 0) & PWMDIMM_ENABLED_PORTB_MASK);
    pwmChannel->PORTC_MASK = (unsigned char)((cb_mask >> 8) & PWMDIMM_ENABLED_PORTC_MASK);
    pwmChannel->FADE_TENDENCY = 0;
    pwmChannel->SET_DIFFERENCE = 0;
    pwmChannel->CURRENT_PWM = &(PWMDIMM_TABLE[0]);
    pwmChannel->NEXT = PWMDIMM_QUEUE_EMPTY;
}

void PWMDIMM_CHANNEL_STANDALONE_SET(PWMDIMM_CHANNEL_t *pwmChannel)
{
    __IO_DRIVER_IO_B_SET_OUT_H(pwmChannel->PORTB_MASK);
    __IO_DRIVER_IO_C_SET_OUT_H(pwmChannel->PORTC_MASK);
}

void PWMDIMM_CHANNEL_STANDALONE_CLEAR(PWMDIMM_CHANNEL_t *pwmChannel)
{
    __IO_DRIVER_IO_B_SET_OUT_L(pwmChannel->PORTB_MASK);
    __IO_DRIVER_IO_C_SET_OUT_L(pwmChannel->PORTC_MASK);
}

/*
 * REGISTER A NEW CHANNEL TO THE DIMM ENGINE
 */
void PWMDIMM_CHANNEL_REGISTER(PWMDIMM_CHANNEL_t *pwmChannel, unsigned short cb_mask)
{
    __TIMER2_DRIVER_CLOCK_OFF();

    pwmChannel->PORTB_MASK = (unsigned char)((cb_mask >> 0) & PWMDIMM_ENABLED_PORTB_MASK);
    pwmChannel->PORTC_MASK = (unsigned char)((cb_mask >> 8) & PWMDIMM_ENABLED_PORTC_MASK);
    pwmChannel->FADE_TENDENCY = 0;
    pwmChannel->SET_DIFFERENCE = 0;
    pwmChannel->CURRENT_PWM = &(PWMDIMM_TABLE[0]);
    pwmChannel->NEXT = PWMDIMM.onQueue;
    PWMDIMM.onQueue = pwmChannel;

    __TIMER2_DRIVER_CLOCK_DIV256();
}

/*
 * DELETE A REGISTERED CHANNEL FROM THE DIMM ENGINE
 */
void PWMDIMM_CHANNEL_UNREGISTER(PWMDIMM_CHANNEL_t *pwmChannel)
{
    PWMDIMM_CHANNEL_t *pivot = PWMDIMM.offQueue;
    PWMDIMM_CHANNEL_t *prev = PWMDIMM.offQueue;

    __TIMER2_DRIVER_CLOCK_OFF();

    while(pivot != PWMDIMM_QUEUE_END)
    {
        if(pivot == pwmChannel)
        {
            prev->NEXT = pivot->NEXT;

            if((PWMDIMM.offQueue != PWMDIMM_QUEUE_EMPTY) && (PWMDIMM.onQueue != PWMDIMM_QUEUE_EMPTY))
            {
                __TIMER2_DRIVER_CLOCK_DIV256();
            }

            return;
        }

        prev = pivot;
        pivot = pivot->NEXT;
    }

    pivot = PWMDIMM.onQueue;
    prev = PWMDIMM.onQueue;

    while(pivot != PWMDIMM_QUEUE_END)
    {
        if(pivot == pwmChannel)
        {
            prev->NEXT = pivot->NEXT;

            if((PWMDIMM.offQueue != PWMDIMM_QUEUE_EMPTY) && (PWMDIMM.onQueue != PWMDIMM_QUEUE_EMPTY))
            {
                __TIMER2_DRIVER_CLOCK_DIV256();
            }

            return;
        }

        prev = pivot;
        pivot = pivot->NEXT;
    }

    if((PWMDIMM.offQueue != PWMDIMM_QUEUE_EMPTY) && (PWMDIMM.onQueue != PWMDIMM_QUEUE_EMPTY))
    {
        __TIMER2_DRIVER_CLOCK_DIV256();
    }
}

/*
 * MODIFY THE BITMASKS FROM A REGISTERED CHANNEL
 */
void PWMDIMM_CHANNEL_MODIFY(PWMDIMM_CHANNEL_t *pwmChannel, unsigned short cb_mask)
{
    __TIMER2_DRIVER_CLOCK_OFF();

    __IO_DRIVER_IO_B_SET_OUT_L(pwmChannel->PORTB_MASK);
    __IO_DRIVER_IO_C_SET_OUT_L(pwmChannel->PORTC_MASK);

    pwmChannel->PORTB_MASK = (unsigned char)((cb_mask >> 0) & PWMDIMM_ENABLED_PORTB_MASK);
    pwmChannel->PORTC_MASK = (unsigned char)((cb_mask >> 8) & PWMDIMM_ENABLED_PORTC_MASK);

    __TIMER2_DRIVER_CLOCK_DIV256();
}

/*
 * SET A CHANNEL TO A CONSTANT PWM LEVEL
 */
void PWMDIMM_CHANNEL_SET(PWMDIMM_CHANNEL_t *pwmChannel, unsigned char PWM_LEVEL)
{
    if(PWM_LEVEL < 101)
    {
        __TIMER2_DRIVER_CLOCK_OFF();

        pwmChannel->FADE_TENDENCY = 0;
        pwmChannel->SET_DIFFERENCE = 0;
        pwmChannel->CURRENT_PWM = &(PWMDIMM_TABLE[((PWM_LEVEL / 2) + (PWM_LEVEL % 2))]);

        __TIMER2_DRIVER_CLOCK_DIV256();
    }
}

/*
 * FADE A CHANNEL TO A CONSTANT PWM LEVEL
 */
void PWMDIMM_CHANNEL_FADE(PWMDIMM_CHANNEL_t *pwmChannel, unsigned char PWM_LEVEL)
{
    if(PWM_LEVEL < 101)
    {
        __TIMER2_DRIVER_CLOCK_OFF();

        if(PWMDIMM.FADE_SPEED == 0)
        {
            pwmChannel->FADE_TENDENCY = 0;
            pwmChannel->SET_DIFFERENCE = 0;
            pwmChannel->CURRENT_PWM = &(PWMDIMM_TABLE[((PWM_LEVEL / 2) + (PWM_LEVEL % 2))]);
        }

        else
        {
            if(pwmChannel->CURRENT_PWM < (&(PWMDIMM_TABLE[((PWM_LEVEL / 2) + (PWM_LEVEL % 2))])))
            {
                pwmChannel->FADE_TENDENCY = 1;
                pwmChannel->SET_DIFFERENCE = (unsigned char)((&(PWMDIMM_TABLE[((PWM_LEVEL / 2) + (PWM_LEVEL % 2))])) - (pwmChannel->CURRENT_PWM));
            }

            else if(pwmChannel->CURRENT_PWM > (&(PWMDIMM_TABLE[((PWM_LEVEL / 2) + (PWM_LEVEL % 2))])))
            {
                pwmChannel->FADE_TENDENCY = -1;
                pwmChannel->SET_DIFFERENCE = (unsigned char)((pwmChannel->CURRENT_PWM) - (&(PWMDIMM_TABLE[((PWM_LEVEL / 2) + (PWM_LEVEL % 2))])));
            }

            else
            {
                pwmChannel->FADE_TENDENCY = 0;
                pwmChannel->SET_DIFFERENCE = 0;
            }
        }

        __TIMER2_DRIVER_CLOCK_DIV256();
    }
}

/*
 * INTERRUPT FOR THE OFF-TIME
 */
static void PWMDIMM_PWM_INTERRUPT_OFF(void)
{
    PWMDIMM_CHANNEL_t *newOffQueue = (void *)0;
    PWMDIMM_CHANNEL_t *pivot = PWMDIMM.offQueue;
    PWMDIMM_CHANNEL_t *temp = (void *)0;

    __TIMER2_DRIVER_CLOCK_OFF();
    __TIMER2_DRIVER_INTERRUPT_COMPA_UPDATE(0xFF);

    while(pivot != PWMDIMM_QUEUE_END)
    {
        if((*(pivot->CURRENT_PWM)) <= __TIMER2_DRIVER_TCNT)
        {
            __IO_DRIVER_IO_B_SET_OUT_L(pivot->PORTB_MASK);
            __IO_DRIVER_IO_C_SET_OUT_L(pivot->PORTC_MASK);

            temp = pivot->NEXT;
            pivot->NEXT = PWMDIMM.onQueue;
            PWMDIMM.onQueue = pivot;
            pivot = temp;
        }

        else
        {
            if((*(pivot->CURRENT_PWM)) < __TIMER2_DRIVER_OCRA)
            {
                __TIMER2_DRIVER_INTERRUPT_COMPA_UPDATE(*(pivot->CURRENT_PWM));
            }

            temp = pivot->NEXT;
            pivot->NEXT = newOffQueue;
            newOffQueue = pivot;
            pivot = temp;
        }
    }

    if(__TIMER2_DRIVER_OCRA == 0xFF)
    {
        __TIMER2_DRIVER_INTERRUPT_COMPA_UPDATE(0x00);
    }

    PWMDIMM.offQueue = newOffQueue;
    __TIMER2_DRIVER_CLOCK_DIV256();
}

/*
 * INTERRUPT FOR THE ON-TIME
 */
static void PWMDIMM_PWM_INTERRUPT_ON(void)
{
    PWMDIMM_CHANNEL_t *newOnQueue = (void *)0;
    PWMDIMM_CHANNEL_t *pivot = PWMDIMM.onQueue;
    PWMDIMM_CHANNEL_t *temp = (void *)0;

    __TIMER2_DRIVER_CLOCK_OFF();
    __TIMER2_DRIVER_INTERRUPT_COMPA_UPDATE(0xFF);

    while(pivot != PWMDIMM_QUEUE_END)
    {
        if((PWMDIMM.FADE_COUNTER == PWMDIMM.FADE_SPEED) && (pivot->SET_DIFFERENCE != 0))
        {
            pivot->CURRENT_PWM += pivot->FADE_TENDENCY;
            pivot->SET_DIFFERENCE--;
        }

        if((pivot->CURRENT_PWM) != &(PWMDIMM_TABLE[0]))
        {
            __IO_DRIVER_IO_B_SET_OUT_H(pivot->PORTB_MASK);
            __IO_DRIVER_IO_C_SET_OUT_H(pivot->PORTC_MASK);

            if((*(pivot->CURRENT_PWM)) < __TIMER2_DRIVER_OCRA)
            {
                __TIMER2_DRIVER_INTERRUPT_COMPA_UPDATE(*(pivot->CURRENT_PWM));
            }

            temp = pivot->NEXT;
            pivot->NEXT = PWMDIMM.offQueue;
            PWMDIMM.offQueue = pivot;
            pivot = temp;
        }

        else
        {
            temp = pivot->NEXT;
            pivot->NEXT = newOnQueue;
            newOnQueue = pivot;
            pivot = temp;
        }
    }

    if((PWMDIMM.FADE_COUNTER)++ == PWMDIMM.FADE_SPEED)
    {
        PWMDIMM.FADE_COUNTER = 1;
    }

    if(__TIMER2_DRIVER_OCRA == 0xFF)
    {
        __TIMER2_DRIVER_INTERRUPT_COMPA_UPDATE(0x00);
    }

    PWMDIMM.onQueue = newOnQueue;
    __TIMER2_DRIVER_CLOCK_DIV256();
}
