#include "Registers.h"


volatile Registers registers = 	{.module_info   = {1,		//serial_number
										   	   	   4},		//program_version

								 .unixtime 		= {0,		//lo
												   0},		//hi

								 .currents 		= {0,		//minimal
												   0,		//average
												   0},		//maximal

								 .settings 		= {5000, 	//trigger_level
												   0,		//minimal_pulse
												   100,		//measur_period
												   0},		//reset_setting

								 .controls 		= {1,		//global_counter_lo
												   0,		//global_counter_hi
												   0,		//last_readed_lo
												   0,		//last_readed_hi
												   0,		//display_lo
												   0},		//display_hi

								 .impulse  		= {0,		//num_lo
												   0,		//num_hi
												   0,		//time_start_lo
												   0,		//time_start_hi
												   0,		//time_start_shift
												   0,		//time_duration
												   0,		//maximal_amplitude
												   0}};		//average_amplitude


uint16_t* regs = (uint16_t*)&registers;
