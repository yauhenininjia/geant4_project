
#ifndef Hit_h
#define Hit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEdep, fPos

class TrackerHit : public G4VHit
{
  public:
   TrackerHit();
   TrackerHit(const TrackerHit&);
    virtual ~TrackerHit();

    // operators
    const TrackerHit& operator=(const TrackerHit&);
    G4int operator==(const TrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetChamberNb(G4int chamb)      { fChamberNb = chamb; };
    void SetEdep     (G4double de)      { fEdep = de; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
    void SetVelocity (G4double velocity) { fVelocity = velocity; };
    void SetMomentum (G4ThreeVector momentum) { fMomentum = momentum; };
    void SetPreStepEnergy (G4double energy) { preStepEnergy = energy; }
    void SetPostStepEnergy (G4double energy) { postStepEnergy = energy; }
    void SetParticleName (G4String name) { particleName = name; }

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetChamberNb() const   { return fChamberNb; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };
    G4double GetVelocity() const { return fVelocity; };
    G4ThreeVector GetMomentum() const { return fMomentum; };
    G4double GetPreStepEnergy() const { return preStepEnergy; }
    G4double GetPostStepEnergy() const { return postStepEnergy; }
    G4String GetParticleName() const { return particleName; }

  private:

  G4int         fTrackID;
  G4int         fChamberNb;
  G4double      fEdep;
  G4ThreeVector fPos;
  G4double      fVelocity;
  G4ThreeVector fMomentum;
  G4double      totalEnergyDeposit;
  G4double      preStepEnergy;
  G4double      postStepEnergy;
  G4String      particleName;
};



typedef G4THitsCollection<TrackerHit> HitsCollection;

extern G4ThreadLocal G4Allocator<TrackerHit>* projectTrackerHitAllocator;



inline void* TrackerHit::operator new(size_t)
{
  if(!projectTrackerHitAllocator)
      projectTrackerHitAllocator = new G4Allocator<TrackerHit>;
  return (void *) projectTrackerHitAllocator->MallocSingle();
}



inline void TrackerHit::operator delete(void *hit)
{
  projectTrackerHitAllocator->FreeSingle((TrackerHit*) hit);
}



#endif
