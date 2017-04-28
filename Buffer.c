#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <TM4C123GH6PM.h>
#include <utils/uartstdio.c>
#include <stdarg.h>

void init_clk()
{
	//upgrade clock to 20MHz
	unsigned int* SYSCLK_INT = (unsigned int*) 0x400fe000;
	SYSCLK_INT[0x60/4] &= (1<<11); 		//set bypass bit, enables main oscillator, clears usesysdiv bit
	__nop();
	__nop();
	SYSCLK_INT[0x60/4] = 0xd40; 			//for xtal = 0x15, oscsrc - 0x0, enables pll
	SYSCLK_INT[0x60/4] |= 0x4c00000; 	//sysdiv = 0x9, use sysdiv = 1
	
	while(!SYSCLK_INT[0x50/4] & 0x40)
	{
		//empty
		//wait for PLLRIS to flip, PLL has been configured
	}

	SYSCLK_INT[0x60/4] |= (1<<22); //enable PLL
	SYSCLK_INT[0x60/4] &= ~(1<<11); //clear bypass
}

void init(void)
{
	SYSCTL->RCGCUART |= 0x01; //Enable UART0 bit 0
	SYSCTL->RCGCGPIO |= 0x01; //Enable Port A bit 0
	while((SYSCTL->PRGPIO & 0x0001) == 0){}; //Wait till the system is ready
	UART0->CTL &= ~0x00000001; //Disable UART
	UART0->IBRD = 8; // IBRD = int(16 MHz / (16 * 115,200)) = int(8.680)
	UART0->FBRD = 44; //FBRD = round(0.5104 * 64) = 33
	//UART0->IBRD = 10; // IBRD = int(20 MHz / (16 * 115200)) = 10.8507
	//UART0->FBRD = 54; //FBRD = round(0.8507 * 64 + 0.5) = 54
	UART0->LCRH = 0x00000070; //Enable FIFO and 8 bit word length
	UART0->CTL |= 0x00000001; //Enable UART
	GPIOA->AFSEL |= 0x03; //Enable alternate functions for PA1 and PA0 bits 1 and 0
	GPIOA->DEN |= 0x03; //Digital enable for PA1 and PA0 bits 1 and 0
	GPIOA->PCTL = (GPIOA->PCTL &0xFFFFFF00) + 0x00000011; //Keep all other values as previous - set pin 0 and 1 to mux 1
		//GPIOA->AMSEL &= ~0x03; //Disable Analog
}

int echo()
{
	char buffer[4];
	uint32_t length = 2000;
		
	UARTgets(buffer, length);
	UARTprintf(buffer);
	
	__nop();
	return 0;
}
int main()
{
	init();
	UARTStdioConfig(0, 115200, 16000000);

	echo();
	
	while(1);
}
