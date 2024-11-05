#include "ActionInitialization.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh"

ActionInitialization::ActionInitialization(bool debugMode, G4int seedNumber)
: G4VUserActionInitialization(), fDebugMode(debugMode), fseedNumber(seedNumber) {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {

    if (fDebugMode) G4cout << "[Log] Before SetUserAction(new RunAction ...)" << G4endl;
    EventAction* eventAction = new EventAction(fDebugMode);
    SetUserAction(new RunAction(fDebugMode, fseedNumber, eventAction));
    if (fDebugMode) G4cout << "[Log] After SetUserAction(new RunAction ...)" << G4endl;

}

void ActionInitialization::Build() const {

    if (fDebugMode) G4cout << "[Log] Before RunAction(fFile, fTree, fDebugMode)" << G4endl;
    auto eventAction = new EventAction(fDebugMode);
    auto runAction = new RunAction(fDebugMode, fseedNumber, eventAction);
    
    if (fDebugMode) G4cout << "[Log] After RunAction(fFile, fTree, fDebugMode)" << G4endl;
    
    if (fDebugMode) G4cout << "[Log] Before SetUserAction(runAction)" << G4endl;
    SetUserAction(runAction);
    if (fDebugMode) G4cout << "[Log] After SetUserAction(runAction)" << G4endl;
    
    if (fDebugMode) G4cout << "[Log] Before SetUserAction(new PrimaryGeneratorAction...)" << G4endl;
    SetUserAction(new PrimaryGeneratorAction());
    if (fDebugMode) G4cout << "[Log] After SetUserAction(new PrimaryGeneratorAction...)" << G4endl;
    
    if (fDebugMode) G4cout << "[Log] Before SetUserAction(new EventAction...)" << G4endl;
    SetUserAction(eventAction);
    if (fDebugMode) G4cout << "[Log] After SetUserAction(new EventAction...)" << G4endl;

    SetUserAction(new SteppingAction(eventAction));

}
