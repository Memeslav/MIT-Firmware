#include "main.h"


int main(void)
{
	Module_FRAM_Enable();
	Module_Clocks_Enable();
	Module_ADC_Enable();

	Module_UART_Enable();

    while(1)
    {

    }
}
