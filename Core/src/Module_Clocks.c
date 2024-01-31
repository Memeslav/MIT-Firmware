#include "Module_Clocks.h"


static void TIM2_Enable(void)
{
	SystemCoreClockUpdate();

    RCC->APB1ENR   |= RCC_APB1ENR_TIM2EN;

    	TIM2->PSC   = SystemCoreClock/1000 - 1;

    	TIM2->DIER |= TIM_DIER_UIE;

    	TIM2->CR1  |= TIM_CR1_CEN;

    	TIM2->ARR   = 999;

    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);
}


void Clock_Enable(void)
{
	TIM2_Enable();
}


void TIM2_IRQHandler(void)
{
	if(++registers.unixtime.lo == 0xFFFF)
	{	 registers.unixtime.hi ++;}

	IWDG_Update();

	TIM2->SR &= ~TIM_SR_UIF;

	NVIC_ClearPendingIRQ(TIM2_IRQn);
}
