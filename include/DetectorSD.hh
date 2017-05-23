
#ifndef DetectorSD_h
#define DetectorSD_h 1

#include "G4VSensitiveDetector.hh"

#include "Hit.hh"
#include "G4ThreeVector.hh"
#include "ProjectConstants.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;
class RunAction;



/// sensitive detector class
///
/// The hits are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step. A hit is created with each step with non zero 
/// energy deposit.

class DetectorSD : public G4VSensitiveDetector
{
  public:
    DetectorSD(const G4String& name, 
                const G4String& hitsCollectionName);
    virtual ~DetectorSD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

    void SetCenterPoint(G4ThreeVector);
    G4ThreeVector GetCenterPoint();

    void SetRotateYAngle(G4double);

    void SetEndFaceCoordinates();

    // G4double isOnEndFace(G4ThreeVector);

  private:
    HitsCollection* fHitsCollection;
    RunAction* runAction;
    G4double detEnergy;
    G4ThreeVector centerPoint;
    G4ThreeVector * firstEndFacePoint;
    G4ThreeVector * secondEndFacePoint;
    G4ThreeVector * thirdEndFacePoint;
    G4double rotateYAngle;
};


#endif
