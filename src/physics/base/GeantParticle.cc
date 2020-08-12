//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantParticle.cc
//---------------------------------------------------------------------------//
#include "GeantParticle.hh"

#include <iostream>

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Construct with defaults
 */
GeantParticle::GeantParticle()
    : name_("empty")
    , pdg_(0)
    , mass_(0)
    , charge_(0)
    , spin_(0)
    , lifetime_(0)
    , is_stable_(0)
{
}

//---------------------------------------------------------------------------//
/*!
 * Construct with user-defined values
 */
GeantParticle::GeantParticle(std::string name,
                             ssize_type  pdg,
                             real_type   mass,
                             real_type   charge,
                             real_type   spin,
                             real_type   lifetime,
                             bool        is_stable)
    : name_(name)
    , pdg_(pdg)
    , mass_(mass)
    , charge_(charge)
    , spin_(spin)
    , lifetime_(lifetime)
    , is_stable_(is_stable)
{
}

//---------------------------------------------------------------------------//
/*!
 * Destructor
 */
GeantParticle::~GeantParticle() {}

//---------------------------------------------------------------------------//
/*!
 * Operator()
 */
GeantParticle GeantParticle::operator()(std::string name,
                                        ssize_type  pdg,
                                        real_type   mass,
                                        real_type   charge,
                                        real_type   spin,
                                        real_type   lifetime,
                                        bool        is_stable)
{
    this->name_      = name;
    this->pdg_       = pdg;
    this->mass_      = mass;
    this->charge_    = charge;
    this->spin_      = spin;
    this->lifetime_  = lifetime;
    this->is_stable_ = is_stable;

    return *this;
}

//---------------------------------------------------------------------------//
/*!
 * Getters for all protected members
 */
std::string GeantParticle::name()
{
    return this->name_;
}

int GeantParticle::pdg()
{
    return this->pdg_;
}

double GeantParticle::mass()
{
    return this->mass_;
}

double GeantParticle::charge()
{
    return this->charge_;
}

double GeantParticle::spin()
{
    return this->spin_;
}

double GeantParticle::lifetime()
{
    return this->lifetime_;
}

bool GeantParticle::is_stable()
{
    return this->is_stable_;
}

//---------------------------------------------------------------------------//
/*!
 * Prints the data of a GeantParticle particle
 */
void GeantParticle::print()
{
    std::cout << "--------------------------" << std::endl;
    std::cout << this->name() << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << "pdg          : " << this->pdg() << std::endl;
    std::cout << "mass [MeV]   : " << this->mass() << std::endl;
    std::cout << "charge       : " << this->charge() << std::endl;
    std::cout << "spin         : " << this->spin() << std::endl;
    std::cout << "lifetime [s] : " << this->lifetime() << std::endl;
    std::cout << "is_stable    : " << this->is_stable() << std::endl;
    std::cout << "--------------------------" << std::endl;
}

//---------------------------------------------------------------------------//
} // namespace celeritas
