//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantProcess.hh
//---------------------------------------------------------------------------//
#pragma once

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * /brief Enumerator for the physics processes
 *
 * This enum does not exists in Geant4. It was created to safely access
 * the many physics tables imported from it.
 * 
 * This list only includes EM models, whilst the process type already has
 * all processes available in Geant. This enum will have to be expanded.
 */
enum class GeantProcess
{
    not_defined,
    ionIoni,
    msc,
    hIoni,
    hBrems,
    hPairProd,
    CoulombScat,
    eIoni,
    eBrem,
    phot,
    compt,
    conv,
    Rayl,
    muIoni,
    muBrems,
    muPairProd
};

//---------------------------------------------------------------------------//
} // namespace celeritas