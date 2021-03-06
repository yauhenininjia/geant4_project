
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
    newHit->SetPos (aStep->GetPreStepPoint()->GetPosition());
    newHit->SetMomentum(aStep->GetTrack()->GetMomentumDirection());
    newHit->SetVelocity(aStep->GetTrack()->GetVelocity());
    newHit->SetPreStepEnergy(aStep->GetPreStepPoint()->GetTotalEnergy());
    newHit->SetPostStepEnergy(aStep->GetPostStepPoint()->GetTotalEnergy());
    newHit->SetParticleName(aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
    newHit->SetKineticEnergy(aStep->GetPreStepPoint()->GetKineticEnergy());
    // G4cout << "Track kinetic energy: " << aStep->GetTrack()->GetKineticEnergy() << G4endl;
    // G4cout << "PreStep kinetic energy: " << aStep->GetPreStepPoint()->GetKineticEnergy() << G4endl;
    // G4cout << "PostStep kinetic energy: " << aStep->GetPostStepPoint()->GetKineticEnergy() << G4endl;
    // G4cout << "Energy deposit: " << edep << G4endl;
    // G4cout << "Dynamic particle: " << aStep->GetTrack()->GetDynamicParticle()->GetKineticEnergy() << G4endl;

    // G4cout  << "Torec points: "
    //         << *firstEndFacePoint << " "
    //         << *secondEndFacePoint << " "
    //         << *thirdEndFacePoint << G4endl;
    // G4cout << "Prestep point: " << aStep->GetPreStepPoint()->GetPosition() << G4endl;
    // G4cout << "Poststep point: " << aStep->GetPostStepPoint()->GetPosition() << G4endl;

    G4ThreeVector *centerVector = new G4ThreeVector(centerPoint.x(), 0, centerPoint.z() - PROJECT_CONSTANTS::GAP_1);
    newHit->SetAngle(newHit->GetMomentum().angle(*centerVector));
    delete centerVector;

    fHitsCollection->insert( newHit );

    runAction->InsertHitToMomelocityCollection(newHit);
    runAction->FillHist(aStep->GetPreStepPoint()->GetKineticEnergy());


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
}

void DetectorSD::SetCenterPoint(G4ThreeVector coords) {
  this->centerPoint = coords;
}

G4ThreeVector DetectorSD::GetCenterPoint() {
  return this->centerPoint;
}

void DetectorSD::SetRotateYAngle(G4double angle) {
  this->rotateYAngle = angle;
}

void DetectorSD::SetEndFaceCoordinates() {
  G4double d = PROJECT_CONSTANTS::GAP_1 + PROJECT_CONSTANTS::GAP_2;
  G4double l = PROJECT_CONSTANTS::CHAMBER_LENGTH;
  G4double w = PROJECT_CONSTANTS::CHAMBER_WIDTH;
  G4double h = PROJECT_CONSTANTS::CHAMBER_HEIGHT;

  G4double x_1 = ( d - (l / 2) ) * sin(this->rotateYAngle);
  G4double y_1 = 0;
  G4double z_1 = ( d - (l / 2) ) * cos(this->rotateYAngle);

  this->firstEndFacePoint = new G4ThreeVector(x_1, y_1, z_1);

  G4double x_2 = ( d - (l / 2) ) * sin(this->rotateYAngle);
  G4double y_2 = h / 2;
  G4double z_2 = ( d - (l / 2) ) * cos(this->rotateYAngle);

  this->secondEndFacePoint = new G4ThreeVector(x_2, y_2, z_2);

  G4double x_3 = ( ( (d - l / 2) ) * sin(this->rotateYAngle) ) - ( ( w / 2 ) * cos(this->rotateYAngle) );
  G4double y_3 = 0;
  G4double z_3 = ( ( d - ( l / 2 ) ) * cos(this->rotateYAngle) ) + ( ( w / 2 ) * sin(this->rotateYAngle) );

  this->thirdEndFacePoint = new G4ThreeVector(x_3, y_3, z_3);
}

// G4double DetectorSD::isOnEndFace(G4ThreeVector point) {
//   G4double adx = firstEndFacePoint->x() - point.x();
//   G4double bdx = secondEndFacePoint->x() - point.x();
//   G4double cdx = thirdEndFacePoint->x() - point.x();

//   G4double ady = firstEndFacePoint->y() - point.y();
//   G4double bdy = secondEndFacePoint->y() - point.y();
//   G4double cdy = thirdEndFacePoint->y() - point.y();

//   G4double adz = firstEndFacePoint->z() - point.z();
//   G4double bdz = secondEndFacePoint->z() - point.z();
//   G4double cdz = thirdEndFacePoint->z() - point.z();

//   return  adx * (bdy * cdz - bdz * cdy) +
//           bdx * (cdy * adz - cdz * ady) +
//           cdx * (ady * bdz - adz * bdy);
// }
