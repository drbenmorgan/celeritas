//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RunAction.cc
//---------------------------------------------------------------------------//

#include "RunAction.hh"

//---------------------------------------------------------------------------//
/*!
 * Constructor and destructor
 */
RunAction::RunAction() : G4UserRunAction() {}
RunAction::~RunAction() {}

//---------------------------------------------------------------------------//
void RunAction::BeginOfRunAction(const G4Run* /*run*/) {}

//---------------------------------------------------------------------------//
void RunAction::EndOfRunAction(const G4Run* /*run*/) {}
