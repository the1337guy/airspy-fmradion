// airspy-fmradion
// Software decoder for FM broadcast radio with Airspy
//
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

#ifndef SOFTFM_AUDIORESAMPLER_H
#define SOFTFM_AUDIORESAMPLER_H

#include <cstdint>
#include <vector>

#include "SoftFM.h"

#include "soxr.h"

class AudioResampler {
public:
  AudioResampler(const double input_rate);
  void process(const SampleVector &samples_in, SampleVector &samples_out);

private:
  const double m_irate;
  soxr_t m_soxr;
};

#endif
