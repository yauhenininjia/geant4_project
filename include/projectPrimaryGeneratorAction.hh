
#ifndef projectPrimaryGeneratorAction_h
#define projectPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;

/// The primary generator action class with particle gum.
///
/// It defines a single particle which hits the Tracker 
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class 
/// (see the macros provided with this example).

class projectPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    projectPrimaryGeneratorAction();    
    virtual ~projectPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* );

    G4GeneralParticleSource* GetParticleGun() {return fParticleGun;}
  
    // Set methods
    void SetRandomFlag(G4bool );

  private:
    G4GeneralParticleSource*          fParticleGun; // G4 particle gun
};



#endif
