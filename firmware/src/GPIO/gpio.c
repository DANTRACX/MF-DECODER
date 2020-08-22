/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           GPIO
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        gpio.c
 *
 * DESCRIPTION:
 *      THIS MODULE GIVES A LOW LEVEL IO ACCESS TO THE USER
 *      IT CONTROLS INPUT/OUTPUT AND EXTERNAL INTERRUPTS
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#include "gpio.h"

/*
 * DEFINE TIMING VALUE FOR SWITCH DEBOUNCING (ADDITIONALLY TO THE CAPACITOR)
 */
#define GPIO_SWITCH_DEBOUNCE ((20000000 / 1000) * 50)

/*
 * DATATYPE FOR THE PROGRAMING SWITCH
 * ADDS FUNCTIONALLITY FOR NUMBER OF PROGRAMMING SELECTIONS AND TWO KIND OF RESET FUNCTIONS
 */
struct GPIO_t
{
    volatile unsigned char SWITCH_SCOPE;
    volatile unsigned char SWITCH_CHOICE;
    void (*SOFT_RESET_FUNC)(void);
    void (*HARD_RESET_FUNC)(void);
}
GPIO;

/*
 * INTERRUPT FUNCTIONS FOR THE PROGRAMMING SWITCH AND THE RESET FUNCTIONS
 */
static void GPIO_SWITCH_INTERRUPT(void);
static void GPIO_PRE_RESET_DUMMY(void);

/*
 * INITIALIZE THE GENERAL PORT I/O
 * SET RESET FUNCTIONS TO DUMMY FUNCTIONS
 * ENABLE THE PROGRAMING SWITCH
 */
void GPIO_INITIALIZE(void)
{
    GPIO_LED_RD_INIT();
    GPIO_LED_YE_INIT();
    GPIO_LED_GN_INIT();

    GPIO_LED_RD_OFF();
    GPIO_LED_YE_OFF();
    GPIO_LED_GN_OFF();

    GPIO.SWITCH_SCOPE = 0x00;
    GPIO.SWITCH_CHOICE = 0xFF;
    GPIO.SOFT_RESET_FUNC = GPIO_PRE_RESET_DUMMY;
    GPIO.HARD_RESET_FUNC = GPIO_PRE_RESET_DUMMY;

    __IO_DRIVER_INTERRUPT_INT1_SETFUNC(GPIO_SWITCH_INTERRUPT);
    __IO_DRIVER_INTERRUPT_INT1_ENABLE();
    __IO_DRIVER_INTERRUPT_INT1_SENSE_LOW();
}

/*
 * SET THE NUMBER OF PROGRAMING MODES WHICH ARE USEABLE
 */
void GPIO_SWITCH_SET_SCOPE(unsigned char scope)
{
    GPIO.SWITCH_SCOPE = scope;
    GPIO.SWITCH_CHOICE = 0xFF;
}

/*
 * GET THE USERS DECISION AFTER HE PUSHED THE PROGRAMING SWITCH
 */
unsigned char GPIO_SWITCH_GET_CHOICE(void)
{
    return GPIO.SWITCH_CHOICE;
}

/*
 * RESET THE USERS CHOICE AFTER HE PUSHED THE PROGRAMING SWITCH
 */
void GPIO_SWITCH_RESET_CHOICE(void)
{
    GPIO.SWITCH_CHOICE = 0xFF;
}

/*
 * SET A SOFT RESET FUNCTIONS WHICH WILL BE EXECUTED WHEN THE YELLOW LED LIGHTS CONTINOUSLY
 */
void GPIO_SET_SOFT_RESET_FUNC(void (*funcptr)(void))
{
    GPIO.SOFT_RESET_FUNC = funcptr;
}

/*
 * SET A HARD RESET FUNCTION WHICH WILL BE EXECUTED 30SECONDS AFTER THE YELLOW LED LIGHTS CONTINOUSLY
 * ALL THREE STATUS LEDS WILL INDICATE HARD-RESET
 */
void GPIO_SET_HARD_RESET_FUNC(void (*funcptr)(void))
{
    GPIO.HARD_RESET_FUNC = funcptr;
}

/*
 * INTERRUPT FUNCTION WICH SWITCHES THROUGH PROGRAMMING MODES
 * CURRENT PROGRAMING MODE INDICATED BY FLASHING OF THE YELLOW LIGHT
 * INCREASE THE PROGRAMING MODE BY HOLDING THE BUTTON AND COUNTING THE YELLOW FLASHES
 */
static void GPIO_SWITCH_INTERRUPT(void)
{
    unsigned long counter = 0;

    __IO_DRIVER_SREG_CLEAR();
    __builtin_avr_delay_cycles(GPIO_SWITCH_DEBOUNCE);
    GPIO_LED_YE_OFF();

    GPIO.SWITCH_CHOICE = 0x00;

    do
    {
        if((++counter) == 750000)
        {
            GPIO.SWITCH_CHOICE = GPIO.SWITCH_CHOICE + 1;
            GPIO_LED_YE_ON();

            if(GPIO.SWITCH_CHOICE > GPIO.SWITCH_SCOPE)
            {
                GPIO_LED_GN_OFF();
                GPIO_LED_RD_OFF();

                counter = 0;
                while(!(__IO_DRIVER_INTERRUPT_INT1_LEVEL()))
                {
                    if(++counter == 50000000)
                    {
                        GPIO_LED_RD_ON();
                        GPIO_LED_YE_ON();
                        GPIO_LED_GN_ON();
                        while(!(__IO_DRIVER_INTERRUPT_INT1_LEVEL()));
                        __builtin_avr_delay_cycles(GPIO_SWITCH_DEBOUNCE);
                        GPIO.HARD_RESET_FUNC();
                        __builtin_avr_delay_cycles(10000000);
                        __WATCHDOG_DRIVER_START();
                        for(;;){}
                    }
                }

                __builtin_avr_delay_cycles(GPIO_SWITCH_DEBOUNCE);
                GPIO.SOFT_RESET_FUNC();
                __builtin_avr_delay_cycles(10000000);
                __WATCHDOG_DRIVER_START();
                for(;;){}
            }
        }

        if(counter == 1000000)
        {
            GPIO_LED_YE_OFF();
            counter = 0;
        }
    }
    while(!(__IO_DRIVER_INTERRUPT_INT1_LEVEL()));

    __builtin_avr_delay_cycles(GPIO_SWITCH_DEBOUNCE);
    GPIO_LED_YE_OFF();
    __IO_DRIVER_SREG_SET();
}

/*
 * DUMMY RESET FUNCTION WICH WONT DO ANYTHING
 */
static void GPIO_PRE_RESET_DUMMY(void)
{
    __asm__("nop");
}
