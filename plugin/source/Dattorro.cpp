#include "DattorroReverbEX/Dattorro.h"
#include "DattorroReverbEX/DelayLine.h"
#include "DattorroReverbEX/Identifiers.h"

// filter helpers
static float LP_process(float* out, float in, float freq) {
  *out += (in - *out) * freq;
  return *out;
}
static float AP_process(_DelayLine* dl, uint16_t cycle, float gain, float in) {
  float delayed = dl->read(0, cycle);
  in += delayed * -gain;
  dl->write(cycle, in);
  return delayed + in * gain;
}
//================================================================
Dattorro::Dattorro() : preDelay(maxPreDelay) {
  // set up the OwnedArrays
  // input diffusion first
  inputDiffusion.add(new _DelayLine(142));
  inputDiffusion.add(new _DelayLine(107));
  inputDiffusion.add(new _DelayLine(379));
  inputDiffusion.add(new _DelayLine(277));
  // left tank
  decayDiffusion1.add(new _DelayLine(672));
  preDampingDelay.add(new _DelayLine(4453));
  preDampingDelay[0]->setDelay(1, 353);
  preDampingDelay[0]->setDelay(2, 3627);
  preDampingDelay[0]->setDelay(3, 1990);

  decayDiffusion2.add(new _DelayLine(1800));
  decayDiffusion2[0]->setDelay(1, 187);
  decayDiffusion2[0]->setDelay(2, 1228);

  postDampingDelay.add(new _DelayLine(3720));
  postDampingDelay[0]->setDelay(1, 1066);
  postDampingDelay[0]->setDelay(2, 2673);

  // right tank
  decayDiffusion1.add(new _DelayLine(908));
  preDampingDelay.add(new _DelayLine(4217));
  preDampingDelay[1]->setDelay(1, 266);
  preDampingDelay[1]->setDelay(2, 2974);
  preDampingDelay[1]->setDelay(3, 2111);

  decayDiffusion2.add(new _DelayLine(2656));
  decayDiffusion2[1]->setDelay(1, 335);
  decayDiffusion2[1]->setDelay(2, 1913);

  postDampingDelay.add(new _DelayLine(3163));
  postDampingDelay[1]->setDelay(1, 121);
  postDampingDelay[1]->setDelay(2, 1996);

  preDelay.setDelay(0, maxPreDelay / 10);
  preFilterAmt = 0.85f;
  inputDiff1Amt = 0.75f;
  inputDiff2Amt = 0.625f;
  decayAmt = 0.75f;
  decayDiff1Amt = 0.70f;
  decayDiff2Amt = std::clamp(decayAmt + 0.5f, 0.25f, 0.5f);
  dampingAmt = 0.95f;
  wetDry = 0.85f;
}

void Dattorro::init(double sampleRate) {
  // the max predelay should be
  // about 1/10th the sample rate
  maxPreDelay = (uint16_t)(sampleRate / 10.0);
  preDelay.setDelay(0, maxPreDelay / 10);
  preFilterAmt = 0.85f;
  inputDiff1Amt = 0.75f;
  inputDiff2Amt = 0.625f;
  decayAmt = 0.75f;
  decayDiff1Amt = 0.70f;
  decayDiff2Amt = std::clamp(decayAmt + 0.5f, 0.25f, 0.5f);
  dampingAmt = 0.95f;
  wetDry = 0.85f;
}

void Dattorro::updateParams(apvts& tree) {
  // 1. get the values in a thread-safe way
  const float preFilter_n =
      *tree.getRawParameterValue(ID::preFilterAmt.toString());
  const float preDelay_n =
      *tree.getRawParameterValue(ID::preDelayAmt.toString());
  const float id1_n = *tree.getRawParameterValue(ID::inputDiff1Amt.toString());
  const float id2_n = *tree.getRawParameterValue(ID::inputDiff2Amt.toString());
  const float decayDiff_n =
      *tree.getRawParameterValue(ID::decayDiff1Amt.toString());
  const float decay_n = *tree.getRawParameterValue(ID::decayAmt.toString());
  const float damping_n = *tree.getRawParameterValue(ID::dampingAmt.toString());
  const float wetDry_n = *tree.getRawParameterValue(ID::wetDry.toString());
  // 2. assign those values to the member variables
  preFilterAmt = preFilter_n;
  inputDiff1Amt = id1_n;
  inputDiff2Amt = id2_n;
  decayDiff1Amt = decayDiff_n;
  decayAmt = decay_n;
  decayDiff2Amt = std::clamp(decay_n + 0.15f, 0.25f, 0.5f);
  dampingAmt = damping_n;
  wetDry = wetDry_n;

  uint16_t preDelaySamples = (uint16_t)(preDelay_n * (float)maxPreDelay);
  preDelay.setDelay(0, preDelaySamples);
}

//--------------------------------------------------
// this function does the work for the common
// pre-delay and input diffusion sections.
// the 'tanks' are handled in the getLeft/getRight methods
void Dattorro::processInput(float input) {
  // modulate decayDiffusion1 for both tanks
  if ((t & 0x07ff) == 0) {
    if (t < (1 << 15)) {
      decayDiffusion1[0]->offsets[0]--;
      decayDiffusion1[1]->offsets[0]--;
    } else {
      decayDiffusion1[0]->offsets[0]++;
      decayDiffusion1[1]->offsets[0]++;
    }
  }
  // pre-delay
  float x = preDelay.process(t, input);
  // pre-filter
  x = LP_process(&preFilter, x, preFilterAmt);
  // input diffusion
  x = AP_process(inputDiffusion[0], t, inputDiff1Amt, x);
  x = AP_process(inputDiffusion[1], t, inputDiff1Amt, x);
  x = AP_process(inputDiffusion[2], t, inputDiff2Amt, x);
  x = AP_process(inputDiffusion[3], t, inputDiff2Amt, x);
  // process the tanks
  float x1;
  for (uint16_t i = 0; i < 2; i++) {
    // cross feedback
    x1 = x + postDampingDelay[1 - i]->read(0, t) * decayAmt;
    // back to this half ot the tank
    x1 = AP_process(decayDiffusion1[i], t, decayDiff1Amt, x1);
    x1 = preDampingDelay[i]->process(t, x1);
    x1 = LP_process(&damping[i], x1, dampingAmt);
    x1 *= decayAmt;
    AP_process(decayDiffusion2[i], t, decayDiff2Amt, x1);
    postDampingDelay[i]->write(t, x1);
  }
  // increment t
  t++;
}

float Dattorro::getLeft() {
  float a = preDampingDelay[1]->read(1, t);
  a += preDampingDelay[1]->read(2, t);
  a -= decayDiffusion2[1]->read(2, t);
  a += postDampingDelay[1]->read(2, t);
  a -= preDampingDelay[0]->read(3, t);
  a -= decayDiffusion2[0]->read(1, t);
  a += postDampingDelay[0]->read(1, t);
  return a;
}

float Dattorro::getRight() {
  float a = preDampingDelay[0]->read(1, t);
  a += preDampingDelay[0]->read(2, t);
  a -= decayDiffusion2[0]->read(2, t);
  a += postDampingDelay[0]->read(2, t);
  a -= preDampingDelay[1]->read(3, t);
  a -= decayDiffusion2[1]->read(1, t);
  a += postDampingDelay[1]->read(1, t);
  return a;
}

// Main audio
// callbacks=============================================================

void Dattorro::processMono(float* buf, int numSamples) {
  for (int i = 0; i < numSamples; ++i) {
    processInput(buf[i]);
    buf[i] = (getLeft() + getRight()) / 2.0f;
  }
}

void Dattorro::processStereo(float* lBuf,
                             float* rBuf,
                             int numSamples,
                             bool inputIsMono) {
  if (inputIsMono) {
    for (int i = 0; i < numSamples; ++i) {
      processInput(lBuf[i]);
      lBuf[i] = getLeft();
      rBuf[i] = getRight();
    }
  } else {
    float input;
    for (int i = 0; i < numSamples; ++i) {
      input = (lBuf[i] + rBuf[i]) / 2.0f;
      processInput(input);
      lBuf[i] = getLeft();
      rBuf[i] = getRight();
    }
  }
}

void Dattorro::processBuffer(juce::AudioBuffer<float>& buf,
                             int inChannels,
                             int outChannels) {
#ifdef CLEAN_AUDIO_BUFFER
  for (int i = inChannels; i < outChannels; ++i)
    buf.clear(i, 0, buf.getNumSamples());
#endif

  if (outChannels < 2) {  // mono mode
    auto* data = buf.getWritePointer(0);
    for (int i = 0; i < buf.getNumSamples(); ++i) {
      processInput(data[i]);
      data[i] = (getLeft() + getRight()) / 2.0f;
    }
  } else if (inChannels < outChannels) {  // mono in/stereo out
    auto* in = buf.getReadPointer(0);
    auto* outL = buf.getWritePointer(0);
    auto* outR = buf.getWritePointer(1);
    for (int i = 0; i < buf.getNumSamples(); ++i) {
      processInput(in[i]);
      outL[i] = getLeft();
      outR[i] = getLeft();
    }
  } else {  // stereo in/stereo out
    auto* lBuf = buf.getWritePointer(0);
    auto* rBuf = buf.getWritePointer(1);
    float input;
    for (int i = 0; i < buf.getNumSamples(); ++i) {
      input = (lBuf[i] + rBuf[i]) * 0.5f;
      processInput(input);
      lBuf[i] = getLeft();
      rBuf[i] = getRight();
    }
  }
}
