#pragma once

#include "DelayLine.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_core/system/juce_PlatformDefs.h"

/* The big kahuna. Can handle either mono or stereo IO,
 * */

#define CLEAN_AUDIO_BUFFER

class Dattorro {
private:
  float preFilter;
  float damping[2];
  uint16_t maxPreDelay = 4410;
  // Delay lines & state------------------------------
  _DelayLine preDelay;
  juce::OwnedArray<_DelayLine> inputDiffusion;
  // the left and right 'tanks'
  juce::OwnedArray<_DelayLine> decayDiffusion1;
  juce::OwnedArray<_DelayLine> decayDiffusion2;
  juce::OwnedArray<_DelayLine> preDampingDelay;
  juce::OwnedArray<_DelayLine> postDampingDelay;
  // DelayLine decayDiffusion1[2];
  // DelayLine decayDiffusion2[2];
  // DelayLine preDampingDelay[2];
  // DelayLine postDampingDelay[2];
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
  // maybe better let's see
  void processBuffer(juce::AudioBuffer<float>& buf,
                     int inChannels,
                     int outChannels);

private:
  // both the process methods above are made out of these helpers
  void processInput(float input);
  float getLeft();
  float getRight();
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dattorro)
};
