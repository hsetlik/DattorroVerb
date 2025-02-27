#pragma once

#include "PluginProcessor.h"

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
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DattorroVerbProcessorEditor)
};

}  // namespace audio_plugin
