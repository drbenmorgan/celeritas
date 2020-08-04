//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantParticleDef.cc
//---------------------------------------------------------------------------//
#include "GeantParticleDef.hh"

#include <iostream>

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Construct with defaults
 */ 
GeantParticleDef::GeantParticleDef()
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
GeantParticleDef::GeantParticleDef(std::string name,
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
GeantParticleDef::~GeantParticleDef() {}

//---------------------------------------------------------------------------//
/*!
 * Operator()
 */
GeantParticleDef GeantParticleDef::operator()(std::string name,
                                              ssize_type  pdg,
                                              real_type   mass,
                                              real_type   charge,
                                              real_type   spin,
                                              real_type   lifetime,
                                              bool        is_stable)
{
    this->name_     = name;
    this->pdg_      = pdg;
    this->mass_     = mass;
    this->charge_   = charge;
    this->spin_     = spin;
    this->lifetime_ = lifetime;
    this->is_stable_ = is_stable;

    return *this;
}

//---------------------------------------------------------------------------//
/*!
 * Getters for all protected members
 */
std::string GeantParticleDef::name()
{
    return this->name_;
}

int GeantParticleDef::pdg()
{
    return this->pdg_;
}

double GeantParticleDef::mass()
{
    return this->mass_;
}

double GeantParticleDef::charge()
{
    return this->charge_;
}

double GeantParticleDef::spin()
{
    return this->spin_;
}

double GeantParticleDef::lifetime()
{
    return this->lifetime_;
}

bool GeantParticleDef::is_stable()
{
    return this->is_stable_;
}

//---------------------------------------------------------------------------//
/*!
 * Prints the data of a GeantParticleDef particle
 */
void GeantParticleDef::print()
{
    std::cout << "-----------------------" << std::endl;
    std::cout << this->name() << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << "pdg       : " << this->pdg() << std::endl;
    std::cout << "mass      : " << this->mass() << std::endl;
    std::cout << "charge    : " << this->charge() << std::endl;
    std::cout << "spin      : " << this->spin() << std::endl;
    std::cout << "lifetime  : " << this->lifetime() << std::endl;
    std::cout << "is_stable : " << this->is_stable() << std::endl;
    std::cout << "-----------------------" << std::endl;
}

//---------------------------------------------------------------------------//
} // namespace celeritas