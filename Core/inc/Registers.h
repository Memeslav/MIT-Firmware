#ifndef INC_REGISTERS_H_
#define INC_REGISTERS_H_


#include "stm32l053xx.h"

typedef enum
{
	serial_number 		= 0,
	program_version 	= 1,

	unixtime_lo 		= 2,
	unixtime_hi 		= 3,

	currents_minimal 	= 4,
	currents_average 	= 5,
	currents_maximal 	= 6,

	trigger_level 		= 7,
	minimal_pulse 		= 8,
	measur_period 		= 9,
	reset_setting 	  	= 10,

	global_counter_lo 	= 11,
	global_counter_hi 	= 12,
	last_readed_lo 	  	= 13,
	last_readed_hi 	  	= 14,
	display_lo 		  	= 15,
	display_hi 		  	= 16,

	num_lo 			  	= 17,
	num_hi 			  	= 18,
	time_start_lo 	  	= 19,
	time_start_hi 	  	= 20,
	time_start_shift  	= 21,
	time_duration 	  	= 22,
	maximal_amplitude 	= 23,
	average_amplitude 	= 24,
}
Register_address;


typedef struct
{
	uint16_t serial_number;
	uint16_t program_version;
}
Module_info;

typedef struct
{
	uint16_t lo;
	uint16_t hi;
}
UNIXTIME;

typedef struct
{
	int16_t min;
	int16_t ave;
	int16_t max;
}
Currents;

typedef struct
{
	int16_t  trigger_level;
	uint16_t minimal_pulse;
	uint16_t measur_period;
	uint16_t reset_setting;
}
Settings;

typedef struct
{
	uint16_t global_counter_lo;
	uint16_t global_counter_hi;

	uint16_t last_readed_lo;
	uint16_t last_readed_hi;

	uint16_t display_lo;
	uint16_t display_hi;
}
Controls;

typedef struct
{
	uint16_t num_lo;
	uint16_t num_hi;

	uint16_t start_lo;
	uint16_t start_hi;

	uint16_t start_shift;
	uint16_t duration;

	int16_t max_amp;
	int16_t ave_amp;
}
Impulse;

typedef struct
{
	Module_info module_info;

	UNIXTIME 	unixtime;

	Currents 	currents;

	Settings 	settings;

	Controls 	controls;

	Impulse  	impulse;
}
Registers;

extern volatile Registers registers;
extern uint16_t* regs;


#endif
