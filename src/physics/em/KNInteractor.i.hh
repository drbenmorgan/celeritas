//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file KNInteractor.i.hh
//---------------------------------------------------------------------------//

#include "base/ArrayUtils.hh"
#include "base/Constants.hh"
#include "random/distributions/BernoulliDistribution.hh"
#include "random/distributions/GenerateCanonical.hh"
#include "random/distributions/UniformRealDistribution.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Construct with shared and state data.
 *
 * The incident particle must be above the energy threshold: this should be
 * handled in code *before* the interactor is constructed.
 */
KNInteractor::KNInteractor(const KNInteractorPointers& shared,
                           const ParticleTrackView&    particle,
                           const Real3&                inc_direction,
                           SecondaryAllocatorView&     allocate)
    : shared_(shared)
    , inc_energy_(particle.energy())
    , inc_direction_(inc_direction)
    , allocate_(allocate)
{
    REQUIRE(inc_energy_ >= this->min_incident_energy());
    REQUIRE(particle.def_id() == shared_.gamma_id);
}

//---------------------------------------------------------------------------//
/*!
 * Sample Compton scattering using the Klein-Nishina model.
 *
 * See section 6.4.2 of the Geant physics reference. Epsilon is the ratio of
 * outgoing to incident gamma energy, bounded in [epsilon_0, 1].
 */
template<class Engine>
CELER_FUNCTION Interaction KNInteractor::operator()(Engine& rng)
{
    // Allocate space for the single electron to be emitted
    Secondary* electron_secondary = this->allocate_(1);
    if (electron_secondary == nullptr)
    {
        // Failed to allocate space for a secondary
        return Interaction::from_failure();
    }

    // Value of epsilon corresponding to minimum photon energy
    const real_type inc_energy_per_mecsq = inc_energy_
                                           * shared_.inv_electron_mass_csq;
    const real_type epsilon_0     = 1 / (1 + 2 * inc_energy_per_mecsq);
    const real_type log_epsilon_0 = std::log(epsilon_0);

    // Probability of alpha_1 to choose f1 (sample epsilon)
    BernoulliDistribution choose_f1(-log_epsilon_0,
                                    0.5 * (1 - epsilon_0 * epsilon_0));
    // Sample square of f_2(\eps) \propto \eps on [\eps_0, 1]
    UniformRealDistribution<real_type> sample_f2_sq(epsilon_0 * epsilon_0, 1);

    // Rejection loop: sample epsilon (energy change) and direction change
    real_type epsilon;
    real_type one_minus_costheta;
    // Temporary sample values used in rejection
    real_type acceptance_prob;
    int       rejection_failure_countdown = this->max_rejection_loops();
    do
    {
        // Abort if too many sampling failures
        if (CELER_UNLIKELY(--rejection_failure_countdown < 0))
        {
            // Mark secondary as a failure
            *electron_secondary = Secondary::from_failure();
            // Exit so the next kernel launch can try again (?)
            return Interaction::from_failure();
        }

        // Sample epsilon and square
        real_type epsilon_sq;
        if (choose_f1(rng))
        {
            // Sample f_1(\eps) \propto 1/\eps on [\eps_0, 1]
            // => \eps \gets \eps_0^\xi = \exp(\xi \log \eps_0)
            epsilon    = std::exp(log_epsilon_0 * generate_canonical(rng));
            epsilon_sq = epsilon * epsilon;
        }
        else
        {
            // Sample f_2(\eps) = 2 * \eps / (1 - epsilon_0 * epsilon_0)
            epsilon_sq = sample_f2_sq(rng);
            epsilon    = std::sqrt(epsilon_sq);
        }
        CHECK(epsilon >= epsilon_0 && epsilon <= 1);

        // Calculate angles: need sin^2 \theta for rejection
        one_minus_costheta = (1 - epsilon) / (epsilon * inc_energy_per_mecsq);
        CHECK(one_minus_costheta >= 0 && one_minus_costheta <= 2);
        real_type sintheta_sq = one_minus_costheta * (2 - one_minus_costheta);
        acceptance_prob       = epsilon * sintheta_sq / (1 + epsilon_sq);
    } while (BernoulliDistribution(acceptance_prob)(rng));

    // Construct interaction for change to primary (incident) particle
    Interaction result;
    result.action      = Action::scatter;
    result.energy      = epsilon * inc_energy_;
    result.direction   = inc_direction_;
    result.secondaries = {electron_secondary, 1};

    // Sample azimuthal direction and rotate the outgoing direction
    UniformRealDistribution<real_type> sample_phi(0, constants::two_pi);
    result.direction
        = rotate(from_spherical(1 - one_minus_costheta, sample_phi(rng)),
                 result.direction);

    // Outgoing secondary is an electron
    electron_secondary->def_id = shared_.electron_id;
    // Construct secondary energy by neglecting electron binding energy
    electron_secondary->energy = inc_energy_ - result.energy;
    // Direction = gamma direction * gamma inc - gamma exit dir * gamma exit
    // (conservation of momentum)
    for (int i = 0; i < 3; ++i)
    {
        electron_secondary->direction[i] = inc_direction_[i] * inc_energy_
                                           - result.direction[i]
                                                 * result.energy;
    }
    normalize_direction(&electron_secondary->direction);

    // Cutoff for secondary production happens *after* the interaction
    // code.
    return result;
}

//---------------------------------------------------------------------------//
} // namespace celeritas
