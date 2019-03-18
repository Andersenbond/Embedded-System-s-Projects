#ifndef __IMX6_UART_H__
#define __IMX6_UART_H__
#include <types.h>

/* Base address for UART instance number 1. */
#define REGS_UART1_BASE 	(0x02020000)
/* UART Receiver Register (RO) base address */
#define HW_UART_URXD_ADDR       (REGS_UART1_BASE + 0x00)
/* UART Transmitter Register (WO) base address */
#define HW_UART_UTXD_ADDR       (REGS_UART1_BASE + 0x40)
/* UART Control Register 1 base address */
#define HW_UART_UCR1_ADDR       (REGS_UART1_BASE + 0x80)
/* UART Control Register 2 base address */
#define HW_UART_UCR2_ADDR       (REGS_UART1_BASE + 0x84)
/* UART Control Register 3 base address */
#define HW_UART_UCR3_ADDR       (REGS_UART1_BASE + 0x88)
/* UART FIFO Control Register (RW) base address */
#define HW_UART_UFCR_ADDR       (REGS_UART1_BASE + 0x90)
/* UART BRM Incremental Register (RW) base address */
#define HW_UART_UBIR_ADDR       (REGS_UART1_BASE + 0xa4)
/*UART BRM Modulator Register (RW) base address */
#define HW_UART_UBMR_ADDR       (REGS_UART1_BASE + 0xa8)
/* UART One Millisecond Register (RW) base address */
#define HW_UART_ONEMS_ADDR      (REGS_UART1_BASE + 0xb0)
/* HW_UART_UTS - UART Test Register (RW) base address */
#define HW_UART_UTS_ADDR     	(REGS_UART1_BASE + 0xb4)

/* Pointers to the registers addresses */
#define HW_UART_URXD            (*(volatile uint32_t*) HW_UART_URXD_ADDR )
#define HW_UART_UTXD            (*(volatile uint32_t*) HW_UART_UTXD_ADDR )
#define HW_UART_UCR1            (*(volatile uint32_t*) HW_UART_UCR1_ADDR )
#define HW_UART_UCR2	          (*(volatile uint32_t*) HW_UART_UCR2_ADDR )
#define HW_UART_UCR3            (*(volatile uint32_t*) HW_UART_UCR3_ADDR )
#define HW_UART_UFCR            (*(volatile uint32_t*) HW_UART_UFCR_ADDR )
#define HW_UART_UBIR            (*(volatile uint32_t*) HW_UART_UBIR_ADDR )
#define HW_UART_UBMR            (*(volatile uint32_t*) HW_UART_UBMR_ADDR )
#define HW_UART_ONEMS           (*(volatile uint32_t*) HW_UART_ONEMS_ADDR)
#define HW_UART_UTS             (*(volatile uint32_t*) HW_UART_UTS_ADDR  )

#define EMPTY   		(0xFF)
#define ERROR_DATA  		(0xFF)

/* For printk */
#define serial_puts(str_buffer) ltz_uart_puts(str_buffer)

/**
 * Initialize the Uart Driver
 * @param	baudrate = Desired UART Baudrate
 * @retval
 */
void ltz_uart_init(uint32_t baudrate);

/**
 * Get a Character from the UART RX fifo buffer
 * @param
 * @retval  first charater on the RX Buffer
 */
uint8_t ltz_uart_getchar(void);

/**
 * Write a character to the UART TX buffer
 * @param	c = character to be written to the TX Fifo Buffer
 * @retval   	character written in the TX fifo Buffer
 */
uint8_t ltz_uart_putchar(uint8_t  c);

/**
 * Write an array of character to the UART TX buffer
 * @param	buffer = array of character to be written to the TX Fifo Buffer
 * @retval   	number of character written in the TX fifo Buffer
 */
uint8_t ltz_uart_puts(uint8_t *buffer);
#endif
