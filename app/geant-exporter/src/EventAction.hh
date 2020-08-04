//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EventAction.hh
//---------------------------------------------------------------------------//

#pragma once

// Geant4
#include "G4Event.hh"
#include "G4UserEventAction.hh"

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* /*event*/);
    virtual void EndOfEventAction(const G4Event* /*event*/);

    G4ThreeVector RandomUnitaryThreeVector();
};
