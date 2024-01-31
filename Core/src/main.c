#include "main.h"

int main(void)
{
	CRC_Enable();
	Clock_Enable();

	Module_ADC_Enable();
	Module_PKM_Enable();

    while(1)
    {

    }
}
