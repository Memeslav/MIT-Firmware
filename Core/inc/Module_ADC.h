#ifndef INC_MODULE_ADC_H_
#define INC_MODULE_ADC_H_


#include "string.h"
#include "Registers.h"
#include "stm32l053xx.h"


void Module_ADC_Settings_Update(void);
void Module_ADC_Enable(void);

void Currents_Convert(void);

void Impulse_Convert(void);

#endif
