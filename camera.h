#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <stdbool.h>
#include "msp.h"
#include "common.h"
#include "cortex_m.h"
#include "sys_tick_timer.h"
#include "adc.h"
#include "periodic_timer.h"
#include "oled_display.h"

#define SI_PORT (P5)
#define SI_PIN (BIT5)
#define CLK_PORT (P5)
#define CLK_PIN (BIT4)

#define PIXEL_COUNT (128)
#define CENTER_PIXEL (PIXEL_COUNT / 2)

#define INTEGRATION_SECS (0.0075f)
// was 0.0075f
// sampled with 0.004

extern uint16_t camera_data[PIXEL_COUNT];

void init_camera(void);

#endif
