//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file PrimaryGeneratorAction.cc
//---------------------------------------------------------------------------//

#include "PrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

//---------------------------------------------------------------------------//
//! Constructor creates a working particle gun for the Geant4 minimal run
PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(), particleGun_(nullptr)
{
    // Creating the particle gun
    G4int numberOfParticles = 1;
    particleGun_ = new G4ParticleGun(numberOfParticles);
    
    G4ParticleDefinition* particle;
    particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    
    // cms2018.gdml center is at (0, 0, 0)
    G4ThreeVector pos(0, 0, 0);
    
    particleGun_->SetParticleDefinition(particle);
    particleGun_->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    particleGun_->SetParticleEnergy(10 * GeV);
    particleGun_->SetParticlePosition(pos);
}

//---------------------------------------------------------------------------//
//! Destructor deletes up the particle gun pointer
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete particleGun_;
}


//---------------------------------------------------------------------------//
//! Called at the begining of each event
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    particleGun_->GeneratePrimaryVertex(event);
}


//---------------------------------------------------------------------------//
//! Particle gun getter
G4ParticleGun* PrimaryGeneratorAction::particleGun()
{
    return particleGun_;
}
