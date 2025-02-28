#include "DattorroVerb/RMSMeter.h"

void RMSMeter::setWindowSamples(uint16_t value) {
  // if we're shrinking the window past the current index,
  // calculate lastOutput and reset the sum and index now
  if (value >= idx) {
    lastOutput = std::sqrt(squareSum / (float)idx);
    idx = 0;
  }
  window = value;
}

float RMSMeter::process(float input) {
  if (idx == window) {
    lastOutput = std::sqrt(squareSum / (float)idx);
    idx = 0;
  } else {
    squareSum += (input * input);
    ++idx;
  }
  return lastOutput;
}
