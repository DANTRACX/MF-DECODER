/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           DRIVER
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        IO_DRIVER.h
 *
 * DESCRIPTION:
 *      THIS MODULE REPRESENTS A DRIVER FOR I/O FUNCTIONALITY
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#ifndef __IO_DRIVER_H__
#define __IO_DRIVER_H__

/*
 * INPUT/OUTPUT REGISTER B
 */
#define __IO_DRIVER_DDRB        (*(volatile unsigned char *)(0x24))
#define __IO_DRIVER_PINB        (*(volatile unsigned char *)(0x23))
#define __IO_DRIVER_PORTB       (*(volatile unsigned char *)(0x25))

/*
 * INPUT/OUTPUT REGISTER C
 */
#define __IO_DRIVER_DDRC        (*(volatile unsigned char *)(0x27))
#define __IO_DRIVER_PINC        (*(volatile unsigned char *)(0x26))
#define __IO_DRIVER_PORTC       (*(volatile unsigned char *)(0x28))

/*
 * INPUT/OUTPUT REGISTER D
 */
#define __IO_DRIVER_DDRD        (*(volatile unsigned char *)(0x2A))
#define __IO_DRIVER_PIND        (*(volatile unsigned char *)(0x29))
#define __IO_DRIVER_PORTD       (*(volatile unsigned char *)(0x2B))


/*
 * INPUT/OUTPUT staticAL INTERRUPT CONTROL REGISTER
 */
#define __IO_DRIVER_EICRA       (*(volatile unsigned char *)(0x69))
#define __IO_DRIVER_ISC00       0
#define __IO_DRIVER_ISC01       1
#define __IO_DRIVER_ISC10       2
#define __IO_DRIVER_ISC11       3

/*
 * INPUT/OUTPUT staticAL INTERRUPT MASK REGISTER
 */
#define __IO_DRIVER_EIMSK       (*(volatile unsigned char *)(0x3D))
#define __IO_DRIVER_INT0        0
#define __IO_DRIVER_INT1        1

/*
 * INPUT/OUTPUT staticAL INTERRUPT FLAG REGISTER
 */
#define __IO_DRIVER_EIFR        (*(volatile unsigned char *)(0x3C))
#define __IO_DRIVER_INTF0       0
#define __IO_DRIVER_INTF1       1

/*
 * INPUT/OUTPUT PIN CHANGE INTERRUPT CONTROL REGISTER
 */
#define __IO_DRIVER_PCICR       (*(volatile unsigned char *)(0x68))
#define __IO_DRIVER_PCIE0       0
#define __IO_DRIVER_PCIE1       1
#define __IO_DRIVER_PCIE2       2

/*
 * INPUT/OUTPUT PIN CHANGE INTERRUPT FLAG REGISTER
 */
#define __IO_DRIVER_PCIFR       (*(volatile unsigned char *)(0x3B))
#define __IO_DRIVER_PCIF0       0
#define __IO_DRIVER_PCIF1       1
#define __IO_DRIVER_PCIF2       2

/*
 * INPUT/OUTPUT PIN CHANGE MASK REGISTER 2
 */
#define __IO_DRIVER_PCMSK2      (*(volatile unsigned char *)(0x6D))
#define __IO_DRIVER_PCINT16     0
#define __IO_DRIVER_PCINT17     1
#define __IO_DRIVER_PCINT18     2
#define __IO_DRIVER_PCINT19     3
#define __IO_DRIVER_PCINT20     4
#define __IO_DRIVER_PCINT21     5
#define __IO_DRIVER_PCINT22     6
#define __IO_DRIVER_PCINT23     7

/*
 * INPUT/OUTPUT PIN CHANGE MASK REGISTER 1
 */
#define __IO_DRIVER_PCMSK1      (*(volatile unsigned char *)(0x6C))
#define __IO_DRIVER_PCINT8      0
#define __IO_DRIVER_PCINT9      1
#define __IO_DRIVER_PCINT10     2
#define __IO_DRIVER_PCINT11     3
#define __IO_DRIVER_PCINT12     4
#define __IO_DRIVER_PCINT13     5
#define __IO_DRIVER_PCINT14     6

/*
 * INPUT/OUTPUT PIN CHANGE MASK REGISTER 0
 */
#define __IO_DRIVER_PCMSK0      (*(volatile unsigned char *)(0x6B))
#define __IO_DRIVER_PCINT0      0
#define __IO_DRIVER_PCINT1      1
#define __IO_DRIVER_PCINT2      2
#define __IO_DRIVER_PCINT3      3
#define __IO_DRIVER_PCINT4      4
#define __IO_DRIVER_PCINT5      5
#define __IO_DRIVER_PCINT6      6
#define __IO_DRIVER_PCINT7      7

/*
 * INPUT/OUTPUT INTERRUPT SREG
 */
#define __IO_DRIVER_SREG_SET()      __asm__ __volatile__ ("sei" ::: "memory")
#define __IO_DRIVER_SREG_CLEAR()    __asm__ __volatile__ ("cli" ::: "memory")

/*
 * TIMER/COUNTER INTERRUPT VECTORS
 */
#define __IO_DRIVER_INT0_VECTOR     __vector_1
#define __IO_DRIVER_INT1_VECTOR     __vector_2
#define __IO_DRIVER_PCINT0_VECTOR   __vector_3
#define __IO_DRIVER_PCINT1_VECTOR   __vector_4
#define __IO_DRIVER_PCINT2_VECTOR   __vector_5

/*
 * INPUT/OUTPUT B SETTINGS
 */
 __attribute__((always_inline))
static inline unsigned char __IO_DRIVER_IO_B_GET(unsigned char bitmask)
{
    return __IO_DRIVER_PINB & bitmask;
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_B_SET_IN(unsigned char bitmask)
{
    __IO_DRIVER_DDRB &= ~(bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_B_SET_IN_PULLUP(unsigned char bitmask)
{
    __IO_DRIVER_DDRB &= ~(bitmask);
    __IO_DRIVER_PORTB |= (bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_B_SET_OUT(unsigned char bitmask)
{
    __IO_DRIVER_DDRB |= (bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_B_SET_OUT_H(unsigned char bitmask)
{
    __IO_DRIVER_PORTB |= (bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_B_SET_OUT_L(unsigned char bitmask)
{
    __IO_DRIVER_PORTB &= ~(bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_B_SET_OUT_T(unsigned char bitmask)
{
    __IO_DRIVER_PORTB ^= (bitmask);
}

/*
 * INPUT/OUTPUT C SETTINGS
 */
 __attribute__((always_inline))
static inline unsigned char __IO_DRIVER_IO_C_GET(unsigned char bitmask)
{
    return __IO_DRIVER_PINC & bitmask;
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_C_SET_IN(unsigned char bitmask)
{
    __IO_DRIVER_DDRC &= ~(bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_C_SET_IN_PULLUP(unsigned char bitmask)
{
    __IO_DRIVER_DDRC &= ~(bitmask);
    __IO_DRIVER_PORTC |= (bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_C_SET_OUT(unsigned char bitmask)
{
    __IO_DRIVER_DDRC |= (bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_C_SET_OUT_H(unsigned char bitmask)
{
    __IO_DRIVER_PORTC |= (bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_C_SET_OUT_L(unsigned char bitmask)
{
    __IO_DRIVER_PORTC &= ~(bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_C_SET_OUT_T(unsigned char bitmask)
{
    __IO_DRIVER_PORTC ^= (bitmask);
}

/*
 * INPUT/OUTPUT D SETTINGS
 */
 __attribute__((always_inline))
static inline unsigned char __IO_DRIVER_IO_D_GET(unsigned char bitmask)
{
    return __IO_DRIVER_PIND & bitmask;
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_D_SET_IN(unsigned char bitmask)
{
    __IO_DRIVER_DDRD &= ~(bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_D_SET_IN_PULLUP(unsigned char bitmask)
{
    __IO_DRIVER_DDRD &= ~(bitmask);
    __IO_DRIVER_PORTD |= (bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_D_SET_OUT(unsigned char bitmask)
{
    __IO_DRIVER_DDRD |= (bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_D_SET_OUT_H(unsigned char bitmask)
{
    __IO_DRIVER_PORTD |= (bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_D_SET_OUT_L(unsigned char bitmask)
{
    __IO_DRIVER_PORTD &= ~(bitmask);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_IO_D_SET_OUT_T(unsigned char bitmask)
{
    __IO_DRIVER_PORTD ^= (bitmask);
}

/*
 * INPUT/OUTPUT EXTERNAL INTERRUPTS
 */
extern void (*__IO_DRIVER_INTERRUPT_INT0_FUNC)(void);
extern void (*__IO_DRIVER_INTERRUPT_INT1_FUNC)(void);

extern void __IO_DRIVER_INT0_VECTOR (void) __attribute__((signal, used, externally_visible));
extern void __IO_DRIVER_INT1_VECTOR (void) __attribute__((signal, used, externally_visible));
extern void __IO_DRIVER_VEC_DUMMY(void);

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT0_SENSE_LOW(void)
{
    __IO_DRIVER_EICRA &= ~(1 << __IO_DRIVER_ISC00);
    __IO_DRIVER_EICRA &= ~(1 << __IO_DRIVER_ISC01);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT0_SENSE_CHANGE(void)
{
    __IO_DRIVER_EICRA |=  (1 << __IO_DRIVER_ISC00);
    __IO_DRIVER_EICRA &= ~(1 << __IO_DRIVER_ISC01);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT0_SENSE_FALLING(void)
{
    __IO_DRIVER_EICRA &= ~(1 << __IO_DRIVER_ISC00);
    __IO_DRIVER_EICRA |=  (1 << __IO_DRIVER_ISC01);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT0_SENSE_RISING(void)
{
    __IO_DRIVER_EICRA |=  (1 << __IO_DRIVER_ISC00);
    __IO_DRIVER_EICRA |=  (1 << __IO_DRIVER_ISC01);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT0_ENABLE(void)
{
    __IO_DRIVER_DDRD &= ~(1 << 2);
    __IO_DRIVER_PORTD &= ~(1 << 2);
    __IO_DRIVER_EIMSK|= (1 << __IO_DRIVER_INT0);
    __IO_DRIVER_SREG_SET();
}

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT0_DISABLE(void)
{
    __IO_DRIVER_EIMSK &= ~(1 << __IO_DRIVER_INT0);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT0_SETFUNC(void (*funcptr)(void))
{
    __IO_DRIVER_INTERRUPT_INT0_FUNC = funcptr;
}

__attribute__((always_inline))
static inline unsigned char __IO_DRIVER_INTERRUPT_INT0_LEVEL(void)
{
    return __IO_DRIVER_PIND & 0x04;
}

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT1_SENSE_LOW(void)
{
    __IO_DRIVER_EICRA &= ~(1 << __IO_DRIVER_ISC10);
    __IO_DRIVER_EICRA &= ~(1 << __IO_DRIVER_ISC11);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT1_SENSE_CHANGE(void)
{
    __IO_DRIVER_EICRA |=  (1 << __IO_DRIVER_ISC10);
    __IO_DRIVER_EICRA &= ~(1 << __IO_DRIVER_ISC11);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT1_SENSE_FALLING(void)
{
    __IO_DRIVER_EICRA &= ~(1 << __IO_DRIVER_ISC10);
    __IO_DRIVER_EICRA |=  (1 << __IO_DRIVER_ISC11);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT1_SENSE_RISING(void)
{
    __IO_DRIVER_EICRA |=  (1 << __IO_DRIVER_ISC10);
    __IO_DRIVER_EICRA |=  (1 << __IO_DRIVER_ISC11);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT1_ENABLE(void)
{
    __IO_DRIVER_DDRD &= ~(1 << 3);
    __IO_DRIVER_PORTD &= ~(1 << 3);
    __IO_DRIVER_EIMSK|= (1 << __IO_DRIVER_INT1);
    __IO_DRIVER_SREG_SET();
}

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT1_DISABLE(void)
{
    __IO_DRIVER_EIMSK &= ~(1 << __IO_DRIVER_INT1);
}

__attribute__((always_inline))
static inline void __IO_DRIVER_INTERRUPT_INT1_SETFUNC(void (*funcptr)(void))
{
    __IO_DRIVER_INTERRUPT_INT1_FUNC = funcptr;
}

__attribute__((always_inline))
static inline unsigned char __IO_DRIVER_INTERRUPT_INT1_LEVEL(void)
{
    return __IO_DRIVER_PIND & 0x08;
}

#endif
