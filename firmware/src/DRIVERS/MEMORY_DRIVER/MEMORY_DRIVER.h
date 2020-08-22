/*======================================================================*
 * PROJECT:         MF-DECODER
 * MODUL:           DRIVER
 * DEVELOPED BY:    Christoph Klie
 * FILENAME:        MEMORY_DRIVER.h
 *
 * DESCRIPTION:
 *      THIS MODULE REPRESENTS A DRIVER FOR MEMORY FUNCTIONALITY
 *      ESPECIALLY EEPROM AND FAST MEMORY REGISTERS
 *
 * (C) COPYRIGHT 2015 - CHRISTOPH KLIE
 *======================================================================*/

#ifndef __MEMORY_DRIVER_H__
#define __MEMORY_DRIVER_H__

/*
 * GENERAL PURPOSE I/O REGISTER
 */
#define __MEMORY_DRIVER_IOMEM_R0        (*(volatile unsigned char *)(0x3E))
#define __MEMORY_DRIVER_IOMEM_R1        (*(volatile unsigned char *)(0x4A))
#define __MEMORY_DRIVER_IOMEM_R2        (*(volatile unsigned char *)(0x4B))

/*
 * EEPROM ADDRESS REGISTER
 */
#define __MEMORY_DRIVER_EEMEM_EEAR      (*(volatile unsigned short *)(0x41))

/*
 * EEPROM DATA REGISTER
 */
#define __MEMORY_DRIVER_EEMEM_EEDR      (*(volatile unsigned char *)(0x40))

/*
 * EEPROM CONTROL REGISTER
 */

#define __MEMORY_DRIVER_EEMEM_EECR      (*(volatile unsigned char *)(0x3F))
#define __MEMORY_DRIVER_EEMEM_EERE      0
#define __MEMORY_DRIVER_EEMEM_EEPE      1
#define __MEMORY_DRIVER_EEMEM_EEMPE     2
#define __MEMORY_DRIVER_EEMEM_EERIE     3
#define __MEMORY_DRIVER_EEMEM_EEPM0     4
#define __MEMORY_DRIVER_EEMEM_EEPM1     5

/*
 * EEPROM MEMORY SIZE
 */
#define __MEMORY_DRIVER_EEMEM_SIZE      1024

/*
 * MEMORY INTERRUPT SREG
 */
#define __MEMORY_DRIVER_SREG_SET()      __asm__ __volatile__ ("sei" ::: "memory")
#define __MEMORY_DRIVER_SREG_CLEAR()    __asm__ __volatile__ ("cli" ::: "memory")

/*
 * MEMORY EEPROM PROGRAMMING MODE
 */
__attribute__((always_inline))
static inline void __MEMORY_DRIVER_EEMEM_MODE_ATOMIC()
{
    __MEMORY_DRIVER_EEMEM_EECR &= ~(1 << __MEMORY_DRIVER_EEMEM_EEPM0);
    __MEMORY_DRIVER_EEMEM_EECR &= ~(1 << __MEMORY_DRIVER_EEMEM_EEPM1);
}

__attribute__((always_inline))
static inline void __MEMORY_DRIVER_EEMEM_MODE_ERASE()
{
    __MEMORY_DRIVER_EEMEM_EECR |=  (1 << __MEMORY_DRIVER_EEMEM_EEPM0);
    __MEMORY_DRIVER_EEMEM_EECR &= ~(1 << __MEMORY_DRIVER_EEMEM_EEPM1);
}

__attribute__((always_inline))
static inline void __MEMORY_DRIVER_EEMEM_MODE_WRITE()
{
    __MEMORY_DRIVER_EEMEM_EECR &= ~(1 << __MEMORY_DRIVER_EEMEM_EEPM0);
    __MEMORY_DRIVER_EEMEM_EECR |=  (1 << __MEMORY_DRIVER_EEMEM_EEPM1);
}

/*
 * MEMORY EEPROM WAIT/READ/WRITE FUNCTIONS
 */
extern void __MEMORY_DRIVER_EEMEM_WAIT();
extern void __MEMORY_DRIVER_EEMEM_WRITE(unsigned short EEMEM_ADDR, unsigned char EEMEM_DATA);
extern unsigned char __MEMORY_DRIVER_EEMEM_READ(unsigned short EEMEM_ADDR);

#endif
