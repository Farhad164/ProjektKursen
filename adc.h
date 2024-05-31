#ifndef ADC_H
#define ADC_H
#define ADC_EXTTRIG_REGULAR_NONE   ((uint32_t)0x00000000U) 
#include "gd32vf103.h"

void adc_config(void);
uint16_t read_adc_value(uint32_t channel);

#endif /* ADC_H */
