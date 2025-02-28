#pragma once
#include "Common.h"
#include "DattorroVerb/Identifiers.h"
#include <juce_gui_basics/juce_gui_basics.h>

enum class ParamSliderType { Round, Vertical };

class ParamSlider : public juce::Component {
private:
  ParamSliderType sType = ParamSliderType::Round;
  juce::Slider slider;
  juce::Label label;
  slider_attach_ptr sAttach;

public:
  ParamSlider(apvts* tree,
              const juce::String& id,
              const juce::String& labelText = "null");

  juce::Slider* getSlider() { return &slider; }
  void resized() override;
  void setLabelText(const juce::String& txt);
  void setSliderType(ParamSliderType nType);
};
