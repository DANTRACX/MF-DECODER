/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           DRIVER
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        TIMER2_DRIVER.h
 *
 * DESCRIPTION:
 *      THIS MODULE REPRESENTS A DRIVER FOR THE 8-BIT TIMER2 HARDWARE
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#ifndef __TIMER2_DRIVER_H__
#define __TIMER2_DRIVER_H__

/*
 * TIMER/COUNTER CONTROL REGISTER A
 */
#define __TIMER2_DRIVER_TCCRA   (*(volatile unsigned char *)(0xB0))
#define __TIMER2_DRIVER_WGM0    0
#define __TIMER2_DRIVER_WGM1    1
#define __TIMER2_DRIVER_COMB0   4
#define __TIMER2_DRIVER_COMB1   5
#define __TIMER2_DRIVER_COMA0   6
#define __TIMER2_DRIVER_COMA1   7

/*
 * TIMER/COUNTER CONTROL REGISTER B
 */
#define __TIMER2_DRIVER_TCCRB   (*(volatile unsigned char *)(0xB1))
#define __TIMER2_DRIVER_CS0     0
#define __TIMER2_DRIVER_CS1     1
#define __TIMER2_DRIVER_CS2     2
#define __TIMER2_DRIVER_WGM2    3
#define __TIMER2_DRIVER_FOCB    6
#define __TIMER2_DRIVER_FOCA    7

/*
 * TIMER/COUNTER REGISTER
 */
#define __TIMER2_DRIVER_TCNT    (*(volatile unsigned char *)(0xB2))

/*
 * TIMER/COUNTER OUTPUT COMPARE REGISTER A
 */
#define __TIMER2_DRIVER_OCRA    (*(volatile unsigned char *)(0xB3))

/*
 * TIMER/COUNTER OUTPUT COMPARE REGISTER B
 */
#define __TIMER2_DRIVER_OCRB    (*(volatile unsigned char *)(0xB4))

/*
 * TIMER/COUNTER INTERRUPT MASK REGISTER
 */
#define __TIMER2_DRIVER_TIMSK   (*(volatile unsigned char *)(0x70))
#define __TIMER2_DRIVER_TOIE    0
#define __TIMER2_DRIVER_OCIEA   1
#define __TIMER2_DRIVER_OCIEB   2

/*
 * TIMER/COUNTER INTERRUPT FLAG REGISTER
 */
#define __TIMER2_DRIVER_TIFR    (*(volatile unsigned char *)(0x37))
#define __TIMER2_DRIVER_TOV     0
#define __TIMER2_DRIVER_OCFA    1
#define __TIMER2_DRIVER_OCFB    2

/*
 * TIMER/COUNTER ASYNCHRONOUS STATUS REGISTER
 */
#define __TIMER2_DRIVER_ASSR    (*(volatile unsigned char *)(0xB6))
#define __TIMER2_DRIVER_TCRBUB  0
#define __TIMER2_DRIVER_TCRAUB  1
#define __TIMER2_DRIVER_OCRBUB  2
#define __TIMER2_DRIVER_OCRAUB  3
#define __TIMER2_DRIVER_TCNUB   4
#define __TIMER2_DRIVER_AS      5
#define __TIMER2_DRIVER_EXCLK   6

/*
 * TIMER/COUNTER GENERAL CONTROL REGISTER
 */
#define __TIMER2_DRIVER_GTCCR   (*(volatile unsigned char *)(0x43))
#define __TIMER2_DRIVER_PSRASY  1

/*
 * TIMER/COUNTER INTERRUPT SREG
 */
#define __TIMER2_DRIVER_SREG_SET()      __asm__ __volatile__ ("sei" ::: "memory")
#define __TIMER2_DRIVER_SREG_CLEAR()    __asm__ __volatile__ ("cli" ::: "memory")

/*
 * TIMER/COUNTER INTERRUPT VECTORS
 */
#define __TIMER2_DRIVER_COMPA_VECTOR    __vector_7
#define __TIMER2_DRIVER_COMPB_VECTOR    __vector_8
#define __TIMER2_DRIVER_OVF_VECTOR      __vector_9

/*
 * TIMER/COUNTER MODE OF OPERATION
 */
__attribute__((always_inline))
static inline void __TIMER2_DRIVER_MODE_NORMAL(void)
{
    __TIMER2_DRIVER_TCCRA &= ~(1 << __TIMER2_DRIVER_WGM0);
    __TIMER2_DRIVER_TCCRA &= ~(1 << __TIMER2_DRIVER_WGM1);
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_WGM2);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_MODE_CTC(void)
{
    __TIMER2_DRIVER_TCCRA &= ~(1 << __TIMER2_DRIVER_WGM0);
    __TIMER2_DRIVER_TCCRA |=  (1 << __TIMER2_DRIVER_WGM1);
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_WGM2);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_MODE_FASTPWM(void)
{
    __TIMER2_DRIVER_TCCRA |=  (1 << __TIMER2_DRIVER_WGM0);
    __TIMER2_DRIVER_TCCRA |=  (1 << __TIMER2_DRIVER_WGM1);
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_WGM2);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_MODE_FASTPWM_SCALED(unsigned char topValue)
{
    __TIMER2_DRIVER_TCCRA |=  (1 << __TIMER2_DRIVER_WGM0);
    __TIMER2_DRIVER_TCCRA |=  (1 << __TIMER2_DRIVER_WGM1);
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_WGM2);
    __TIMER2_DRIVER_OCRA = (unsigned char)topValue;
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_MODE_PCPWM(void)
{
    __TIMER2_DRIVER_TCCRA |=  (1 << __TIMER2_DRIVER_WGM0);
    __TIMER2_DRIVER_TCCRA &= ~(1 << __TIMER2_DRIVER_WGM1);
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_WGM2);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_MODE_PCPWM_SCALED(unsigned char topValue)
{
    __TIMER2_DRIVER_TCCRA |=  (1 << __TIMER2_DRIVER_WGM0);
    __TIMER2_DRIVER_TCCRA &= ~(1 << __TIMER2_DRIVER_WGM1);
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_WGM2);
    __TIMER2_DRIVER_OCRA = (unsigned char)topValue;
}

/*
 * TIMER/COUNTER CLOCK PRESCALER SETUP
 */
__attribute__((always_inline))
static inline void __TIMER2_DRIVER_CLOCK_OFF(void)
{
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_CS0);
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_CS1);
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_CLOCK_DIV1(void)
{
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_CS0);
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_CS1);
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_CLOCK_DIV8(void)
{
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_CS0);
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_CS1);
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_CLOCK_DIV32(void)
{
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_CS0);
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_CS1);
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_CLOCK_DIV64(void)
{
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_CS0);
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_CS1);
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_CLOCK_DIV128(void)
{
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_CS0);
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_CS1);
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_CLOCK_DIV256(void)
{
    __TIMER2_DRIVER_TCCRB &= ~(1 << __TIMER2_DRIVER_CS0);
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_CS1);
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_CLOCK_DIV1024(void)
{
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_CS0);
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_CS1);
    __TIMER2_DRIVER_TCCRB |=  (1 << __TIMER2_DRIVER_CS2);
}

/*
 * TIMER/COUNTER INTERRUPTS
 */
extern void (*__TIMER2_DRIVER_INTERRUPT_COMPA_FUNC)(void);
extern void (*__TIMER2_DRIVER_INTERRUPT_COMPB_FUNC)(void);
extern void (*__TIMER2_DRIVER_INTERRUPT_OVF_FUNC)(void);

extern void __TIMER2_DRIVER_COMPA_VECTOR (void) __attribute__((signal, used, externally_visible));
extern void __TIMER2_DRIVER_COMPB_VECTOR (void) __attribute__((signal, used, externally_visible));
extern void __TIMER2_DRIVER_OVF_VECTOR (void) __attribute__((signal, used, externally_visible));
extern void __TIMER2_DRIVER_VEC_DUMMY(void);

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_INTERRUPT_COMPA_ENABLE(unsigned char matchValue)
{
    __TIMER2_DRIVER_OCRA = (unsigned char)matchValue;
    __TIMER2_DRIVER_TIMSK |= (1 << __TIMER2_DRIVER_OCIEA);
    __TIMER2_DRIVER_SREG_SET();
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_INTERRUPT_COMPA_DISABLE(void)
{
    __TIMER2_DRIVER_TIMSK &= ~(1 << __TIMER2_DRIVER_OCIEA);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_INTERRUPT_COMPA_UPDATE(unsigned char matchValue)
{
    __TIMER2_DRIVER_OCRA = (unsigned char)matchValue;
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_INTERRUPT_COMPA_SETFUNC(void (*funcptr)(void))
{
    __TIMER2_DRIVER_INTERRUPT_COMPA_FUNC = funcptr;
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_INTERRUPT_COMPB_ENABLE(unsigned char matchValue)
{
    __TIMER2_DRIVER_OCRB = (unsigned char)matchValue;
    __TIMER2_DRIVER_TIMSK |= (1 << __TIMER2_DRIVER_OCIEB);
    __TIMER2_DRIVER_SREG_SET();
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_INTERRUPT_COMPB_DISABLE(void)
{
    __TIMER2_DRIVER_TIMSK &= ~(1 << __TIMER2_DRIVER_OCIEB);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_INTERRUPT_COMPB_UPDATE(unsigned char matchValue)
{
    __TIMER2_DRIVER_OCRB = (unsigned char)matchValue;
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_INTERRUPT_COMPB_SETFUNC(void (*funcptr)(void))
{
    __TIMER2_DRIVER_INTERRUPT_COMPB_FUNC = funcptr;
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_INTERRUPT_OVF_ENABLE(void)
{
    __TIMER2_DRIVER_TIMSK |= (1 << __TIMER2_DRIVER_TOIE);
    __TIMER2_DRIVER_SREG_SET();
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_INTERRUPT_OVF_DISABLE(void)
{
    __TIMER2_DRIVER_TIMSK &= ~(1 << __TIMER2_DRIVER_TOIE);
}

__attribute__((always_inline))
static inline void __TIMER2_DRIVER_INTERRUPT_OVF_SETFUNC(void (*funcptr)(void))
{
    __TIMER2_DRIVER_INTERRUPT_OVF_FUNC = funcptr;
}

#endif
