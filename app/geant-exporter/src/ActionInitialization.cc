//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file ActionInitialization.cc
//---------------------------------------------------------------------------//


#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"


//---------------------------------------------------------------------------//
//! Default constructor ad destructor
//! This class invokes all the remaining classes
ActionInitialization::ActionInitialization()
: G4VUserActionInitialization()
{
}

ActionInitialization::~ActionInitialization()
{
}

//---------------------------------------------------------------------------//
//! Constructs and invokes all other Geant4 related classes
void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGeneratorAction());
    SetUserAction(new RunAction());
    SetUserAction(new EventAction());
    SetUserAction(new SteppingAction());
}
