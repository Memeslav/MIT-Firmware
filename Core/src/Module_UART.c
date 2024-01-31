#include "Module_UART.h"

void Module_UART_Enable(void)
{
	SystemCoreClockUpdate();

	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

		GPIOA->MODER  &= ~(GPIO_MODER_MODE9_0
					  |    GPIO_MODER_MODE10_0);

		GPIOA->AFR[1] |=  (0b0100 << GPIO_AFRH_AFSEL9_Pos
					  |    0b0100 << GPIO_AFRH_AFSEL10_Pos);

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

		USART1->BRR    = SystemCoreClock/9600;

		USART1->CR3    = USART_CR3_DMAT;

		USART1->CR2    = 0;

		USART1->CR1    = USART_CR1_UE
					  |  USART_CR1_TE;
}
