#ifndef _OLED_DISPLAY_H_
#define _OLED_DISPLAY_H_

#include <stdbool.h>
#include <string.h>
#include "msp.h"
#include "common.h"

#define SSD1306_CONTROL_REG (0x00)
#define SSD1306_DATA_REG (0x40)
#define SSD1306_ADDR (0x3C)
#define SSD1306_DISPLAYOFF (0xAE)
#define SSD1306_MULTIPLEX_RATIO (0xA8)

void init_OLED_display(void);
void plot_line_data(uint16_t *line_data);

#endif
