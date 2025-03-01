#pragma once

#include "PluginProcessor.h"

namespace audio_plugin {

class DattorroReverbEXProcessorEditor : public juce::AudioProcessorEditor {
public:
  explicit DattorroReverbEXProcessorEditor(DattorroReverbEXAudioProcessor&);
  ~DattorroReverbEXProcessorEditor() override;
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  DattorroReverbEXAudioProcessor& processorRef;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DattorroReverbEXProcessorEditor)
};

}  // namespace audio_plugin
