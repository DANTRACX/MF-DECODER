/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           PWMDIMM
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        pwmdimm.h
 *
 * DESCRIPTION:
 *      THIS MODULE IS A BASIC FADER AND DIMMER FOR LEDS WHICH EMITS THE
 *      LIGHT BULB EFFECT.
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#ifndef _PWMDIMM_H_
#define _PWMDIMM_H_

#include "../DRIVERS/IO_DRIVER/IO_DRIVER.h"
#include "../DRIVERS/TIMER2_DRIVER/TIMER2_DRIVER.h"

/*
 * DATATYPE FOR A CHANNEL WHICH MUST BE CREATED BY THE USER MANUALLY
 */
typedef struct PWMDIMM_CHANNEL_t
{
    volatile unsigned char PORTB_MASK;
    volatile unsigned char PORTC_MASK;
    volatile unsigned char const *CURRENT_PWM;
    volatile unsigned char SET_DIFFERENCE;
    volatile signed char FADE_TENDENCY;
    struct PWMDIMM_CHANNEL_t *NEXT;
}
PWMDIMM_CHANNEL_t;

/*
 * FADER FUNCTIONS
 */
extern void PWMDIMM_INITIALIZE(unsigned char FADER_SPEED);
extern void PWMDIMM_REINITIALIZE(void);
extern void PWMDIMM_CHANNEL_STANDALONE(PWMDIMM_CHANNEL_t *pwmChannel, unsigned short cb_mask);
extern void PWMDIMM_CHANNEL_STANDALONE_SET(PWMDIMM_CHANNEL_t *pwmChannel);
extern void PWMDIMM_CHANNEL_STANDALONE_CLEAR(PWMDIMM_CHANNEL_t *pwmChannel);
extern void PWMDIMM_CHANNEL_REGISTER(PWMDIMM_CHANNEL_t *pwmChannel, unsigned short cb_mask);
extern void PWMDIMM_CHANNEL_UNREGISTER(PWMDIMM_CHANNEL_t *pwmChannel);
extern void PWMDIMM_CHANNEL_MODIFY(PWMDIMM_CHANNEL_t *pwmChannel, unsigned short cb_mask);
extern void PWMDIMM_CHANNEL_SET(PWMDIMM_CHANNEL_t *pwmChannel, unsigned char PWM_LEVEL);
extern void PWMDIMM_CHANNEL_FADE(PWMDIMM_CHANNEL_t *pwmChannel, unsigned char PWM_LEVEL);

#endif
