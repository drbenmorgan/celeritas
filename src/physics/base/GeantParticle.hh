//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantParticleDef.hh
//---------------------------------------------------------------------------//
#pragma once

#include <string>

#include "base/Types.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * /brief Stores particle information for the GeantImporter class
 *
 * This class provides the same structure seen in the particle.root file
 * created by the app/geant-exporter external code and it is used to load
 * said ROOT data into memory by the GeantImporter class.
 */

class GeantParticle
{
  protected:
    std::string name_;
    ssize_type  pdg_;
    real_type   mass_;
    real_type   charge_;
    real_type   spin_;
    real_type   lifetime_;
    bool        is_stable_;

  public:
    GeantParticle();
    GeantParticle(std::string name,
                  ssize_type  pdg,
                  real_type   mass,
                  real_type   charge,
                  real_type   spin,
                  real_type   lifetime,
                  bool        is_stable);

    ~GeantParticle();

    GeantParticle operator()(std::string name,
                             ssize_type  pdg,
                             real_type   mass,
                             real_type   charge,
                             real_type   spin,
                             real_type   lifetime,
                             bool        is_stable);

    std::string name();
    ssize_type  pdg();
    real_type   mass();
    real_type   charge();
    real_type   spin();
    real_type   lifetime();
    bool        is_stable();

    void print();
};

//---------------------------------------------------------------------------//
} // namespace celeritas
