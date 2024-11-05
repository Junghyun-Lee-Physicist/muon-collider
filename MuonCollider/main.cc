#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "FTFP_BERT.hh"
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"

#include "Randomize.hh"

#include "Analyzer.hh"

#include <string>
#include <cstdlib> // std::atoi
#include <cstring> // std::isdigit

class CommandLineOptions {

    public:
        CommandLineOptions(int argc, char* argv[]) : macro(""), seed(0), isArguCorrectlyApplied(true) {
            parseArguments(argc, argv);
        }
    
        void printUsage() const {
            std::cout << "Usage: ./ExeMuonCollider -m <name of macro file> -s <seed number>" << std::endl;
        }
    
        bool IsArguCorrectlyApplied() const {
            return isArguCorrectlyApplied;
        }
    
        std::string getMacro() const {
            return macro;
        }
    
        int getSeed() const {
            return seed;
        }
    
    private:
        std::string macro;
        int seed;
        bool isArguCorrectlyApplied;
    
        void parseArguments(int argc, char* argv[]) {

            for (int i = 1; i < argc; i += 2) {
                if (std::string(argv[i]) == "-m") {
                    if (i + 1 < argc) {
                        macro = argv[i + 1];
                    } else {
                        std::cout << "No macro file specified, running UI instead." << std::endl;
                    }
                } else if (std::string(argv[i]) == "-s") {
                    if (i + 1 < argc) {
                        if (isInteger(argv[i + 1])) {
                            seed = std::atoi(argv[i + 1]);
                        } else {
                            std::cout << "Seed must be an integer." << std::endl;
                            isArguCorrectlyApplied = false;
                            return;
                        }
                    } else {
                        std::cout << "Seed not specified, using default value 0." << std::endl;
                    }
                } else {
                    printUsage();
                    isArguCorrectlyApplied = false;
                    return;
                }
            }
    
            if (macro.empty()) {
                std::cout << "No macro file specified, running UI instead." << std::endl;
            }
        }
    
        bool isInteger(const char* str) {
            for (size_t i = 0; i < std::strlen(str); ++i) {
                if (!std::isdigit(str[i])) {
                    return false;
                }
            }
            return true;
        }
};



int main(int argc, char** argv) {

    CommandLineOptions options(argc, argv);
    if (!options.IsArguCorrectlyApplied()) {
        return 1;
    }
    G4String macroName  = options.getMacro();
    G4int    seedNumber = options.getSeed(); 
    std::cout << "Setted Macro file using 1st argument  : " << options.getMacro() << "\n";
    std::cout << "Setted Seed Number using 2nd argument : " << options.getSeed()  << "\n";

    bool debugMode = false;
    if (debugMode) G4cout << "[Log] Starting Geant4 application" << G4endl;

    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);

    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new FTFP_BERT);

    if (debugMode) G4cout << "[Log] Before ActionInitialization" << G4endl;

    runManager->SetUserInitialization(new ActionInitialization(debugMode, seedNumber));
    if (debugMode) G4cout << "[Log] After ActionInitialization" << G4endl;



    auto visManager = new G4VisExecutive;
    visManager->Initialize();

    auto UImanager = G4UImanager::GetUIpointer();
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    if (ui) {

        // Set seed as 1 when activate visulization
        G4Random::setTheEngine(new CLHEP::RanecuEngine);
        G4Random::setTheSeed(seedNumber);

        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;

    } else {

        // Set the seed value as inputed argument
        G4Random::setTheEngine(new CLHEP::RanecuEngine);
        G4Random::setTheSeed(seedNumber);

        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + macroName);
    }

    delete visManager;
    delete runManager;
    if (debugMode) G4cout << "[Log] Geant4 application ended" << G4endl;

    //----------------------------------------------------------------------
    // Analyzer
    Analyzer* analyzer = new Analyzer( ".", seedNumber);
    delete analyzer;

    return 0;
}

