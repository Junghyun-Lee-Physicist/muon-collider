
#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"


SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(), fEventAction(eventAction) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {

    G4int TrackID  = step->GetTrack()->GetTrackID();
    
    //if( TrackID != 1 ) {
    //    step->GetTrack()->SetTrackStatus(fStopAndKill);
    //    return;
    //}

    G4int StepNumber = step->GetTrack()->GetCurrentStepNumber(); 
    const G4StepPoint* preStepPoint  = step->GetPreStepPoint();
    const G4StepPoint* postStepPoint = step->GetPostStepPoint();

    G4bool IsEnv = ( postStepPoint->GetPhysicalVolume()->GetName()) == ("WorldEnvPhysical");
    if(IsEnv) step->GetTrack()->SetTrackStatus(fStopAndKill);

   
    G4double Position_x         = -50123.000;
    G4double Position_y         = -50123.000;
    G4double Position_z         = -50123.000;
    G4double KineticEnergy      = -50123.000;
    G4double KineticEnergyDiff  = -50123.000;
    G4double EnergyDeposit      = -50123.000;
    G4double LocalTime          = -50123.000;
    G4double GlobalTime         = -50123.000;
    G4String ProcessName        = "Initialized";
    G4int    ProcessNumber      = -15;
    
    Position_x              = postStepPoint->GetPosition().x();
    Position_y              = postStepPoint->GetPosition().y();
    Position_z              = postStepPoint->GetPosition().z();
    KineticEnergy           = postStepPoint->GetKineticEnergy();
    KineticEnergyDiff       = preStepPoint->GetKineticEnergy() - postStepPoint->GetKineticEnergy();
    EnergyDeposit           = step->GetTotalEnergyDeposit();

    ////G4cout<<"Current Edep & Kin : "<<EnergyDeposit / GeV <<"  &  "<<KineticEnergy / GeV <<G4endl;

    LocalTime               = postStepPoint->GetLocalTime();
    GlobalTime              = postStepPoint->GetGlobalTime();
    ProcessName             = postStepPoint->GetProcessDefinedStep()->GetProcessName();
    ProcessNumber           = ProcessDistinguisher(ProcessName);
    
    G4double PositionUnit   = cm;
    G4double EnergyUnit     = MeV;
    G4double TimeUnit       = ns;
    
    ////G4cout<<"Posi x, y, z : "<<Position_x<<", "<<Position_y<<", "<<Position_z<<G4endl;
    
    fEventAction->SaveStepInfo( Position_x/PositionUnit, Position_y/PositionUnit, Position_z/PositionUnit,
                                KineticEnergy/EnergyUnit, KineticEnergyDiff/EnergyUnit, EnergyDeposit/EnergyUnit,
                                LocalTime/TimeUnit, GlobalTime/TimeUnit,
                                ProcessNumber );
    

                           
}


using ProcessMap = std::map<std::string, int>;
using Iterator   = ProcessMap::const_iterator;
G4int SteppingAction::ProcessDistinguisher(G4String NameOfProcess) {

    G4int ProcessNumber = -77777;

    static const ProcessMap map = {

        {"PrimaryStep"          , 0},
        {"Transportation"       , 1},
        {"CoulombScat"          , 2},
        {"muIoni"               , 3},
        {"muPairProd"           , 4},
        {"muMinusCaptureAtRest" , 5},
        {"muBrems"              , 6},
        {"muonNuclear"          , 7},
        {"Decay"                , 8},

        {"Rayl"                 , 9}, // Rayleigh scattering
        {"conv"                 , 10}, // Gamma conversion = gamma's pair production
        {"msc"                  , 11},
        {"eIoni"                , 12},
        {"annihil"              , 13},
        {"eBrem"                , 14},
        {"RadioactiveDecayBase" , 15},
        {"ionIoni"              , 16},
        {"phot"                 , 17},
        {"compt"                , 18},
        {"StepLimiter"          , 19}

    };

    Iterator it = map.find(NameOfProcess);
    if (it != map.end()) {
        return it->second;  // Return the corresponding number
    } 
    else {
        G4cout<<"Missing process in book : "<<NameOfProcess<<G4endl;
        return -35;  // Return -35 if the process name is not found
    }

}
