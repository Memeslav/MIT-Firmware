#include "Module_IWDG.h"

void Module_IWDG_Enable(void)
{
	RCC->CSR |= RCC_CSR_LSION;

		while(!(RCC->CSR & RCC_CSR_LSIRDY));

	IWDG->KR = 0x0000CCCC;
	IWDG->KR = 0x00005555;

	IWDG->PR = IWDG_PR_PR_Msk;

	IWDG->RLR = 0x0FFF;

		while(!(IWDG->SR));

	IWDG->KR = 0x0000AAAA;
}

void IWDG_Update(void)
{
	IWDG->KR = 0x0000AAAA;
}
