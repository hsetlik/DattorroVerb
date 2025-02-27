#include "DattorroVerb/Identifiers.h"

frange_t rangeWithCenter(float start, float end, float center) {
  frange_t range(start, end);
  range.setSkewForCentre(center);
  return range;
}

apvts::ParameterLayout ID::getParameterLayout() {
  apvts::ParameterLayout layout;
  // ranges and defaults
  const frange_t preDelayRange = rangeWithCenter(0.0f, 1.0f, 0.35f);
  const float preDelayDefault = 0.1f;
  const frange_t preFilterRange = rangeWithCenter(0.0f, 1.0f, 0.75f);
  const float preFilterDefault = 0.85f;
  const frange_t iDiff1Range = rangeWithCenter(0.0f, 1.0f, 0.6f);
  const float iDiff1Default = 0.75f;
  const frange_t iDiff2Range = rangeWithCenter(0.0f, 1.0f, 0.6f);
  const float iDiff2Default = 0.625f;
  const frange_t decayRange = rangeWithCenter(0.0f, 1.0f, 0.6f);
  const float decayDefault = 0.75f;
  const frange_t dDiffRange = rangeWithCenter(0.0f, 1.0f, 0.7f);
  const float dDiffDefault = 0.7f;
  const frange_t dampingRange = rangeWithCenter(0.0f, 1.0f, 0.85f);
  const float dampingDefault = 0.95f;
  const frange_t wetDryRange = rangeWithCenter(0.0f, 1.0f, 0.5f);
  const float wetDryDefault = 0.8f;

  // add the params
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::preDelayAmt.toString(), "Pre-delay", preDelayRange, preDelayDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::preFilterAmt.toString(), "Pre-filter", preFilterRange,
      preFilterDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::inputDiff1Amt.toString(), "In diffusion 1", iDiff1Range,
      iDiff1Default));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::inputDiff2Amt.toString(), "In diffusion 2", iDiff2Range,
      iDiff2Default));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::decayAmt.toString(), "Decay", decayRange, decayDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::decayDiff1Amt.toString(), "Decay diffusion", dDiffRange,
      dDiffDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::dampingAmt.toString(), "Damping", dampingRange, dampingDefault));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      ID::wetDry.toString(), "Wet/dry", wetDryRange, wetDryDefault));
  return layout;
}
