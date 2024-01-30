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

		uint32_t baudrate = 9600;

		USART1->BRR    = SystemCoreClock/baudrate;

		USART1->CR3    = USART_CR3_DMAR
					  |  USART_CR3_DMAT;

		USART1->CR2    = USART_CR2_RTOEN;

		USART1->CR1    = USART_CR1_RXNEIE
					  |  USART_CR1_RTOIE
					  |  USART_CR1_UE
					  |  USART_CR1_RE
					  |  USART_CR1_TE;

	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(USART1_IRQn);
}
