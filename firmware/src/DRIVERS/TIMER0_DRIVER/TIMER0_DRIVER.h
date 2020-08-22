/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           DRIVER
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        TIMER0_DRIVER.h
 *
 * DESCRIPTION:
 *      THIS MODULE REPRESENTS A DRIVER FOR THE 8-BIT TIMER0 HARDWARE
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#ifndef __TIMER0_DRIVER_H__
#define __TIMER0_DRIVER_H__

/*
 * TIMER/COUNTER CONTROL REGISTER A
 */
#define __TIMER0_DRIVER_TCCRA   (*(volatile unsigned char *)(0x44))
#define __TIMER0_DRIVER_WGM0    0
#define __TIMER0_DRIVER_WGM1    1
#define __TIMER0_DRIVER_COMB0   4
#define __TIMER0_DRIVER_COMB1   5
#define __TIMER0_DRIVER_COMA0   6
#define __TIMER0_DRIVER_COMA1   7

/*
 * TIMER/COUNTER CONTROL REGISTER B
 */
#define __TIMER0_DRIVER_TCCRB   (*(volatile unsigned char *)(0x45))
#define __TIMER0_DRIVER_CS0     0
#define __TIMER0_DRIVER_CS1     1
#define __TIMER0_DRIVER_CS2     2
#define __TIMER0_DRIVER_WGM2    3
#define __TIMER0_DRIVER_FOCB    6
#define __TIMER0_DRIVER_FOCA    7

/*
 * TIMER/COUNTER REGISTER
 */
#define __TIMER0_DRIVER_TCNT    (*(volatile unsigned char *)(0x46))

/*
 * TIMER/COUNTER OUTPUT COMPARE REGISTER A
 */
#define __TIMER0_DRIVER_OCRA    (*(volatile unsigned char *)(0x47))

/*
 * TIMER/COUNTER OUTPUT COMPARE REGISTER B
 */
#define __TIMER0_DRIVER_OCRB    (*(volatile unsigned char *)(0x48))

/*
 * TIMER/COUNTER INTERRUPT MASK REGISTER
 */
#define __TIMER0_DRIVER_TIMSK   (*(volatile unsigned char *)(0x6E))
#define __TIMER0_DRIVER_TOIE    0
#define __TIMER0_DRIVER_OCIEA   1
#define __TIMER0_DRIVER_OCIEB   2

/*
 * TIMER/COUNTER INTERRUPT FLAG REGISTER
 */
#define __TIMER0_DRIVER_TIFR    (*(volatile unsigned char *)(0x35))
#define __TIMER0_DRIVER_TOV     0
#define __TIMER0_DRIVER_OCFA    1
#define __TIMER0_DRIVER_OCFB    2

/*
 * TIMER/COUNTER GENERAL CONTROL REGISTER
 */
#define __TIMER0_DRIVER_GTCCR   (*(volatile unsigned char *)(0x43))
#define __TIMER0_DRIVER_PSRSYNC 0
#define __TIMER0_DRIVER_TSM     7

/*
 * TIMER/COUNTER INTERRUPT SREG
 */
#define __TIMER0_DRIVER_SREG_SET()      __asm__ __volatile__ ("sei" ::: "memory")
#define __TIMER0_DRIVER_SREG_CLEAR()    __asm__ __volatile__ ("cli" ::: "memory")

/*
 * TIMER/COUNTER INTERRUPT VECTORS
 */
#define __TIMER0_DRIVER_COMPA_VECTOR    __vector_14
#define __TIMER0_DRIVER_COMPB_VECTOR    __vector_15
#define __TIMER0_DRIVER_OVF_VECTOR      __vector_16

/*
 * TIMER/COUNTER MODE OF OPERATION
 */
__attribute__((always_inline))
static inline void __TIMER0_DRIVER_MODE_NORMAL(void)
{
    __TIMER0_DRIVER_TCCRA &= ~(1 << __TIMER0_DRIVER_WGM0);
    __TIMER0_DRIVER_TCCRA &= ~(1 << __TIMER0_DRIVER_WGM1);
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_WGM2);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_MODE_CTC(void)
{
    __TIMER0_DRIVER_TCCRA &= ~(1 << __TIMER0_DRIVER_WGM0);
    __TIMER0_DRIVER_TCCRA |=  (1 << __TIMER0_DRIVER_WGM1);
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_WGM2);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_MODE_FASTPWM(void)
{
    __TIMER0_DRIVER_TCCRA |=  (1 << __TIMER0_DRIVER_WGM0);
    __TIMER0_DRIVER_TCCRA |=  (1 << __TIMER0_DRIVER_WGM1);
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_WGM2);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_MODE_FASTPWM_SCALED(unsigned char topValue)
{
    __TIMER0_DRIVER_TCCRA |=  (1 << __TIMER0_DRIVER_WGM0);
    __TIMER0_DRIVER_TCCRA |=  (1 << __TIMER0_DRIVER_WGM1);
    __TIMER0_DRIVER_TCCRB |=  (1 << __TIMER0_DRIVER_WGM2);
    __TIMER0_DRIVER_OCRA = (unsigned char)topValue;
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_MODE_PCPWM(void)
{
    __TIMER0_DRIVER_TCCRA |=  (1 << __TIMER0_DRIVER_WGM0);
    __TIMER0_DRIVER_TCCRA &= ~(1 << __TIMER0_DRIVER_WGM1);
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_WGM2);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_MODE_PCPWM_SCALED(unsigned char topValue)
{
    __TIMER0_DRIVER_TCCRA |=  (1 << __TIMER0_DRIVER_WGM0);
    __TIMER0_DRIVER_TCCRA &= ~(1 << __TIMER0_DRIVER_WGM1);
    __TIMER0_DRIVER_TCCRB |=  (1 << __TIMER0_DRIVER_WGM2);
    __TIMER0_DRIVER_OCRA = (unsigned char)topValue;
}

/*
 * TIMER/COUNTER CLOCK PRESCALER SETUP
 */
__attribute__((always_inline))
static inline void __TIMER0_DRIVER_CLOCK_OFF(void)
{
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_CS0);
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_CS1);
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_CLOCK_DIV1(void)
{
    __TIMER0_DRIVER_TCCRB |=  (1 << __TIMER0_DRIVER_CS0);
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_CS1);
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_CLOCK_DIV8(void)
{
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_CS0);
    __TIMER0_DRIVER_TCCRB |=  (1 << __TIMER0_DRIVER_CS1);
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_CLOCK_DIV64(void)
{
    __TIMER0_DRIVER_TCCRB |=  (1 << __TIMER0_DRIVER_CS0);
    __TIMER0_DRIVER_TCCRB |=  (1 << __TIMER0_DRIVER_CS1);
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_CLOCK_DIV256(void)
{
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_CS0);
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_CS1);
    __TIMER0_DRIVER_TCCRB |=  (1 << __TIMER0_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_CLOCK_DIV1024(void)
{
    __TIMER0_DRIVER_TCCRB |=  (1 << __TIMER0_DRIVER_CS0);
    __TIMER0_DRIVER_TCCRB &= ~(1 << __TIMER0_DRIVER_CS1);
    __TIMER0_DRIVER_TCCRB |=  (1 << __TIMER0_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_CLOCK_T0FALLING(void)
{
    __TIMER0_DRIVER_TCCRB &= (1 << __TIMER0_DRIVER_CS0);
    __TIMER0_DRIVER_TCCRB |= (1 << __TIMER0_DRIVER_CS1);
    __TIMER0_DRIVER_TCCRB |= (1 << __TIMER0_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_CLOCK_T0RISING(void)
{
    __TIMER0_DRIVER_TCCRB |=  (1 << __TIMER0_DRIVER_CS0);
    __TIMER0_DRIVER_TCCRB |=  (1 << __TIMER0_DRIVER_CS1);
    __TIMER0_DRIVER_TCCRB |=  (1 << __TIMER0_DRIVER_CS2);
}

/*
 * TIMER/COUNTER INTERRUPTS
 */
extern void (*__TIMER0_DRIVER_INTERRUPT_COMPA_FUNC)(void);
extern void (*__TIMER0_DRIVER_INTERRUPT_COMPB_FUNC)(void);
extern void (*__TIMER0_DRIVER_INTERRUPT_OVF_FUNC)(void);

extern void __TIMER0_DRIVER_COMPA_VECTOR (void) __attribute__((signal, used, externally_visible));
extern void __TIMER0_DRIVER_COMPB_VECTOR (void) __attribute__((signal, used, externally_visible));
extern void __TIMER0_DRIVER_OVF_VECTOR (void) __attribute__((signal, used, externally_visible));
extern void __TIMER0_DRIVER_VEC_DUMMY(void);

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_INTERRUPT_COMPA_ENABLE(unsigned char matchValue)
{
    __TIMER0_DRIVER_OCRA = (unsigned char)matchValue;
    __TIMER0_DRIVER_TIMSK |= (1 << __TIMER0_DRIVER_OCIEA);
    __TIMER0_DRIVER_SREG_SET();
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_INTERRUPT_COMPA_DISABLE(void)
{
    __TIMER0_DRIVER_TIMSK &= ~(1 << __TIMER0_DRIVER_OCIEA);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_INTERRUPT_COMPA_UPDATE(unsigned char matchValue)
{
    __TIMER0_DRIVER_OCRA = (unsigned char)matchValue;
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_INTERRUPT_COMPA_SETFUNC(void (*funcptr)(void))
{
    __TIMER0_DRIVER_INTERRUPT_COMPA_FUNC = funcptr;
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_INTERRUPT_COMPB_ENABLE(unsigned char matchValue)
{
    __TIMER0_DRIVER_OCRB = (unsigned char)matchValue;
    __TIMER0_DRIVER_TIMSK |= (1 << __TIMER0_DRIVER_OCIEB);
    __TIMER0_DRIVER_SREG_SET();
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_INTERRUPT_COMPB_DISABLE(void)
{
    __TIMER0_DRIVER_TIMSK &= ~(1 << __TIMER0_DRIVER_OCIEB);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_INTERRUPT_COMPB_UPDATE(unsigned char matchValue)
{
    __TIMER0_DRIVER_OCRB = (unsigned char)matchValue;
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_INTERRUPT_COMPB_SETFUNC(void (*funcptr)(void))
{
    __TIMER0_DRIVER_INTERRUPT_COMPB_FUNC = funcptr;
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_INTERRUPT_OVF_ENABLE(void)
{
    __TIMER0_DRIVER_TIMSK |= (1 << __TIMER0_DRIVER_TOIE);
    __TIMER0_DRIVER_SREG_SET();
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_INTERRUPT_OVF_DISABLE(void)
{
    __TIMER0_DRIVER_TIMSK &= ~(1 << __TIMER0_DRIVER_TOIE);
}

__attribute__((always_inline))
static inline void __TIMER0_DRIVER_INTERRUPT_OVF_SETFUNC(void (*funcptr)(void))
{
    __TIMER0_DRIVER_INTERRUPT_OVF_FUNC = funcptr;
}

#endif
