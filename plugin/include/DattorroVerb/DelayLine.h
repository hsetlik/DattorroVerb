
#pragma once
#include "Common.h"

// max number of taps on a single delay line
#define MAX_TAPS 4

class DelayLine {
private:
  uint16_t maxDelay;
  uint16_t length;
  std::unique_ptr<float[]> data;
  uint16_t mask = 0;

public:
  uint16_t offsets[MAX_TAPS];
  DelayLine();
  void init(uint16_t delay);
  // set the length of a delay tap
  void setDelay(uint8_t tap, uint16_t delaySamples);
  // read/write access
  float process(uint16_t cycle, float input);
  void write(uint16_t cycle, float input);
  float read(uint8_t tap, uint16_t cycle);
};
