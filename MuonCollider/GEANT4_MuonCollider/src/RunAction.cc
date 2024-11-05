
#include "RunAction.hh"
#include "EventAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
//#include "g4root.hh"

RunAction::RunAction(bool debugMode, G4int seedNumber, EventAction* eventAction)
: G4UserRunAction(), fDebugMode(debugMode), fseedNumber(seedNumber), fEventAction(eventAction) {

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    //analysisManager->SetNtupleMerging(true);

    analysisManager->CreateNtuple("MuonSim", "Muon Sim ntuple");

    analysisManager->CreateNtupleDColumn("Position_x",        fEventAction->GetPosition_x());
    analysisManager->CreateNtupleDColumn("Position_y",        fEventAction->GetPosition_y());
    analysisManager->CreateNtupleDColumn("Position_z",        fEventAction->GetPosition_z());
    analysisManager->CreateNtupleDColumn("KineticEnergy",     fEventAction->GetKineticEnergy());
    analysisManager->CreateNtupleDColumn("KineticEnergyDiff", fEventAction->GetKineticEnergyDiff());
    analysisManager->CreateNtupleDColumn("EnergyDeposit",     fEventAction->GetEnergyDeposit());
    analysisManager->CreateNtupleDColumn("LocalTime",         fEventAction->GetLocalTime());
    analysisManager->CreateNtupleDColumn("GlobalTime",        fEventAction->GetGlobalTime());
    analysisManager->CreateNtupleIColumn("ProcessNumber",     fEventAction->GetProcessNumber());

    analysisManager->FinishNtuple();
}

RunAction::~RunAction() {
    delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run*) {

    auto analysisManager = G4AnalysisManager::Instance();
    string seedNumber_str = to_string(fseedNumber);
    analysisManager->OpenFile("ntupleMuonCollider_" + seedNumber_str + ".root");

    if (fDebugMode) G4cout << "[Log] Begin of Run Action" << G4endl;
}

void RunAction::EndOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

    if (fDebugMode) G4cout << "[Log] End of Run Action" << G4endl;
}

void RunAction::FillNtuple(G4int eventID, G4double energyDeposit) {

////    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
////    analysisManager->FillNtupleIColumn(0, eventID);
////    analysisManager->FillNtupleDColumn(1, energyDeposit);
////    analysisManager->AddNtupleRow();
////
////    if (fDebugMode) G4cout << "[Log] Filling Ntuple with event ID: " << eventID << " and energy deposit: " << energyDeposit << G4endl;
////
////    analysisManager->Write();
////    analysisManager->CloseFile();
////
}

