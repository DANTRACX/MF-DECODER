/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           DRIVER
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        TIMER1_DRIVER.h
 *
 * DESCRIPTION:
 *      THIS MODULE REPRESENTS A DRIVER FOR THE 16-BIT TIMER1 HARDWARE
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#ifndef __TIMER1_DRIVER_H__
#define __TIMER1_DRIVER_H__

/*
 * TIMER/COUNTER CONTROL REGISTER A
 */
#define __TIMER1_DRIVER_TCCRA   (*(volatile unsigned char *)(0x80))
#define __TIMER1_DRIVER_WGM0    0
#define __TIMER1_DRIVER_WGM1    1
#define __TIMER1_DRIVER_COMB0   4
#define __TIMER1_DRIVER_COMB1   5
#define __TIMER1_DRIVER_COMA0   6
#define __TIMER1_DRIVER_COMA1   7

/*
 * TIMER/COUNTER CONTROL REGISTER B
 */
#define __TIMER1_DRIVER_TCCRB   (*(volatile unsigned char *)(0x81))
#define __TIMER1_DRIVER_CS0     0
#define __TIMER1_DRIVER_CS1     1
#define __TIMER1_DRIVER_CS2     2
#define __TIMER1_DRIVER_WGM2    3
#define __TIMER1_DRIVER_WGM3    4
#define __TIMER1_DRIVER_ICES    6
#define __TIMER1_DRIVER_ICNC    7

/*
 * TIMER/COUNTER CONTROL REGISTER C
 */
#define __TIMER1_DRIVER_TCCRC   (*(volatile unsigned char *)(0x82))
#define __TIMER1_DRIVER_FOCB     6
#define __TIMER1_DRIVER_FOCA     7

/*
 * TIMER/COUNTER REGISTER
 */
#define __TIMER1_DRIVER_TCNT    (*(volatile unsigned short *)(0x84))

/*
 * TIMER/COUNTER OUTPUT COMPARE REGISTER A
 */
#define __TIMER1_DRIVER_OCRA    (*(volatile unsigned short *)(0x88))

/*
 * TIMER/COUNTER OUTPUT COMPARE REGISTER B
 */
#define __TIMER1_DRIVER_OCRB    (*(volatile unsigned short *)(0x8A))

/*
 * TIMER/COUNTER INPUT CAPTURE REGISTER
 */
#define __TIMER1_DRIVER_ICR     (*(volatile unsigned short *)(0x86))

/*
 * TIMER/COUNTER INTERRUPT MASK REGISTER
 */
#define __TIMER1_DRIVER_TIMSK   (*(volatile unsigned char *)(0x6F))
#define __TIMER1_DRIVER_TOIE    0
#define __TIMER1_DRIVER_OCIEA   1
#define __TIMER1_DRIVER_OCIEB   2
#define __TIMER1_DRIVER_ICIE    5

/*
 * TIMER/COUNTER INTERRUPT FLAG REGISTER
 */
#define __TIMER1_DRIVER_TIFR    (*(volatile unsigned char *)(0x36))
#define __TIMER1_DRIVER_TOV     0
#define __TIMER1_DRIVER_OCFA    1
#define __TIMER1_DRIVER_OCFB    2
#define __TIMER1_DRIVER_ICF     5

/*
 * TIMER/COUNTER INTERRUPT SREG
 */
#define __TIMER1_DRIVER_SREG_SET()      __asm__ __volatile__ ("sei" ::: "memory")
#define __TIMER1_DRIVER_SREG_CLEAR()    __asm__ __volatile__ ("cli" ::: "memory")

/*
 * TIMER/COUNTER INTERRUPT VECTORS
 */
#define __TIMER1_DRIVER_COMPA_VECTOR    __vector_11
#define __TIMER1_DRIVER_COMPB_VECTOR    __vector_12
#define __TIMER1_DRIVER_OVF_VECTOR      __vector_13
#define __TIMER1_DRIVER_IC_VECTOR       __vector_10

/*
 * TIMER/COUNTER MODE OF OPERATION
 */
__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_NORMAL(void)
{
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM3);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_PCPWM8(void)
{
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM3);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_PCPWM9(void)
{
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM3);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_PCPWM10(void)
{
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM3);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_CTC(void)
{
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM3);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_FASTPWM8(void)
{
    __TIMER1_DRIVER_TCCRA |= (1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB |= (1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM3);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_FASTPWM9(void)
{
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM3);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_FASTPWM10(void)
{
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM3);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_PFCPWM_ICR(unsigned short matchValue)
{
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_WGM3);
    __TIMER1_DRIVER_ICR = matchValue;
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_PFCPWM_OCR(unsigned short matchValue)
{
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_WGM3);
    __TIMER1_DRIVER_OCRA = matchValue;
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_PCPWM_ICR(unsigned short matchValue)
{
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_WGM3);
    __TIMER1_DRIVER_ICR = matchValue;
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_PCPWM_OCR(unsigned short matchValue)
{
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_WGM3);
    __TIMER1_DRIVER_OCRA = matchValue;
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_CTC_ICR(unsigned short matchValue)
{
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_WGM3);
    __TIMER1_DRIVER_ICR = matchValue;
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_FASTPWM_ICR(unsigned short matchValue)
{
    __TIMER1_DRIVER_TCCRA &= ~(1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_WGM3);
    __TIMER1_DRIVER_ICR = matchValue;
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_MODE_FASTPWM_OCR(unsigned short matchValue)
{
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM0);
    __TIMER1_DRIVER_TCCRA |=  (1 << __TIMER1_DRIVER_WGM1);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_WGM2);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_WGM3);
    __TIMER1_DRIVER_OCRA = matchValue;
}

/*
 * TIMER/COUNTER CLOCK PRESCALER SETUP
 */
__attribute__((always_inline))
static inline void __TIMER1_DRIVER_CLOCK_OFF(void)
{
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_CS0);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_CS1);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_CLOCK_DIV1(void)
{
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_CS0);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_CS1);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_CLOCK_DIV8(void)
{
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_CS0);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_CS1);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_CLOCK_DIV64(void)
{
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_CS0);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_CS1);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_CLOCK_DIV256(void)
{
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_CS0);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_CS1);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_CLOCK_DIV1024(void)
{
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_CS0);
    __TIMER1_DRIVER_TCCRB &= ~(1 << __TIMER1_DRIVER_CS1);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_CLOCK_T0FALLING(void)
{
    __TIMER1_DRIVER_TCCRB &= (1 << __TIMER1_DRIVER_CS0);
    __TIMER1_DRIVER_TCCRB |= (1 << __TIMER1_DRIVER_CS1);
    __TIMER1_DRIVER_TCCRB |= (1 << __TIMER1_DRIVER_CS2);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_CLOCK_T0RISING(void)
{
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_CS0);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_CS1);
    __TIMER1_DRIVER_TCCRB |=  (1 << __TIMER1_DRIVER_CS2);
}

/*
 * TIMER/COUNTER INTERRUPTS
 */
extern void (*__TIMER1_DRIVER_INTERRUPT_COMPA_FUNC)(void);
extern void (*__TIMER1_DRIVER_INTERRUPT_COMPB_FUNC)(void);
extern void (*__TIMER1_DRIVER_INTERRUPT_OVF_FUNC)(void);
extern void (*__TIMER1_DRIVER_INTERRUPT_IC_FUNC)(void);

extern void __TIMER1_DRIVER_COMPA_VECTOR(void) __attribute__((signal, used, externally_visible));
extern void __TIMER1_DRIVER_COMPB_VECTOR(void) __attribute__((signal, used, externally_visible));
extern void __TIMER1_DRIVER_OVF_VECTOR(void) __attribute__((signal, used, externally_visible));
extern void __TIMER1_DRIVER_IC_VECTOR(void) __attribute__((signal, used, externally_visible));
extern void __TIMER1_DRIVER_VEC_DUMMY(void);


__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_COMPA_ENABLE(unsigned short matchValue)
{
    __TIMER1_DRIVER_OCRA = (unsigned short)matchValue;
    __TIMER1_DRIVER_TIMSK |= (1 << __TIMER1_DRIVER_OCIEA);
    __TIMER1_DRIVER_SREG_SET();
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_COMPA_DISABLE(void)
{
    __TIMER1_DRIVER_TIMSK &= ~(1 << __TIMER1_DRIVER_OCIEA);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_COMPA_UPDATE(unsigned short matchValue)
{
    __TIMER1_DRIVER_OCRA = (unsigned short)matchValue;
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_COMPA_SETFUNC(void (*funcptr)(void))
{
    __TIMER1_DRIVER_INTERRUPT_COMPA_FUNC = funcptr;
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_COMPB_ENABLE(unsigned short matchValue)
{
    __TIMER1_DRIVER_OCRB = (unsigned short)matchValue;
    __TIMER1_DRIVER_TIMSK |= (1 << __TIMER1_DRIVER_OCIEB);
    __TIMER1_DRIVER_SREG_SET();
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_COMPB_DISABLE(void)
{
    __TIMER1_DRIVER_TIMSK &= ~(1 << __TIMER1_DRIVER_OCIEB);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_COMPB_UPDATE(unsigned short matchValue)
{
    __TIMER1_DRIVER_OCRB = (unsigned short)matchValue;
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_COMPB_SETFUNC(void (*funcptr)(void))
{
    __TIMER1_DRIVER_INTERRUPT_COMPB_FUNC = funcptr;
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_OVF_ENABLE(void)
{
    __TIMER1_DRIVER_TIMSK |= (1 << __TIMER1_DRIVER_TOIE);
    __TIMER1_DRIVER_SREG_SET();
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_OVF_DISABLE(void)
{
    __TIMER1_DRIVER_TIMSK &= ~(1 << __TIMER1_DRIVER_TOIE);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_OVF_SETFUNC(void (*funcptr)(void))
{
    __TIMER1_DRIVER_INTERRUPT_OVF_FUNC = funcptr;
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_IC_ENABLE(unsigned short matchValue)
{
    __TIMER1_DRIVER_ICR = (unsigned short)matchValue;
    __TIMER1_DRIVER_TIMSK |= (1 << __TIMER1_DRIVER_ICIE);
    __TIMER1_DRIVER_SREG_SET();
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_IC_DISABLE(void)
{
    __TIMER1_DRIVER_TIMSK &= ~(1 << __TIMER1_DRIVER_ICIE);
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_IC_UPDATE(unsigned short matchValue)
{
    __TIMER1_DRIVER_ICR = (unsigned short)matchValue;
}

__attribute__((always_inline))
static inline void __TIMER1_DRIVER_INTERRUPT_IC_SETFUNC(void (*funcptr)(void))
{
    __TIMER1_DRIVER_INTERRUPT_IC_FUNC = funcptr;
}

#endif
