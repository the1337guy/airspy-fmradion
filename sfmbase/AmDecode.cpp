// airspy-fmradion
// Software decoder for FM broadcast radio with Airspy
//
// Copyright (C) 2015 Edouard Griffiths, F4EXB
// Copyright (C) 2019 Kenji Rikitake, JJ1BDX
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <cassert>
#include <cmath>

#include "AmDecode.h"

// class AmDecoder

// Static constants.

const double AmDecoder::sample_rate_pcm = 48000;
// Half bandwidth of audio signal in Hz (4.5kHz for AM)
const double AmDecoder::bandwidth_pcm = 4500;
// Deemphasis constant in microseconds.
const double AmDecoder::default_deemphasis = 100;

AmDecoder::AmDecoder(double sample_rate_demod)
    // Initialize member fields
    : m_sample_rate_demod(sample_rate_demod), m_baseband_mean(0),
      m_baseband_level(0), m_agc_peak1(0), m_agc_peak2(0),
      m_agc_reference(0.9), m_agc_last_gain(1.0)

      // Construct AudioResampler for mono and stereo channels
      ,
      m_audioresampler(m_sample_rate_demod / 4.0, sample_rate_pcm)

      // Construct IfDownsampler
      ,
      m_ifdownsampler(2, FilterParameters::jj1bdx_am_48khz_div2, true, 2,
                      FilterParameters::jj1bdx_am_24khz_div2)

      // Construct HighPassFilterIir
      // cutoff: 60Hz for 12kHz sampling rate
      ,
      m_dcblock(0.005)

      // Construct LowPassFilterRC
      ,
      m_deemph(default_deemphasis * sample_rate_pcm * 1.0e-6) {
  // Do nothing
}

void AmDecoder::process(const IQSampleVector &samples_in, SampleVector &audio) {

  // Downsample input signal to /4
  m_ifdownsampler.process(samples_in, m_buf_downsampled);

  // TODO: narrower filter here

  // Demodulate AM signal.
  demodulate(m_buf_downsampled, m_buf_baseband_demod);

  // DC blocking.
  m_dcblock.process_inplace(m_buf_baseband_demod);

  // Audio AGC
  audio_agc(m_buf_baseband_demod, m_buf_baseband);

  // TODO: AGC here

  // Measure baseband level after DC blocking.
  double baseband_mean, baseband_rms;
  samples_mean_rms(m_buf_baseband, baseband_mean, baseband_rms);
  m_baseband_mean = 0.95 * m_baseband_mean + 0.05 * baseband_mean;
  m_baseband_level = 0.95 * m_baseband_level + 0.05 * baseband_rms;

  // Upsample baseband signal.
  m_audioresampler.process(m_buf_baseband, m_buf_mono);

  // If no mono audio signal comes out, terminate and wait for next block,
  if (m_buf_mono.size() == 0) {
    audio.resize(0);
    return;
  }

  // Deemphasis
  m_deemph.process_inplace(m_buf_mono);

  // Return mono channel.
  audio = move(m_buf_mono);
}

// Demodulate AM signal.
inline void AmDecoder::demodulate(const IQSampleVector &samples_in,
                                  SampleVector &samples_out) {
  unsigned int n = samples_in.size();
  samples_out.resize(n);

  for (unsigned int i = 0; i < n; i++) {
    samples_out[i] = std::abs(samples_in[i]);
  }
}

// Audio AGC.
// Algorithm: function fastagc_ff() in
// https://github.com/simonyiszk/csdr/blob/master/libcsdr.c
inline void AmDecoder::audio_agc(const SampleVector &samples_in,
                                  SampleVector &samples_out) {
  const double agc_max_gain = 50;
  unsigned int n = samples_in.size();
  samples_out.resize(n);
  m_agc_buf1.resize(n);
  m_agc_buf2.resize(n);

  double agc_peak = 0;
  for (unsigned int i = 0; i < n; i++) {
    double v = fabs(samples_in[i]);
    if (v > agc_peak) {
      agc_peak = v;
    }
  }

  double target_peak = agc_peak;
  if (target_peak < m_agc_peak2) {
    target_peak = m_agc_peak2;
  }
  if (target_peak < m_agc_peak1) {
    target_peak = m_agc_peak1;
  }

  double target_gain = m_agc_reference / target_peak;
  if (target_gain > agc_max_gain) {
    target_gain = agc_max_gain;
  }

  for (unsigned int i = 0; i < n; i++) {
     double rate = (double)i / (double)n;
     double gain = (m_agc_last_gain * (1.0 - rate)) + (target_gain * rate);
     samples_out[i] = m_agc_buf1[i] * gain;
  }

  m_agc_buf1 = m_agc_buf2;
  m_agc_peak1 = m_agc_peak2;
  m_agc_buf2 = samples_in;
  m_agc_peak2 = agc_peak;
  m_agc_last_gain = target_gain;

}
// end
