#include "projectActionInitialization.hh"
#include "projectPrimaryGeneratorAction.hh"
#include "projectRunAction.hh"
#include "projectEventAction.hh"


projectActionInitialization::projectActionInitialization()
 : G4VUserActionInitialization()
{}


projectActionInitialization::~projectActionInitialization()
{}


void projectActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction);
}

void projectActionInitialization::Build() const
{
  SetUserAction(new projectPrimaryGeneratorAction);
  SetUserAction(new RunAction);
  SetUserAction(new projectEventAction);
}  

