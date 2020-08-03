//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file KNInteractor.hh
//---------------------------------------------------------------------------//
#pragma once

#include "base/Macros.hh"
#include "base/Types.hh"
#include "physics/base/Interaction.hh"
#include "physics/base/ParticleTrackView.hh"
#include "physics/base/SecondaryAllocatorView.hh"
#include "physics/base/Secondary.hh"
#include "physics/base/Units.hh"
#include "KNInteractorPointers.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Perform Compton scattering, neglecting atomic binding energy.
 *
 * This is a model for the discrete Compton inelastic scattering process. Given
 * an incident gamma, it adds a single secondary (electron) to the secondary
 * stack and returns an interaction for the change to the incident gamma
 * direction and energy. No cutoffs are performed for the incident energy or
 * the exiting gamma energy.
 *
 * The rejection rate increases with particles of lower energy. With a "max
 * rejection loops" of 8 for an incident gamma at 10 keV (the lower cutoff),
 * the rejection fraction is about 1 in 10000.
 *
 * \note This performs the same sampling routine as in Geant4's
 *  G4KleinNishinaCompton, as documented in section 6.4.2 of the Geant4 Physics
 *  Reference (release 10.6).
 */
class KNInteractor
{
  public:
    // Construct sampler from shared and state data
    inline CELER_FUNCTION KNInteractor(const KNInteractorPointers& shared,
                                       const ParticleTrackView&    particle,
                                       const Real3&            inc_direction,
                                       SecondaryAllocatorView& allocate);

    // Sample an interaction with the given RNG
    template<class Engine>
    inline CELER_FUNCTION Interaction operator()(Engine& rng);

    // >>> COMMON PROPERTIES

    //! Minimum incident energy for this model to be valid
    //! TODO: this isn't currently used.
    static CELER_CONSTEXPR_FUNCTION real_type min_incident_energy()
    {
        return 10 * units::kilo_electron_volt;
    }

    // Maximum loops to allow over the rejection function
    static CELER_CONSTEXPR_FUNCTION int max_rejection_loops() { return 256; }

  private:
    // Gamma energy divided by electron mass * csquared
    const KNInteractorPointers& shared_;
    // Incident gamma energy
    const real_type inc_energy_;
    // Incident direction
    const Real3& inc_direction_;
    // Allocate space for a secondary particle
    SecondaryAllocatorView& allocate_;
};

//---------------------------------------------------------------------------//
} // namespace celeritas

#include "KNInteractor.i.hh"
