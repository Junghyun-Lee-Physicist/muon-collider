
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "skeletonClass.hh"

#include "G4SystemOfUnits.hh"

EventAction::EventAction(bool debugMode)
: G4UserEventAction(), fEnergyDeposit(0.), fDebugMode(debugMode) {}

EventAction::~EventAction() {

}

void EventAction::BeginOfEventAction(const G4Event* event) {
    
    fEnergyDeposit = 0.;
    fKinEnergy     = 0.;
  
    VPosition_x.clear();
    VPosition_y.clear();
    VPosition_z.clear();
    VKineticEnergy.clear();
    VKineticEnergyDiff.clear();
    VEnergyDeposit.clear();
    VLocalTime.clear();
    VGlobalTime.clear();
    VProcessNumber.clear();

    fEventCounter++;
    if (fEventCounter % 100 == 0)
    {
        G4cout << "Processed " << fEventCounter << " events" << G4endl;
    }
}

void EventAction::EndOfEventAction(const G4Event* event) {

    ///skeletonClass* sk = new skeletonClass();
    ///sk->setMsg("[ log using sk ] End of Event Action");
    ///sk->PrintMessage();
    ///delete sk;

    ///if (fDebugMode) {
    ///    G4cout << "[Log] End of Event Action for Event ID: " << event->GetEventID() << G4endl;
    ///}

    ////if (fRunAction) {
    ////    if (fDebugMode) G4cout << "[Log] Cummulated Edep = "<<fEnergyDeposit<<G4endl;
    ////    fRunAction->FillNtuple(event->GetEventID(), fEnergyDeposit);
    ////}

    ////G4cout<<"Cumulated Edep & loss kin energy : "<<fEnergyDeposit / GeV<<"  &&  "<< (1000.0 - fKinEnergy/GeV) << G4endl;

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->AddNtupleRow();
}

