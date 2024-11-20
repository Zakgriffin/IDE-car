#include "camera.h"

uint16_t camera_data[128];
long pixel_counter = 0;
bool pixel_was_collected = false;

void SI_handler(void) {
  // the clock could stop when its high, so check on that
  if ((CLK_PORT->OUT & CLK_PIN) != 0)
    CLK_PORT->OUT &= ~CLK_PIN;  // set the clock low in case it was high.

  SI_PORT->OUT |= SI_PIN;
  CLK_PORT->OUT |= CLK_PIN;
  SI_PORT->OUT &= ~SI_PIN;
  CLK_PORT->OUT &= ~CLK_PIN;

  // OK, Data should be ready to clock out, so start the clock
  enable_sys_tick_timer();
}

void init_camera_SI_pin() {
  unsigned long rollover = hz_to_rollover(1.0 / (double)INTEGRATION_SECS);
  SI_PORT->SEL0 &= ~SI_PIN;  // gpio
  SI_PORT->SEL1 &= ~SI_PIN;  // gpio
  SI_PORT->DIR |= SI_PIN;    // output
  SI_PORT->DS |= SI_PIN;     // high drive strength

  init_periodic_timer_1(*SI_handler, rollover, T32DIV1);
}

void CLK_handler(void) {
  CLK_PORT->OUT ^= CLK_PIN;
  if ((CLK_PORT->OUT & CLK_PIN) != 0) {
    uint16_t pixel = read_adc_blocking();
    camera_data[pixel_counter] = pixel;
    pixel_counter++;
    if (pixel_counter == 128) {
      pixel_was_collected = true;
      // stop the clock until the next SI pulse
      disable_sys_tick_timer();
      pixel_counter = 0;
    }
  }
}

void init_camera_CLK_pin() {
  unsigned long rollover_ticks = hz_to_rollover(100000 * 2);

  CLK_PORT->SEL0 &= ~CLK_PIN;  // gpio
  CLK_PORT->SEL1 &= ~CLK_PIN;  // gpio
  CLK_PORT->DIR |= CLK_PIN;    // output
  CLK_PORT->DS |= CLK_PIN;     // high drive strength

  init_sys_tick_timer(*CLK_handler, rollover_ticks);
  disable_sys_tick_timer();
}

void init_camera(void) {
  disable_interrupts();

  init_camera_SI_pin();
  init_camera_CLK_pin();
  init_ADC_pin();

  enable_interrupts();
}
