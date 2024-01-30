#include "Module_FRAM.h"


#define	CS_LOW	(GPIOB->BSRR |= GPIO_BSRR_BR_12);
#define	CS_HIG	(GPIOB->BSRR |= GPIO_BSRR_BS_12);


typedef enum
{
	WREN  = 0x06,
	READ  = 0x03,
	WRITE = 0x02,
}
FRAM_Commands;


void SPI2_Enable(void)
{
    RCC->IOPENR   |= RCC_IOPENR_GPIOBEN;

		GPIOB->MODER  &= ~(GPIO_MODER_MODE12_1
					  |    GPIO_MODER_MODE13_0
					  |    GPIO_MODER_MODE14_0
					  |    GPIO_MODER_MODE15_0);

    RCC->APB1ENR  |= RCC_APB1ENR_SPI2EN;

    	SPI2->CR2  = 0;

		SPI2->CR1  = SPI_CR1_MSTR
				  |  SPI_CR1_SPE
				  |  SPI_CR1_SSM
				  |  SPI_CR1_SSI;
}

void Module_FRAM_Enable(void)
{
	SPI2_Enable();
}


uint8_t SPI2_SendByte (uint8_t data)
{
	while (!(SPI2->SR & SPI_SR_TXE));
	SPI2->DR = data;

	while (!(SPI2->SR & SPI_SR_RXNE));
	return SPI2->DR;
}


void FRAM_READ (uint16_t address,
				uint16_t data_size,
				uint8_t* data)
{
	CS_LOW;

	SPI2_SendByte(READ);

	SPI2_SendByte((address >> 8) & 0xFF);
	SPI2_SendByte(address & 0xFF);

	for(uint16_t i = 0; i < data_size; i++)
	{
		data[i] = SPI2_SendByte(0x00);
	}

	CS_HIG;
}

void FRAM_WRITE(uint16_t address,
				uint16_t data_size,
				uint8_t* data)
{
	CS_LOW;
	SPI2_SendByte(WREN);
	CS_HIG;

	CS_LOW;

	SPI2_SendByte(WRITE);

	SPI2_SendByte((address >> 8) & 0xFF);
	SPI2_SendByte(address & 0xFF);

	for(uint16_t i = 0; i < data_size; i++)
	{
		SPI2_SendByte(data[i]);
	}

	CS_HIG;
}
