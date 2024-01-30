#include "Module_PKM.h"

typedef struct
{
	uint16_t data[29];

	uint16_t index;
	uint16_t command;
	uint16_t start_address;
	uint16_t register_count;
}
Receive_buffer;
Receive_buffer rx = {{0}, 0, 0, 0, 0};


static void SPI1_Enable (void)
{
    RCC->IOPENR   |= RCC_IOPENR_GPIOAEN;

    	GPIOA->MODER &= ~(GPIO_MODER_MODE4_Msk
				  	 |    GPIO_MODER_MODE5_Msk
					 |    GPIO_MODER_MODE6_Msk
					 |    GPIO_MODER_MODE7_Msk);

		GPIOA->MODER |=	  GPIO_MODER_MODE4_1
					 |	  GPIO_MODER_MODE5_1
					 |    GPIO_MODER_MODE6_1
					 |    GPIO_MODER_MODE7_1;

		EXTI->IMR 	 |=   EXTI_IMR_IM4;
		EXTI->RTSR   |=   EXTI_RTSR_RT4;
		EXTI->FTSR   |=   EXTI_FTSR_FT4;

	NVIC_SetPriority(EXTI4_15_IRQn, 0);
	NVIC_EnableIRQ(EXTI4_15_IRQn);

    RCC->APB2ENR  |= RCC_APB2ENR_SPI1EN;

    	SPI1->CR2  = SPI_CR2_RXNEIE;

		SPI1->CR1  = SPI_CR1_CPHA
				  |  SPI_CR1_CPOL
				  |  SPI_CR1_DFF
				  |  SPI_CR1_SPE;

	NVIC_SetPriority(SPI1_IRQn, 1);
	NVIC_EnableIRQ(SPI1_IRQn);
}
void Module_PKM_Enable(void)
{
	SPI1_Enable();
}


void EXTI4_15_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PIF4)
	{
		if(!(GPIOA->IDR & GPIO_IDR_ID4))
		{
			memset(&rx, 0, sizeof(rx));
			CRC->CR |= CRC_CR_RESET;
		}
		EXTI->PR |= EXTI_PR_PIF4;
	}
	NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
}
void SPI1_IRQHandler(void)
{
	if(SPI1->SR & SPI_SR_RXNE)
	{
		rx.data[rx.index] = SPI1->DR;

		if(rx.index == 0)
		{
			rx.command 			= rx.data[0] & 0x8000;
			rx.start_address 	= rx.data[0] & 0x7FFF;
		}

		if(rx.index == 1)
		{
			 rx.register_count  = __REV16(rx.data[1]);
		}

		if(rx.command)
		{
			if(rx.index == (2 + rx.register_count))
			{
				for(uint8_t i = 0; i < rx.index; i++)
				{
					*((__IO uint16_t *)&CRC->DR) = rx.data[i];
				}

				if(__REV16(CRC->DR) == rx.data[rx.index])
				{
					uint8_t increment = 0;

					for(uint8_t i = rx.start_address; i < (rx.start_address + rx.register_count); i++)
					{
						regs[i] = __REV16(rx.data[2 + increment]);

						increment++;
					}
				}
			}
		}
		else
		{
			if		(rx.index == 0)
	        {
				*((__IO uint16_t *)&CRC->DR) = rx.start_address;
									SPI1->DR = rx.start_address;
	        }
			else if	(rx.index == 1)
	        {
				*((__IO uint16_t *)&CRC->DR) = __REV16(rx.register_count);
									SPI1->DR = __REV16(rx.register_count);
	        }
			else if	(rx.index <  (2 + rx.register_count))
	        {
				Currents_Convert();
				Impulse_Convert();

				*((__IO uint16_t *)&CRC->DR) = __REV16(regs[rx.start_address]);
									SPI1->DR = __REV16(regs[rx.start_address]);
									rx.start_address++;
	        }
			else if (rx.index == (2 + rx.register_count))
	        {
				SPI1->DR = __REV16(CRC->DR);
	        }
		}

		rx.index++;
	}

	NVIC_ClearPendingIRQ(SPI1_IRQn);
}
