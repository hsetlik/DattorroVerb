#pragma once
/* This namespace is were all the string identifiers for
 * parameters, components, and anything else should live.
 * Simply use the `DECLARE_ID` macro and never worry about
 * misspelling a string literal again.
 *
 * This translation unit is also responsible for the
 * `ID::getParameterLayout()` function which we use to set up
 * the AudioProcessorValueTreeState that runs everything
 * */
#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>

// typedefs bc some of these types are a mouthful
typedef juce::AudioProcessorValueTreeState apvts;

typedef juce::NormalisableRange<float> frange_t;

// it's always annoyed me that juce doesn't just provide this constructor
frange_t rangeWithCenter(float start, float end, float center);

#define DECLARE_ID(name) const juce::Identifier name(#name);

namespace ID {
// top level ID for the apvts
DECLARE_ID(DattorroVerb_state)

// effect parameters
DECLARE_ID(preDelayAmt)
DECLARE_ID(preFilterAmt)
DECLARE_ID(inputDiff1Amt)
DECLARE_ID(inputDiff2Amt)
DECLARE_ID(decayDiff1Amt)
DECLARE_ID(dampingAmt)
DECLARE_ID(decayAmt)
DECLARE_ID(wetDry)

apvts::ParameterLayout getParameterLayout();
}  // namespace ID

#undef DECLARE_ID
