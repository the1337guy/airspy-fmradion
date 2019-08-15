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

#ifndef SOFTFM_IFAGC_H
#define SOFTFM_IFAGC_H

#include <cstdint>
#include <vector>

#include "SoftFM.h"
#include "util.h"

// Class IfAgc

class IfAgc {
public:
  // Construct IF AGC.
  // initial_gain :: Initial gain value.
  // max_gain     :: Maximum gain value.
  // reference    :: target output level.
  // rate         :: rate factor for changing the gain value.
  IfAgc(const double initial_gain, const double max_gain,
        const double reference, const double rate);

  // Process IQ samples.
  void process(const IQSampleVector &samples_in, IQSampleVector &samples_out);

  // Return IF AGC current gain.
  double get_current_gain() const { return m_current_gain; }

private:
  double m_current_gain;
  double m_max_gain;
  double m_reference;
  double m_rate;
};

#endif