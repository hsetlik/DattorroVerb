
#pragma once
#include "Common.h"
#include "juce_core/system/juce_PlatformDefs.h"

// max number of taps on a single delay line
#define MAX_TAPS 4

class DelayLine {
private:
  uint16_t maxDelay;
  uint16_t length;
  float* data;
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

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayLine)
};

//================================================
// this one takes the max delay size in the constructor to
// avoid the init thing
class _DelayLine {
private:
  uint16_t maxDelay;
  uint16_t length;
  float* data = nullptr;
  uint16_t mask = 0;

public:
  uint16_t offsets[MAX_TAPS];
  _DelayLine(uint16_t maxDelay);
  ~_DelayLine();
  // set the length of a delay tap
  void setDelay(uint8_t tap, uint16_t delaySamples);
  // read/write access
  float process(uint16_t cycle, float input);
  void write(uint16_t cycle, float input);
  float read(uint8_t tap, uint16_t cycle);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(_DelayLine)
};
