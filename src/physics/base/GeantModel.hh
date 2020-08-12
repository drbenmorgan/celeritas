//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantModel.hh
//---------------------------------------------------------------------------//
#pragma once

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * /brief Enumerator for physics models
 *
 * This enum does not exists in Geant4. It was created to safely access
 * the many physics tables imported from it.
 */
enum class GeantModel
{
    not_defined,
    BraggIon,
    BetheBloch,
    UrbanMsc,
    ICRU73QO,
    WentzelVIUni,
    hBrem,
    hPairProd,
    eCoulombScattering,
    Bragg,
    MollerBhabha,
    eBremSB,
    eBremLPM,
    LivermorePhElectric,
    KleinNishina,
    BetheHeitlerLPM,
    LivermoreRayleigh,
    MuBetheBloch,
    MuBrem,
    muPairProd
};

//---------------------------------------------------------------------------//
} // namespace celeritas