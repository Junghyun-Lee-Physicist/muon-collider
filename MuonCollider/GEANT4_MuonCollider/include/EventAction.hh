#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include <vector>
using namespace std;

class RunAction;

class EventAction : public G4UserEventAction {
public:
    EventAction(bool debugMode);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event) override;
    virtual void EndOfEventAction(const G4Event* event) override;

    //void AddEnergyDeposit(G4double edep) { fEnergyDeposit += edep; }
    void SaveStepInfo( G4double Position_x, G4double Position_y, G4double Position_z,
                       G4double KineticEnergy, G4double KineticEnergyDiff, G4double EnergyDeposit,
                       G4double LocalTime, G4double GlobalTime,
                       G4int ProcessNumber ); 

    vector<G4double>& GetPosition_x()          {return VPosition_x; }
    vector<G4double>& GetPosition_y()          {return VPosition_y; }
    vector<G4double>& GetPosition_z()          {return VPosition_z; }
    vector<G4double>& GetKineticEnergy()       {return VKineticEnergy; }
    vector<G4double>& GetKineticEnergyDiff()   {return VKineticEnergyDiff; }
    vector<G4double>& GetEnergyDeposit()       {return VEnergyDeposit; }
    vector<G4double>& GetLocalTime()           {return VLocalTime; } 
    vector<G4double>& GetGlobalTime()          {return VGlobalTime; }
    vector<G4int>&    GetProcessNumber()       {return VProcessNumber; }


private:
    RunAction* fRunAction;
    G4double fEnergyDeposit;
    G4double fKinEnergy;
    bool fDebugMode;
    G4int fEventCounter;

    vector<G4double>   VPosition_x;
    vector<G4double>   VPosition_y;
    vector<G4double>   VPosition_z;
    vector<G4double>   VKineticEnergy;
    vector<G4double>   VKineticEnergyDiff;
    vector<G4double>   VEnergyDeposit;
    vector<G4double>   VLocalTime;
    vector<G4double>   VGlobalTime;
    vector<G4int>      VProcessNumber;

};

inline void EventAction::SaveStepInfo( G4double Position_x, G4double Position_y, G4double Position_z,
                                       G4double KineticEnergy, G4double KineticEnergyDiff, G4double EnergyDeposit,
                                       G4double LocalTime, G4double GlobalTime,
                                       G4int ProcessNumber )
{
    VPosition_x.push_back( Position_x );
    VPosition_y.push_back( Position_y );
    VPosition_z.push_back( Position_z );
    VKineticEnergy.push_back( KineticEnergy );
    VKineticEnergyDiff.push_back( KineticEnergyDiff );
    VEnergyDeposit.push_back( EnergyDeposit );
    VLocalTime.push_back( LocalTime );
    VGlobalTime.push_back( GlobalTime );
    VProcessNumber.push_back( ProcessNumber );

    fEnergyDeposit += EnergyDeposit;
    fKinEnergy = KineticEnergy;
}

#endif

