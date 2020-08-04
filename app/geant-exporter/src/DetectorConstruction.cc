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
    : phys_vol_world_(0)
{
    this->gdml_parser_.Read(gdmlInput);
    this->phys_vol_world_ = this->gdml_parser_.GetWorldVolume();
}

//---------------------------------------------------------------------------//
G4VPhysicalVolume* DetectorConstruction::Construct()
{
    return phys_vol_world_;
}
