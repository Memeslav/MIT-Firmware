#include "main.h"

uint8_t flag = 0;

int main(void)
{
	CRC_Enable();
	Clock_Enable();

	Module_ADC_Enable();
	Module_PKM_Enable();

    while(1)
    {
    	if(flag)
    	{
    		Currents_Convert();
    		Impulse_Convert();

    		flag = 0;
    	}
    }
}
