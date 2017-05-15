
#ifndef projectRunAction_h
#define projectRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "MomentumVelocity.hh"



class G4Run;
class Hist1i;

/// Run action class

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run* run);
    virtual void EndOfRunAction(const G4Run* run);
    
    void InsertHitToMomelocityCollection(TrackerHit*);
    
    void FillHist(G4double);

  private:
    Hist1i* hist;
    MomentumVelocity *hitsCollection;
};



#endif
