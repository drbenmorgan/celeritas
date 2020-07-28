//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantParticleDef.hh
//---------------------------------------------------------------------------//
#pragma once

#include <string>

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Used by GeantImporter to store data from Geant4's G4ParticleDefinition class
 */

class GeantParticleDef
{
  protected:
    std::string name_;
    int         pdg_;
    double      mass_;
    double      charge_;
    double      spin_;
    double      lifetime_;
    bool        isStable_;

  public:
    GeantParticleDef();
    GeantParticleDef(std::string name,
                     int         pdg,
                     double      mass,
                     double      charge,
                     double      spin,
                     double      lifetime,
                     bool        isStable);
    
    ~GeantParticleDef();
    
    GeantParticleDef operator()(std::string name,
                                int         pdg,
                                double      mass,
                                double      charge,
                                double      spin,
                                double      lifetime,
                                bool        isStable);

    std::string name();
    int         pdg();
    double      mass();
    double      charge();
    double      spin();
    double      lifetime();
    bool        isStable();
};

//---------------------------------------------------------------------------//
} // namespace celeritas
