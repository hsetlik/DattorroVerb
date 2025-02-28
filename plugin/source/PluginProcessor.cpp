#include "DattorroVerb/PluginProcessor.h"

#include "DattorroVerb/Identifiers.h"
#include "DattorroVerb/PluginEditor.h"
namespace audio_plugin {
DattorroVerbAudioProcessor::DattorroVerbAudioProcessor()
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
      tree(*this, nullptr, ID::DattorroVerb_state, ID::getParameterLayout()) {
}

DattorroVerbAudioProcessor::~DattorroVerbAudioProcessor() {}

const juce::String DattorroVerbAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool DattorroVerbAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool DattorroVerbAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool DattorroVerbAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;

#endif
}

double DattorroVerbAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int DattorroVerbAudioProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
  // programs, so this should be at least 1, even if you're not
  // really implementing programs.
}

int DattorroVerbAudioProcessor::getCurrentProgram() {
  return 0;
}

void DattorroVerbAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String DattorroVerbAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);

  return {};
}

void DattorroVerbAudioProcessor::changeProgramName(
    int index,
    const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void DattorroVerbAudioProcessor::prepareToPlay(double sampleRate,
                                               int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  SampleRate::set(sampleRate);
  verb.init(sampleRate);
  juce::ignoreUnused(samplesPerBlock);
}

void DattorroVerbAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

bool DattorroVerbAudioProcessor::isBusesLayoutSupported(
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

void DattorroVerbAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages) {
  juce::ignoreUnused(midiMessages);
  juce::ScopedNoDenormals noDenormals;
  auto totalNumOutputChannels = getTotalNumOutputChannels();
  const int length = buffer.getNumSamples();
  if (totalNumOutputChannels > 1) {  // stereo mode
    float* lPtr = buffer.getWritePointer(0);
    float* rPtr = buffer.getWritePointer(1);
    verb.processStereo(lPtr, rPtr, length, getTotalNumInputChannels() < 2);
  } else {  // mono mode
    float* lPtr = buffer.getWritePointer(0);
    verb.processMono(lPtr, length);
  }
}

bool DattorroVerbAudioProcessor::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DattorroVerbAudioProcessor::createEditor() {
  return new DattorroVerbProcessorEditor(*this);
}

void DattorroVerbAudioProcessor::getStateInformation(

    juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  juce::ignoreUnused(destData);
}

void DattorroVerbAudioProcessor::setStateInformation(const void* data,
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
  return new audio_plugin::DattorroVerbAudioProcessor();
}
