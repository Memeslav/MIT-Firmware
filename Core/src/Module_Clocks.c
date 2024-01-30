#include "Module_Clocks.h"




static void TIM2_Enable(void)
{
	SystemCoreClockUpdate();

    RCC->APB1ENR   |= RCC_APB1ENR_TIM2EN;

    	TIM2->PSC   = SystemCoreClock/10000 - 1;

    	TIM2->DIER |= TIM_DIER_UIE;

    	TIM2->CR1  |= TIM_CR1_CEN;

    	TIM2->ARR   = 9999;

    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);
}

static void TIM6_Enable(void)
{
	SystemCoreClockUpdate();

	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

		TIM6->PSC = SystemCoreClock/1000 - 1;

		TIM6->ARR = registers.settings.measur_period - 1;

		TIM6->CNT = 0;

		TIM6->CR2 = TIM_CR2_MMS_1;

		TIM6->CR1 = TIM_CR1_CEN;
}

void Module_Clocks_Enable(void)
{
	TIM2_Enable();
	TIM6_Enable();
}




void TIM2_IRQHandler(void)
{
	if(TIM2->CNT % 10 == 0)
	{
		if(++registers.unixtime.lo == 0xFFFF)
		{	 registers.unixtime.hi ++;}

		USART1->TDR = 0xAA;
	}

	TIM2->SR &= ~TIM_SR_UIF;

	NVIC_ClearPendingIRQ(TIM2_IRQn);
}
