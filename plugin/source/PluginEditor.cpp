#include "DattorroVerb/PluginEditor.h"
#include "DattorroVerb/Identifiers.h"
#include "DattorroVerb/PluginProcessor.h"
#include "juce_core/juce_core.h"

namespace audio_plugin {
DattorroVerbProcessorEditor::DattorroVerbProcessorEditor(
    DattorroVerbAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p)
/*       sPreDelay(&p.tree, ID::preDelayAmt.toString(), "Pre-delay"),
      sPreFilter(&p.tree, ID::preFilterAmt.toString(), "Pre-filter"),
      sInDiff1(&p.tree, ID::inputDiff1Amt.toString(), "In diff. 1"),
      sInDiff2(&p.tree, ID::inputDiff2Amt.toString(), "In diff. 2"),
      sDecayDiff(&p.tree, ID::decayDiff1Amt.toString(), "Decay diff."),
      sDamping(&p.tree, ID::dampingAmt.toString(), "Damping"),
      sDecay(&p.tree, ID::decayAmt.toString(), "Decay"),
      sWetDry(&p.tree, ID::wetDry.toString(), "Dry/Wet mix") */
{
  juce::ignoreUnused(processorRef);
  addAndMakeVisible(&sPreDelay);
  addAndMakeVisible(&sPreFilter);
  addAndMakeVisible(&sInDiff1);
  addAndMakeVisible(&sInDiff2);
  addAndMakeVisible(&sDecayDiff);
  addAndMakeVisible(&sDamping);
  addAndMakeVisible(&sDecay);
  addAndMakeVisible(&sWetDry);
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  sPreDelay.attach(p.tree, ID::preDelayAmt.toString(), "Pre-delay");
  sPreFilter.attach(p.tree, ID::preFilterAmt.toString(), "Pre-filter");
  sInDiff1.attach(p.tree, ID::inputDiff1Amt.toString(), "Input diff. 1");
  sInDiff2.attach(p.tree, ID::inputDiff2Amt.toString(), "Input diff. 2");
  sDecayDiff.attach(p.tree, ID::decayDiff1Amt.toString(), "Decay diff.");
  sDamping.attach(p.tree, ID::dampingAmt.toString(), "Damping");
  sDecay.attach(p.tree, ID::decayAmt.toString(), "Decay");
  sWetDry.attach(p.tree, ID::wetDry.toString(), "Wet/dry");
  setSize(800, 600);
}

DattorroVerbProcessorEditor::~DattorroVerbProcessorEditor() {}

void DattorroVerbProcessorEditor::paint(juce::Graphics& g) {
  juce::ignoreUnused(g);
}

void DattorroVerbProcessorEditor::resized() {
  // just 2 rows of 4 knobs for now
  auto fBounds = getLocalBounds().toFloat();
  const float dX = fBounds.getWidth() / 4.0f;
  const float dY = fBounds.getHeight() / 2.0f;
  // top row left to right
  f_rect t1(0.0f, 0.0f, dX, dY);
  f_rect t2(dX, 0.0f, dX, dY);
  f_rect t3(dX * 2.0f, 0.0f, dX, dY);
  f_rect t4(dX * 3.0f, 0.0f, dX, dY);
  // bottom row
  f_rect b1(0.0f, dY, dX, dY);
  f_rect b2(dX, dY, dX, dY);
  f_rect b3(dX * 2.0f, dY, dX, dY);
  f_rect b4(dX * 3.0f, dY, dX, dY);

  sPreDelay.setBounds(t1.toNearestInt());
  sPreFilter.setBounds(t2.toNearestInt());
  sInDiff1.setBounds(t3.toNearestInt());
  sInDiff2.setBounds(t4.toNearestInt());
  sDecayDiff.setBounds(b1.toNearestInt());
  sDamping.setBounds(b2.toNearestInt());
  sDecay.setBounds(b3.toNearestInt());
  sWetDry.setBounds(b4.toNearestInt());
}
}  // namespace audio_plugin
