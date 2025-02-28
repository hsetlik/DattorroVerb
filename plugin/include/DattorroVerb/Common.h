#pragma once
/* This file just includes juce and sets up a few
 * typedefs that I find handy in every juce project
 * */
#include "Identifiers.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_graphics/juce_graphics.h"
constexpr float twoPi_f = juce::MathConstants<float>::twoPi;

// some aliases to shorten things a bit
typedef std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
    slider_attach_ptr;
typedef std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
    combo_attach_ptr;
typedef juce::Rectangle<float> f_rect;
typedef juce::Rectangle<int> i_rect;
//
// maybe we'll avoid writing 5 zillion 'prepare()' functions this way
namespace SampleRate {
void set(double rate);
double get();
}  // namespace SampleRate

inline float flerp(float a, float b, float t) {
  return a + ((b - a) * t);
}
