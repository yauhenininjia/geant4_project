
#ifndef projectEventAction_h
#define projectEventAction_h 1

#include "G4UserEventAction.hh"

#include "globals.hh"

/// Event action class

class projectEventAction : public G4UserEventAction
{
  public:
    projectEventAction();
    virtual ~projectEventAction();

    virtual void  BeginOfEventAction(const G4Event* );
    virtual void    EndOfEventAction(const G4Event* );
};



#endif
