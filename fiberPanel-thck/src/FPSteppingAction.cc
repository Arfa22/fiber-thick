// Aug 5, 2020: hexc and Zachary
// 
// Add stepping action to track energy loss of the primary particle energy loss.
//

#include "FPSteppingAction.hh"
#include "FPEventAction.hh"
#include "FPDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4OpticalPhoton.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FPSteppingAction::FPSteppingAction(FPEventAction* eventAction)
 : G4UserSteppingAction(), fEventAction(eventAction), fPhotonsCounts(0)
{}

FPSteppingAction::~FPSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FPSteppingAction::UserSteppingAction(const G4Step* step)
{
    // Collect energy and track length step by step
    
    // get volume of the current step
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    
    // energy deposit
    auto edep = step->GetTotalEnergyDeposit();
    
    // Ensure that only muon-initiated events are considered
    auto track = step->GetTrack();
    auto particleDef = track->GetDefinition();
    
    // Check if the primary particle is a muon (either MuonMinus or MuonPlus)
    if (particleDef == G4MuonMinus::MuonMinusDefinition() || particleDef == G4MuonPlus::MuonPlusDefinition()) {
       
    // Increment particle count regardless of particle type
    fPhotonsCounts++; // Increment particle count for muon-initiated events
    
    // Notify FPEventAction to track the particle count
    if (fEventAction) {
        fEventAction->AddPhotonCount();  // Increment particle count in FPEventAction
    }
}
          
    if (edep > 0.) {
      fEventAction->AddELoss(edep);
      //G4cout << " Energy deposit (in stepping action): " << G4BestUnit(edep, "Energy") << G4endl;
    }
    //if (edep <= 0.) G4cout << " Energy deposit (in stepping action): " << G4BestUnit(edep, "Energy") << G4endl;
    
    
    /*
    // step length
    G4double stepLength = 0.;
    if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
	stepLength = step->GetStepLength();
    }

    G4ThreeVector position = step->GetPostStepPoint()->GetPosition();

    
    if ( volume == fDetConstruction->GetLoopLV() ) {
	fEventAction->AddEloss_TrackLength(0, edep,stepLength);
    } else if (volume == fDetConstruction->GetPanelLV()) {
	fEventAction->AddEloss_TrackLength(1, edep,stepLength);
    }
    */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
