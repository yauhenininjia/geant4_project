
#include "projectRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "Hist1i.h"
#include "Randomize.hh"

#include "G4SystemOfUnits.hh"
#include "g4root.hh"
#include <G4CsvAnalysisManager.hh>



RunAction::RunAction()
 : G4UserRunAction()
{ 
  // set printing event number per each 100 events
  G4RunManager::GetRunManager()->SetPrintProgress(1000);  

  G4CsvAnalysisManager* analysisManager = G4CsvAnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  analysisManager->CreateH1("1","Edep in absorber", 100, 0., 14*TeV);
}


RunAction::~RunAction()
{}



void RunAction::BeginOfRunAction(const G4Run*)
{ 
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  hist = new Hist1i(0, 14000000000, 1000000);
  G4CsvAnalysisManager::Instance()->OpenFile("analysis.csv");
  hitsCollection = new MomentumVelocity();
}

void RunAction::EndOfRunAction(const G4Run* )
{
  hist->save("spectrum.csv", "\"energy keV\", N");
  G4CsvAnalysisManager* analysisManager = G4CsvAnalysisManager::Instance();
  // G4cout << "Statistics: " << G4endl;
  // G4cout << "mean: " << analysisManager->GetH1(0)->mean() << G4endl;
  // G4cout << "rms: " << analysisManager->GetH1(0)->rms() << G4endl;
  // for(G4int i = 0; i < analysisManager->GetH1(0)->bins_entries().size(); i++) {
  //   G4cout << "entries from " << (i + 0) * 14.0 / 100 << " TeV to " << (i + 1) * 14.0 / 100 << " TeV: " << analysisManager->GetH1(0)->bins_entries()[i] << G4endl;
  // }
  analysisManager->Write();
  analysisManager->CloseFile();
  hitsCollection->WriteMomelocity("momelocity.csv");
}

void RunAction::FillHist(G4double energy)
{
  if (energy > 0) {
      // заполняем гистограмму величиной энергии в кэВ
      hist->fill(energy/keV);
      // G4cout << "--------" << G4endl;
      // G4cout << energy / keV << G4endl;
      // G4cout << "--------" << G4endl;
      //hist->fill(energy/keV + G4RandGauss::shoot(0, 33));


      G4CsvAnalysisManager* analysisManager = G4CsvAnalysisManager::Instance();
      analysisManager->FillH1(0, energy);
  }
}

void RunAction::InsertHitToMomelocityCollection(TrackerHit *hit)
{
  TrackerHit * h = new TrackerHit();
  h->SetTrackID  (hit->GetTrackID());
  h->SetChamberNb(hit->GetChamberNb());
  h->SetEdep(hit->GetEdep());
  h->SetPos (hit->GetPos());
  h->SetMomentum(hit->GetMomentum());
  h->SetVelocity(hit->GetVelocity());
  h->SetPreStepEnergy(hit->GetPreStepEnergy());
  h->SetPostStepEnergy(hit->GetPostStepEnergy());
  h->SetParticleName(hit->GetParticleName());
  h->SetAngle(hit->GetAngle());
  h->SetKineticEnergy(hit->GetKineticEnergy());

  hitsCollection->GetHitsArray().push_back(h);
}


