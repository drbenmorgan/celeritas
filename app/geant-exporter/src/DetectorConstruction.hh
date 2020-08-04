//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file DetectorConstruction.hh
//---------------------------------------------------------------------------//

#pragma once

// Geant4
#include "G4VUserDetectorConstruction.hh"
#include "G4GDMLParser.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction(G4String gdmlInput);
    virtual G4VPhysicalVolume* Construct();

  private:
    G4GDMLParser       gdml_parser_;
    G4VPhysicalVolume* phys_vol_world_;
};
