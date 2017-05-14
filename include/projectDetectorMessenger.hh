#ifndef projectDetectorMessenger_h
#define projectDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class projectDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;



/// Messenger class that defines commands for projectDetectorConstruction.
///
/// It implements commands:
/// - /project/det/setTargetMaterial name
/// - /project/det/setChamberMaterial name
/// - /project/det/stepMax value unit

class projectDetectorMessenger: public G4UImessenger
{
  public:
    projectDetectorMessenger(projectDetectorConstruction* );
    virtual ~projectDetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    projectDetectorConstruction*  fDetectorConstruction;

    G4UIdirectory*           fprojectDirectory;
    G4UIdirectory*           fDetDirectory;

    G4UIcmdWithAString*      fTargMatCmd;
    G4UIcmdWithAString*      fChamMatCmd;

    G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
};



#endif
