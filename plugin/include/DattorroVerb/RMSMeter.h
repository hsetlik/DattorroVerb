#pragma once
#include "Common.h"

#define RMS_MAX_WINDOW 25000
#define RMS_MIN_WINDOW 250
#define RMS_DEFAULT_WINDOW 3000

/* Returns the RMS signal level
 * over a given window
 * */

class RMSMeter {
private:
  uint16_t window = RMS_DEFAULT_WINDOW;
  uint16_t idx = 0;
  float squareSum = 0.0f;
  float lastOutput = 0.0f;

public:
  RMSMeter() = default;
  uint16_t getWindowSamples() const { return window; }
  void setWindowSamples(uint16_t value);
  // main function. Takes an audio sample as an input and
  // returns the current RMS signal
  float process(float input);
};
