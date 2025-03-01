#include "DattorroReverbEX/DelayLine.h"

DelayLine::DelayLine() : data(nullptr) {}

static uint16_t neededBufSize(uint16_t delay) {
  uint16_t sigBits = 0;
  while (delay) {
    sigBits++;
    delay >>= 1;
  }
  return (uint16_t)(1 << sigBits);
}

void DelayLine::init(uint16_t delay) {
  maxDelay = delay;
  length = neededBufSize(delay);
  data = new float[length];

  mask = length - 1;
  offsets[0] = mask + 1 - maxDelay;
}

void DelayLine::setDelay(uint8_t tap, uint16_t delay) {
  offsets[tap] = mask + 1 - delay;
}

float DelayLine::process(uint16_t cycle, float input) {
  write(cycle, input);
  return read(0, cycle);
}

void DelayLine::write(uint16_t cycle, float input) {
  data[cycle & mask] = input;
}

float DelayLine::read(uint8_t tap, uint16_t cycle) {
  return data[(cycle + offsets[tap]) & mask];
}

//======================================================

_DelayLine::_DelayLine(uint16_t max) {
  maxDelay = max;
  length = neededBufSize(maxDelay);
  data = new float[length];
  std::fill(data, data + (size_t)length, 0.0f);
  mask = length - 1;
  offsets[0] = mask + 1 - maxDelay;
}

_DelayLine::~_DelayLine() {
  delete[] data;
}

void _DelayLine::setDelay(uint8_t tap, uint16_t delay) {
  offsets[tap] = mask + 1 - delay;
}

float _DelayLine::process(uint16_t cycle, float input) {
  write(cycle, input);
  return read(0, cycle);
}

void _DelayLine::write(uint16_t cycle, float input) {
  data[cycle & mask] = input;
}

float _DelayLine::read(uint8_t tap, uint16_t cycle) {
  return data[(cycle + offsets[tap]) & mask];
}
