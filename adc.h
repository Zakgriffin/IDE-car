#ifndef _ADC_H_
#define _ADC_H_

#include "msp.h"
#include "stdint.h"

void init_camera_ADC_pin(void);

void init_ADC_pin(void);
uint16_t read_adc_blocking(void);

#endif
