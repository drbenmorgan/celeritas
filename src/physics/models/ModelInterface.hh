//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file ModelInterface.hh
//---------------------------------------------------------------------------//
#ifndef physics_models_ModelInterface_hh
#define physics_models_ModelInterface_hh

#include <vector>

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Host interface for a physics model.
 */
class ModelInterface
{
  public:
    //@{
    //! Type aliases
    //@}

  public:
    virtual ~ModelInterface() = 0;

    //! Get the particles that this process applies to

    //! Get the cutoff energy for a particle type
    real_type low
};

//---------------------------------------------------------------------------//
} // namespace celeritas

#endif // physics_models_ModelInterface_hh
