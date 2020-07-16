//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file G4ParticleDef.hh
//---------------------------------------------------------------------------//
#ifndef base_G4ParticleDef_hh
#define base_G4ParticleDef_hh

#include <string>

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Used by G4Importer to store data from Geant4's G4ParticleDefinition class.
 */

class G4ParticleDef
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
    G4ParticleDef();
    G4ParticleDef(std::string name,
                  int         pdg,
                  double      mass,
                  double      charge,
                  double      spin,
                  double      lifetime,
                  bool        isStable);

    ~G4ParticleDef();

    G4ParticleDef operator()(std::string name,
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

#endif // base_G4ParticleDef_hh
