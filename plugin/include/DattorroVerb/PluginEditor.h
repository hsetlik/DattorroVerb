#pragma once

#include "PluginProcessor.h"
#include "ParamSlider.h"

namespace audio_plugin {

class DattorroVerbProcessorEditor : public juce::AudioProcessorEditor {
public:
  explicit DattorroVerbProcessorEditor(DattorroVerbAudioProcessor&);
  ~DattorroVerbProcessorEditor() override;
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  DattorroVerbAudioProcessor& processorRef;
  // the sliders
  ParamSlider sPreDelay;
  ParamSlider sPreFilter;
  ParamSlider sInDiff1;
  ParamSlider sInDiff2;
  ParamSlider sDecayDiff;
  ParamSlider sDamping;
  ParamSlider sDecay;
  ParamSlider sWetDry;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DattorroVerbProcessorEditor)
};

}  // namespace audio_plugin
