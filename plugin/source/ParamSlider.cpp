#include "DattorroVerb/ParamSlider.h"
#include "juce_events/juce_events.h"
/*
ParamSlider::ParamSlider(apvts* tree,
                         const juce::String& id,
                         const juce::String& labelText) {
  // 1. set up slider/label stuff as needed
  addAndMakeVisible(&slider);
  addAndMakeVisible(&label);
  label.setText(labelText, juce::dontSendNotification);
  setSliderType(sType);
  // 2. set up the slider attachment
  sAttach.reset(new apvts::SliderAttachment(*tree, id, slider));
}
 */
ParamSlider::ParamSlider() : sAttach(nullptr) {
  addAndMakeVisible(slider);
  addAndMakeVisible(label);
  setSliderType(sType);
}

void ParamSlider::attach(apvts& tree,
                         const juce::Identifier& id,
                         juce::StringRef labelText) {
  sAttach.reset(new apvts::SliderAttachment(tree, id.toString(), slider));
  label.setText(labelText, juce::dontSendNotification);
}

void ParamSlider::setSliderType(ParamSliderType nType) {
  sType = nType;
  switch (sType) {
    case ParamSliderType::Round:
      slider.setSliderStyle(juce::Slider::Rotary);
      slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 50);
      break;
    case ParamSliderType::Vertical:
      slider.setSliderStyle(juce::Slider::LinearVertical);
      slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 50);
      break;
    default:
      break;
  }
}

void ParamSlider::setLabelText(const juce::String& txt) {
  label.setText(txt, juce::dontSendNotification);
}

void ParamSlider::resized() {
  auto fBounds = getLocalBounds().toFloat();
  constexpr float minLabelHeight = 20.0f;
  const float labelHeight =
      std::max(fBounds.getHeight() * 0.2f, minLabelHeight);
  auto lBounds = fBounds.removeFromTop(labelHeight).toNearestInt();
  label.setBounds(lBounds);
  slider.setBounds(fBounds.toNearestInt());
}
