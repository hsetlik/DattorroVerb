#include "DattorroReverbEX/PluginProcessor.h"

#include "DattorroReverbEX/Identifiers.h"
#include "DattorroReverbEX/PluginEditor.h"
namespace audio_plugin {
DattorroReverbEXAudioProcessor::DattorroReverbEXAudioProcessor()
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
      tree(*this,
           nullptr,
           ID::DattorroReverbEX_state,
           ID::getParameterLayout()) {
}

DattorroReverbEXAudioProcessor::~DattorroReverbEXAudioProcessor() {}

const juce::String DattorroReverbEXAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool DattorroReverbEXAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool DattorroReverbEXAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool DattorroReverbEXAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;

#endif
}

double DattorroReverbEXAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int DattorroReverbEXAudioProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
  // programs, so this should be at least 1, even if you're not
  // really implementing programs.
}

int DattorroReverbEXAudioProcessor::getCurrentProgram() {
  return 0;
}

void DattorroReverbEXAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String DattorroReverbEXAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);

  return {};
}

void DattorroReverbEXAudioProcessor::changeProgramName(
    int index,
    const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void DattorroReverbEXAudioProcessor::prepareToPlay(double sampleRate,
                                                   int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  SampleRate::set(sampleRate);
  verb.init(sampleRate);
  juce::ignoreUnused(samplesPerBlock);
}

void DattorroReverbEXAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

bool DattorroReverbEXAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;
  // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif
  return true;
#endif
}

void DattorroReverbEXAudioProcessor::processBlock(
    juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages) {
  juce::ignoreUnused(midiMessages);
  juce::ScopedNoDenormals noDenormals;
  verb.updateParams(tree);
  const int inChannels = getTotalNumOutputChannels();
  const int outChannels = getTotalNumInputChannels();
  verb.processBuffer(buffer, inChannels, outChannels);
}

bool DattorroReverbEXAudioProcessor::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DattorroReverbEXAudioProcessor::createEditor() {
  return new DattorroReverbEXProcessorEditor(*this);
}

void DattorroReverbEXAudioProcessor::getStateInformation(

    juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  juce::ignoreUnused(destData);
}

void DattorroReverbEXAudioProcessor::setStateInformation(const void* data,
                                                         int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  juce::ignoreUnused(data, sizeInBytes);
}

}  // namespace audio_plugin
// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new audio_plugin::DattorroReverbEXAudioProcessor();
}
