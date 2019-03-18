#include <imx6_uart_driver.h>
#include <ltz_iomux_config.h>
#include <imx6_ccm.h>

void  ltz_uart_init(uint32_t baudrate){
	/* configure the I/O for the port */
	//ltz_uart1_iomux_config();

	/* enable the source clocks to the UART port */

    	//uint32_t cgx_offset = 0x00F000000; // bits [25:24] (uart_clk_enable) and bits[27:26] (uart_serial_clk_enable) on
	//*(volatile uint32_t *)(ccm_ccgrx) |= cgx_offset;
	HW_CCM_CCGR5 |= 0x00F000000;
    /* guarantee tha the TxFIFO is empty before changing the configuration */
	while(!(HW_UART_UTS & 0x40));

	/* Disable UART */
	HW_UART_UCR1 &=~(0x00000001);
	// DCE/DTE mode select . Select UART as data communication equipment (DCE mode) or as data terminal equipment (DTE mode).
	//HW_UART_UFCR |=0x00000040;
	HW_UART_UFCR |=0x00000020; // RX fifo Trigger Level -> 32 bytes
	HW_UART_UFCR |=0x00008000; // TX fifo Trigger Level -> 32 bytes
	HW_UART_UFCR |=0x00000200; // Divide input clock by -> 2

	/* Setup One Millisecond timer */
	// UART source clock is a fixed PLL3 / 6
	uint32_t uart_freq = 480000000 / 6 / ((HW_CCM_CSCDR1& 0x3F )  + 1); //0x3F -> first 6
	uint32_t div  = HW_UART_UFCR & 0x00000380;
	div  = div >> 7;
	/* Reference Frequency Divider Values:
	* - 000 - Divide input clock by 6
	* - 001 - Divide input clock by 5
	* - 010 - Divide input clock by 4
	* - 011 - Divide input clock by 3
	* - 100 - Divide input clock by 2
	* - 101 - Divide input clock by 1
	* - 110 - Divide input clock by 7
	* - 111 - Reserved
	*/
	switch(div){
		case (0x0): uart_freq = uart_freq/6; break;
		case (0x1): uart_freq = uart_freq/5; break;
		case (0x2): uart_freq = uart_freq/4; break;
		case (0x3): uart_freq = uart_freq/3; break;
		case (0x4): uart_freq = uart_freq/2; break;
		case (0x5): uart_freq = uart_freq/1; break;
		case (0x6): uart_freq = uart_freq/7; break;
		default: break;
	}
	HW_UART_ONEMS = uart_freq/1000;

	/* Set parity  -> no parity */
	// PROE - 0 - Even parity, PREN - 0  parity  generator and checker disable
	HW_UART_UCR2 &= (~(0x00000300));

	/* Set stop bit -> one stopbit */
	HW_UART_UCR2 &= (~(0x00000040));

	/* Set data size -> 8 bits */
	HW_UART_UCR2 |= 0x00000020;

    	/* Configure the flow control -> off*/
	/* Ignore RTS */
	HW_UART_UCR2 |= 0x00004000;
	/* CTS controlled by the CTS bit */
	HW_UART_UCR2 &= (~(0x00002000));

	/* the reference manual says that this bit must always be set */
 	HW_UART_UCR3 |= 0x00000004;

	/* Enable UART */
	HW_UART_UCR1 |= 0x00000001;

	/* Enable FIFOs and does software reset to clear status flags, reset the transmit and receive state machine, and reset the FIFOs */
	HW_UART_UCR2 |=0x00000007;

	/* Set the numerator and denominator value minus one of the BRM ratio */
	HW_UART_UBIR = ((baudrate / 100) - 1);
    	/* Set the denominator value minus one of the BRM ratio */
	HW_UART_UBMR = ((uart_freq/1600) - 1);
}

uint8_t ltz_uart_getchar(void){
	uint32_t new_data;
	// the bit [0] of HW_UART_USR2 -> Receive Data Ready -Indicates that at least 1 character is received and written to the RxFIFO.
	if(!(HW_UART_UCR2 & 0x00000001))
		return EMPTY;

	new_data = HW_UART_URXD;

	//check the errors [10-14] [10 - parity error, 11 - BREAK Detect, 12 - Frame Error, 13 - Receiver Overrun, 14 - Error Detect ]
	if(new_data & 0x00007C00)
		return ERROR_DATA;

	return ((uint8_t)new_data);
}

uint8_t ltz_uart_putchar(uint8_t  c){
	//wait if the fifo buffer is full
	while( HW_UART_UTS & 0x00000010);
	// Write register
	HW_UART_UTXD = (c & 0x000000ff);
	return c;
}

uint8_t ltz_uart_puts(uint8_t *buffer){
	int i = 0;
	while (*buffer){
		ltz_uart_putchar(*buffer);
		if(*buffer=='\n')
			ltz_uart_putchar('\r');
		buffer++;
		i++;
	}
	return i;
}
