
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

#include "PrimaryGeneratorAction.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
      fParticleGun(nullptr)
{
 
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    // Set default particle
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle("mu-");

    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(1.0 * TeV);  // Set energy (example: 1 TeV)

    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, -10.*cm));  // Set initial position
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));  // Set direction
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
