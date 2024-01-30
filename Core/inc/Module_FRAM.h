#ifndef INC_MODULE_FRAM_H_
#define INC_MODULE_FRAM_H_

#include "stm32l053xx.h"


void Module_FRAM_Enable(void);


void FRAM_READ (uint16_t address,
				uint16_t data_size,
				uint8_t* data);

void FRAM_WRITE(uint16_t address,
				uint16_t data_size,
				uint8_t* data);

#endif
