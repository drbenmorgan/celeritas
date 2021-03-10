//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file BremSbInterface.hh
//---------------------------------------------------------------------------//
#pragma once

#include "base/Collection.hh"
#include "physics/base/Units.hh"
#include "Types.hh"
#include "physics/base/Types.hh"
#include "physics/material/Types.hh"

namespace celeritas
{
struct BremSbData
{
    int atomic_number;

    struct GridData
    {
        int max_atomic_number;
        int num_electron_energies;
        int num_kappa_values;
    };

    struct KappaTableData
    {
        real_type cumulative;
        real_type parameter_a;
        real_type parameter_b;
    };

    using KappaTablePerEnergy = std::vector<KappaTableData>;

    GridData               grid_;
    std::vector<real_type> first_e_energy_vector_;
    std::vector<real_type> first_e_log_energy_vector_;
    std::vector<real_type> kappa_vector_;
    std::vector<real_type> log_kappa_vector_;
};

//---------------------------------------------------------------------------//
/*!
 * Persistent shared Bremmsthralung Seltzer-Berger data.
 *
 * Data is retrieved from ASCII and Z files stored in $G4LEDATA.
 */
template<Ownership W, MemSpace M>
struct BremSbParamsData
{
    template<class T>
    using Data = Collection<T, W, M>;

    // Backend storage
    Data<BremSbData> brem_sb_tables; // [num_elements]

    ElementId::size_type num_elements;

    //// MEMBER FUNCTIONS ////

    //! True if assigned
    explicit CELER_FUNCTION operator bool() const
    {
        return brem_sb_tables.size() == num_elements && !brem_sb_tables.empty();
    }

    //! Assign from another set of data
    template<Ownership W2, MemSpace M2>
    BremSbParamsData& operator=(const BremSbParamsData<W2, M2>& other)
    {
        CELER_EXPECT(other);

        this->brem_sb_tables = other.brem_sb_tables;
        this->num_elements   = other.num_elements;

        return *this;
    }
};

//---------------------------------------------------------------------------//
} // namespace celeritas
