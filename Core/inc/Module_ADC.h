#ifndef INC_MODULE_ADC_H_
#define INC_MODULE_ADC_H_

#include "stm32l053xx.h"

#include "Module_Registers.h"

void Module_ADC_Settings_Update(void);
void Module_ADC_Enable(void);

void Currents_Convert(void);
void Currents_Reset(void);

void Impulse_Convert(void);
void Impulse_Reset(void);

#endif
