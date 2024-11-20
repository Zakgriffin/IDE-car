#include "common.h"

unsigned long hz_to_rollover(unsigned long frequency_hz) {
  return SystemCoreClock / frequency_hz;
}

double map_range(double n, double start1, double stop1, double start2, double stop2) {
  return ((n - start1) / (stop1 - start1)) * (stop2 - start2) + start2;
};

double clamp_within(double duty, double low, double high) {
  if (duty > high) return high;
  if (duty < low) return low;
  return duty;
}

double double_abs(double d) { return d > 0 ? d : -d; }
