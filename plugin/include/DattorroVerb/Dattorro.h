#pragma once

#include "DelayLine.h"
#include "juce_core/system/juce_PlatformDefs.h"

/* The big kahuna. Can handle either mono or stereo IO,
 * */

// #define WET_ONLY

class Dattorro {
private:
  // Delay lines & state------------------------------
  DelayLine preDelay;
  DelayLine inDiffusion[4];
  float preFilter;
  uint16_t maxPreDelay;
  // the left and right 'tanks'
  DelayLine decayDiffusion1[2];
  DelayLine decayDiffusion2[2];
  DelayLine preDampingDelay[2];
  DelayLine postDampingDelay[2];
  float damping[2];
  // for tracking our sample offset
  uint16_t t = 0;
  // Parameters------------------------------
  float preFilterAmt;
  float inputDiff1Amt;
  float inputDiff2Amt;
  float decayDiff1Amt;
  float decayDiff2Amt;
  float dampingAmt;
  float decayAmt;
  float wetDry;

public:
  Dattorro();
  // call this in the 'prepare()' callback to set up the delay lines
  void init(double sampleRate);
  // call this in 'processBlock' to update from the UI
  void updateParams(apvts& tree);
  // process a number of samples. be sure to call the correct
  // stereo/mono version
  void processMono(float* writePtr, int numSamples);
  void processStereo(float* lPtr,
                     float* rPtr,
                     int numSamples,
                     bool inputIsMono = false);

private:
  // both the process methods above are made out of these helpers
  void processInput(float input);
  float getLeft();
  float getRight();
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dattorro)
};
