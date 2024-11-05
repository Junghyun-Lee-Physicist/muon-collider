#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "G4AnalysisManager.hh"
//#include "g4root.hh"

class EventAction;

class RunAction : public G4UserRunAction {
public:
    RunAction(bool debugMode, G4int seedNumber, EventAction* eventAction);
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    void FillNtuple(G4int eventID, G4double energyDeposit);

private:
    bool fDebugMode;
    G4int fseedNumber;
    EventAction* fEventAction;
};

#endif

