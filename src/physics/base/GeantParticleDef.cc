//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file G4ParticleDef.cc
//---------------------------------------------------------------------------//
#include "G4ParticleDef.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Construct with defaults
 */
G4ParticleDef::G4ParticleDef()
    : name_("empty")
    , pdg_(0)
    , mass_(0)
    , charge_(0)
    , spin_(0)
    , lifetime_(0)
    , isStable_(0)
{
}

//---------------------------------------------------------------------------//
/*!
 * Construct with user-defined values
 */
G4ParticleDef::G4ParticleDef(std::string name,
                             int         pdg,
                             double      mass,
                             double      charge,
                             double      spin,
                             double      lifetime,
                             bool        isStable)
    : name_(name)
    , pdg_(pdg)
    , mass_(mass)
    , charge_(charge)
    , spin_(spin)
    , lifetime_(lifetime)
    , isStable_(isStable)
{
}

//---------------------------------------------------------------------------//
/*!
 * Destructor
 */
G4ParticleDef::~G4ParticleDef() {}

//---------------------------------------------------------------------------//
/*!
 * Operator()
 */
G4ParticleDef G4ParticleDef::operator()(std::string name,
                                        int         pdg,
                                        double      mass,
                                        double      charge,
                                        double      spin,
                                        double      lifetime,
                                        bool        isStable)
{
    this->name_     = name;
    this->pdg_      = pdg;
    this->mass_     = mass;
    this->charge_   = charge;
    this->spin_     = spin;
    this->lifetime_ = lifetime;
    this->isStable_ = isStable;

    return *this;
}

//---------------------------------------------------------------------------//
/*!
 * Getters for all G4ParticleDef class variables
 */
std::string G4ParticleDef::name()
{
    return this->name_;
}

int G4ParticleDef::pdg()
{
    return this->pdg_;
}

double G4ParticleDef::mass()
{
    return this->mass_;
}

double G4ParticleDef::charge()
{
    return this->charge_;
}

double G4ParticleDef::spin()
{
    return this->spin_;
}

double G4ParticleDef::lifetime()
{
    return this->lifetime_;
}

bool G4ParticleDef::isStable()
{
    return this->isStable_;
}

//---------------------------------------------------------------------------//
} // namespace celeritas
