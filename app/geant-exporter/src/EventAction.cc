//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file EventAction.cc
//---------------------------------------------------------------------------//

#include "EventAction.hh"

//---------------------------------------------------------------------------//
//! Default constructor and destructor
EventAction::EventAction() : G4UserEventAction() {}

EventAction::~EventAction() {}

//---------------------------------------------------------------------------//
void EventAction::BeginOfEventAction(const G4Event* /*event*/) {}

//---------------------------------------------------------------------------//
void EventAction::EndOfEventAction(const G4Event* /*event*/) {}
