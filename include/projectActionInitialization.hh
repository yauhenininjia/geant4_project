
#ifndef projectActionInitialization_h
#define projectActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class B4DetectorConstruction;

/// Action initialization class.
///

class projectActionInitialization : public G4VUserActionInitialization
{
  public:
    projectActionInitialization();
    virtual ~projectActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif

    
