
#include "DetectorSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "projectRunAction.hh"
#include "G4RunManager.hh"
#include "Hit.hh"
#include "g4root.hh"
#include <G4CsvAnalysisManager.hh>



DetectorSD::DetectorSD(const G4String& name,
                         const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
  runAction = (RunAction*) G4RunManager::GetRunManager()->GetUserRunAction();
}



DetectorSD::~DetectorSD() 
{}



void DetectorSD::Initialize(G4HCofThisEvent* hce)
{
  detEnergy = 0;
  // Create hits collection

  fHitsCollection 
    = new HitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce

  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 
}



G4bool DetectorSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{  
  if (aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {

    // energy deposit
    G4double edep = aStep->GetTotalEnergyDeposit();
    detEnergy += edep;

    if (edep==0.) return false;

    TrackerHit* newHit = new TrackerHit();

    newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
    newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()
                                                 ->GetCopyNumber());
    newHit->SetEdep(edep);
    newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());

    fHitsCollection->insert( newHit );

    // Prints trackID, chamberNb, Edep and position
    // newHit->Print();

    G4cout << "***********************" << G4endl;
    auto position = newHit->GetPos();
    G4cout << position << G4endl;
    // G4cout << position.x() << " " << position.y() << " " << position.z() << G4endl;
    G4cout << "***********************" << G4endl;
    
    runAction->FillHist(edep);
     
  }

  return true;
}



void DetectorSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) { 
     G4int nofHits = fHitsCollection->entries();
     G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits 
            << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }

  // сохраняем энергию накопленную за событие в детекторе
  // в гистограмму
  // runAction->FillHist(detEnergy);
}
