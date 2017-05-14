#include "projectDetectorConstruction.hh"
#include "projectDetectorMessenger.hh"
#include "DetectorSD.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4NistManager.hh"
#include "globals.hh"
#include "G4VisAttributes.hh" 
#include "G4SDManager.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UserLimits.hh"
//
#include <iostream>
using namespace std;

G4ThreadLocal 
G4GlobalMagFieldMessenger* projectDetectorConstruction::fMagFieldMessenger = 0;

projectDetectorConstruction::projectDetectorConstruction()
:G4VUserDetectorConstruction(), 
 fNbOfChambers(0),
 fLogicTarget(NULL), fLogicChamber(NULL), 
 fTargetMaterial(NULL), fChamberMaterial(NULL), 
 fStepLimit(NULL),
 fCheckOverlaps(true)
{
  fMessenger = new projectDetectorMessenger(this);

  fNbOfChambers = 1;
  fLogicChamber = new G4LogicalVolume*[fNbOfChambers];
  this->detectorRotateYAngle = 0.0 * deg;
}
//
projectDetectorConstruction::~projectDetectorConstruction()
{
  delete [] fLogicChamber; 
  delete fStepLimit;
  delete fMessenger;
}

//

G4VPhysicalVolume* projectDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//

void projectDetectorConstruction::DefineMaterials()
{
  // Material definition 
  
  G4NistManager* nistManager = G4NistManager::Instance();

  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density;
  G4String name, symbol;
  G4int ncomponents;
   
  
  a=1.01*g/mole;
  G4Element* elH = new G4Element(name = "Hydrogen", symbol = "H" , z = 1., a);
  this->fTargetMaterial = new G4Material (name = "Hydrogen", density = 3.000*g/cm3, ncomponents = 1);
  this->fTargetMaterial->AddElement(elH, 1);
  fChamberMaterial = nistManager->FindOrBuildMaterial("G4_PbWO4");

  // Vacuum
  
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//

  G4VPhysicalVolume* projectDetectorConstruction::DefineVolumes()
{
   // Get materials
  // G4Material* targetMaterial  = G4Material::GetMaterial("Hydrogen");
  G4Material* defaultMaterial = G4Material::GetMaterial("Galactic");
  G4Material* chamberMaterial = G4Material::GetMaterial("G4_PbWO4");



// Sizes of the principal geometrical components (solids)

  G4double worldLength = 100*cm;

  G4double targetLength =  5.0*cm;             // length of Target
  G4double targetRadius  = 0.5*targetLength;   // Radius of Target
  targetLength = 0.5*targetLength;        // Half length of the Target  

    
  G4double chamberWidth = 3*cm; // width of the chamber
  G4double chamberLength = 22*cm; // length of the chamber
  G4double chamberHigh = 3*cm; // high of the chamber 
  G4double gap1 = 5*cm; // расстояние от источника до мишени
  G4double gap2 = 30*cm; // расстояние от мишени до детектора
  
  
  

  // Definitions of Solids, Logical Volumes, Physical Volumes

  // World

  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);

  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;

  G4Box* worldS
    = new G4Box("world",                                    //its name
                worldLength/2,worldLength/2,worldLength/2); //its size
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,   //its solid
                 defaultMaterial,      //its material
                 "World"); //its name
  //

//  Must place the World Physical volume unrotated at (0,0,0).
  // помещаем в центр координат
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother  volume
                 false,           // no boolean operations
                 0,               // copy number
                 fCheckOverlaps); // checking overlaps 



 // Target
  
  G4ThreeVector positionTarget = G4ThreeVector(0,0,gap1);

  G4Tubs* targetS = new G4Tubs("target",0.,targetRadius,targetLength,0.,2*CLHEP::pi);
  fLogicTarget = new G4LogicalVolume(targetS, fTargetMaterial,"target");

  G4double phi = 0.0003 * radian;
  G4RotationMatrix *rotation_matrix  = new G4RotationMatrix();
  rotation_matrix->rotateX(phi);

  G4VPhysicalVolume* targetPV = new G4PVPlacement(rotation_matrix, G4ThreeVector(0, 0, gap1), fLogicTarget, "target", worldLV, false, 0, fCheckOverlaps);
                  

  G4cout << "Target is " << targetLength/cm << " cm of "
         << fTargetMaterial->GetName() << G4endl;
 // Chamber


  G4Box* cham_box = new G4Box("chamber", chamberWidth/2, chamberHigh/2, chamberLength/2);
  G4LogicalVolume* cham_log = new G4LogicalVolume(cham_box, chamberMaterial, "chamber");

  G4RotationMatrix *chamber_rotation_matrix  = new G4RotationMatrix();
  chamber_rotation_matrix->rotateY(-this->detectorRotateYAngle);
  G4ThreeVector coordinates =  *(this->GetRotatedDetectorCoordinates(gap1 + gap2));
  G4VPhysicalVolume* cham_phys = new G4PVPlacement(chamber_rotation_matrix, coordinates, cham_log, "chamber", worldLV, false, 0);


 // Visualization attributes

  G4VisAttributes* boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));

  worldLV      ->SetVisAttributes(boxVisAtt);
  fLogicTarget ->SetVisAttributes(boxVisAtt);
 

  // Set cut equal to 10 GeV for detector

  G4double minEkin = 10*GeV;
  cham_log-> SetUserLimits(new G4UserLimits(DBL_MAX, DBL_MAX, DBL_MAX, minEkin));

  // Always return the physical world

  return worldPV;

}

void projectDetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors

  DetectorSD* detectorSD = new DetectorSD("DetectorSD", "HitsCollection");
  G4SDManager* sdMan = G4SDManager::GetSDMpointer();
  sdMan->AddNewDetector(detectorSD);
  SetSensitiveDetector("chamber", detectorSD, true);
  

  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
 // G4ThreeVector fieldValue = G4ThreeVector();
  //fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  //fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
 // G4AutoDelete::Register(fMagFieldMessenger);
}


//
void projectDetectorConstruction::SetTargetMaterial(G4String materialName)
{

  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial = 
              nistManager->FindOrBuildMaterial(materialName);

  if (fTargetMaterial != pttoMaterial) {
     if ( pttoMaterial ) {
        fTargetMaterial = pttoMaterial;
        if (fLogicTarget) fLogicTarget->SetMaterial(fTargetMaterial);
        G4cout 
          << G4endl 
          << "----> The target is made of " << materialName << G4endl;
     } else {
        G4cout 
          << G4endl 
          << "-->  WARNING from SetTargetMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
}

void projectDetectorConstruction::SetChamberMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial =
              nistManager->FindOrBuildMaterial(materialName);

  if (fChamberMaterial != pttoMaterial) {
     if ( pttoMaterial ) {
        fChamberMaterial = pttoMaterial;
        for (G4int copyNo=0; copyNo<fNbOfChambers; copyNo++) {
            if (fLogicChamber[copyNo]) fLogicChamber[copyNo]->
                                               SetMaterial(fChamberMaterial);
        }
        G4cout 
          << G4endl 
          << "----> The chambers are made of " << materialName << G4endl;
     } else {
        G4cout 
          << G4endl 
          << "-->  WARNING from SetChamberMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
}
 void projectDetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

void projectDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
  fCheckOverlaps = checkOverlaps;
}

void projectDetectorConstruction::SetDetectorRotateYAngle(G4double angle) {
  this->detectorRotateYAngle = angle;
}

G4ThreeVector* projectDetectorConstruction::GetRotatedDetectorCoordinates(G4double distance) {
  G4ThreeVector *coordinates = new G4ThreeVector();

  G4double x = distance * sin(this->detectorRotateYAngle);
  G4double z = distance * cos(this->detectorRotateYAngle);

  coordinates->setX(x);
  coordinates->setZ(z);

  return coordinates;
}

