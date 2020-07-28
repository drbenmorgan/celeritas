//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DetectorConstruction.cc
//---------------------------------------------------------------------------//


#include "DetectorConstruction.hh"


//---------------------------------------------------------------------------//
//! Constructs with a given gdml input file
DetectorConstruction::DetectorConstruction(G4String gdmlInput)
: physVolWorld_(0)
{
    this->gdmlParser_.Read(gdmlInput);
    this->physVolWorld_ = this->gdmlParser_.GetWorldVolume();
}

//---------------------------------------------------------------------------//
G4VPhysicalVolume* DetectorConstruction::Construct()
{
    return physVolWorld_;
}
