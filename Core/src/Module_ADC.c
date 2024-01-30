#include "Module_ADC.h"


#define supply 3.28
#define gain  27


typedef enum
{
	OUTSIDE_THE_IMPULS,
	INSIDE_THE_IMPULS
}
ADC_State;


struct ADC_Data
{
	int16_t input;
	int16_t offset;

	int16_t signal;

	int64_t cur_sums;
	int64_t cur_times;

	int64_t imp_sums;
	int64_t imp_times;

	ADC_State state;

	Currents currents;

	Impulse impulse;
}
ADC_Data = {0};


static void DMA1_Enable(void)
{
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

		DMA1_Channel1->CNDTR = 2;
		DMA1_Channel1->CMAR  = (uint32_t)(&ADC_Data);
		DMA1_Channel1->CPAR  = (uint32_t)(&(ADC1->DR));

		DMA1_Channel1->CCR   = DMA_CCR_MSIZE_0
							|  DMA_CCR_PSIZE_0
							|  DMA_CCR_PL_1
							|  DMA_CCR_MINC
							|  DMA_CCR_CIRC
							|  DMA_CCR_EN;
}

static void ADC1_Enable(void)
{
	RCC->IOPENR  |= RCC_IOPENR_GPIOAEN
				 |  RCC_IOPENR_GPIOBEN;

		GPIOA->MODER |= GPIO_MODER_MODE2_Msk;

		GPIOB->MODER |= GPIO_MODER_MODE0_Msk
					 |  GPIO_MODER_MODE1_Msk;

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

		ADC1->CFGR2   = ADC_CFGR2_CKMODE_Msk
					 |	ADC_CFGR2_OVSR_Msk
					 |	ADC_CFGR2_OVSS_3
					 |  ADC_CFGR2_OVSE;

		ADC->CCR 	 |= ADC_CCR_VREFEN
					 |  ADC_CCR_TSEN;

		ADC1->CR 	 |= ADC_CR_ADVREGEN
					 |  ADC_CR_ADCAL;

		while(ADC1->CR & ADC_CR_ADCAL);

		ADC1->CHSELR  =  ADC_CHSELR_CHSEL8
					 |   ADC_CHSELR_CHSEL9;

		ADC1->CFGR1  |= ADC_CFGR1_EXTEN_0
				     |  ADC_CFGR1_AUTOFF
					 |  ADC_CFGR1_DMACFG
					 |  ADC_CFGR1_DMAEN
					 |  ADC_CFGR1_WAIT;

		ADC1->SMPR 	  = ADC_SMPR_SMP_Msk;

		ADC1->IER	  = ADC_IER_EOSIE;

		ADC1->CR 	 |= ADC_CR_ADSTART
					 |  ADC_CR_ADEN;

	NVIC_SetPriority(ADC1_COMP_IRQn, 0);
	NVIC_EnableIRQ(ADC1_COMP_IRQn);
}

void Module_ADC_Enable(void)
{
	DMA1_Enable();
	ADC1_Enable();
}


static float From_level_to_mVolt(int level)
{
	return (100000 * ((supply/4096) * (level)/gain));
}

static int From_mVolt_to_level(float mVolt)
{
	return ((mVolt / 100000) * gain * 4096 / supply);
}


void static Currents_Update(void)
{
	ADC_Data.cur_times++;
	ADC_Data.cur_sums += ADC_Data.signal;

	if (ADC_Data.signal < ADC_Data.currents.min)
	{
		ADC_Data.currents.min = ADC_Data.signal;
	}

	else if(ADC_Data.signal > ADC_Data.currents.max)
	{
		ADC_Data.currents.max = ADC_Data.signal;
	}

	ADC_Data.currents.ave  =  ADC_Data.cur_sums / ADC_Data.cur_times;
}

void Currents_Reset(void)
{
	ADC_Data.cur_sums  = 0;
	ADC_Data.cur_times = 0;

	ADC_Data.currents.ave   = 0;
	registers.currents.ave  = 0;

	ADC_Data.currents.min   = 4095;
	registers.currents.min  = 4095;

	ADC_Data.currents.max   = -4095;
	registers.currents.max  = -4095;
}

void Currents_Convert(void)
{
	registers.currents.min = From_level_to_mVolt(ADC_Data.currents.min);
	registers.currents.ave = From_level_to_mVolt(ADC_Data.currents.ave);
	registers.currents.max = From_level_to_mVolt(ADC_Data.currents.max);
}




void Impulse_Start(void)
{

}

void Impulse_Update(void)
{
	if(ADC_Data.state == OUTSIDE_THE_IMPULS)
	{
		if(ADC_Data.signal >= registers.settings.trigger_level)
		{
			ADC_Data.state = INSIDE_THE_IMPULS;

			//Impulse_Start();
		}
	}
	else
	{
		//Impulse_Record();

		if(ADC_Data.signal < registers.settings.trigger_level)
		{
			//Impulse_Saving();

			ADC_Data.state = OUTSIDE_THE_IMPULS;
		}
	}
}




void Module_ADC_Settings_Update(void)
{
	registers.settings.trigger_level = From_mVolt_to_level(registers.settings.trigger_level);

	TIM6->ARR = registers.settings.measur_period - 1;
}




void ADC1_COMP_IRQHandler(void)
{
	if(ADC1->ISR & ADC_ISR_EOS)
	{
		ADC1->ISR |= ADC_ISR_EOS;

		ADC_Data.signal = ADC_Data.input - ADC_Data.offset;

		Currents_Update();

		Impulse_Update();
	}

	NVIC_ClearPendingIRQ(ADC1_COMP_IRQn);
}
