
//////////////////////////////////////////////////////////////////////
//                                                                  //
//  This class, named [ MaterialManager ], is designed to register  // 
//  and manage materials needed in the simulation.                  //
//                                                                  //
//  It is primarily used in [ Detector Construction ],              //
//  where the instance pointer of this class is utilized            //
//  to fetch and set materials using the [ GetMaterial ] method.    //
//                                                                  // 
//  The class is structured using the [ Singleton pattern ],        //
//  which ensures that only one instance of the class is created    //
//  and provides a global access point to this instance.            //
//                                                                  //
//  The [ GetInstance ] method returns the pointer                  //
//  to this unique instance,                                        //
//  while the constructor is declared private                       //
//  to prevent instances from being created outside the class.      //
//                                                                  // 
//  This design helps the use of resources to be more efficient     //
//  by maintaining only one instance.                               // 
//                                                                  //
//////////////////////////////////////////////////////////////////////


#include "MaterialManager.hh"
#include "G4SystemOfUnits.hh"

#include <sstream>
// In Geant4, it appears possible to use [ std::ostringstream ] without explicitly including <sstream>,
// likely because the compiler automatically includes standard libraries. 
// However, this behavior can vary between compilers, which might lead to issues in different environments. 
// Therefore, it's important to explicitly include <sstream> when using std::ostringstream to ensure code clarity and portability.


MaterialManager* MaterialManager::fInstance = nullptr;
bool MaterialManager::isDeleted = false;


MaterialManager::MaterialManager(){
    fNistMan = G4NistManager::Instance();
    SetupElements();
    CreateMaterials();
    RegisterMaterial();
}


void MaterialManager::DeleteInstance() {
    delete fInstance;
    fInstance = nullptr;
    isDeleted = true;
}


MaterialManager::~MaterialManager() {
    // In Geant4, since the materials are managed automatically, 
    // We don't have to do explicit deletion of dynamically allocated materials objects
    // e.g. new G4Material("Copper", ....) etc

    // The C++ map container is automatically allocated on the heap 
    // and is deallocated automatically. 
    // Therefore, there is no need to explicitly declare 
    // the [ materialMap ] in the destructor.
}


MaterialManager* MaterialManager::GetInstance(){

    if (isDeleted) {
        G4Exception("MaterialManager::GetInstance()",
                    "SingletonReuseAttempt",
                    FatalException,
                    "Attempt to create a new instance after deletion or disabling [ MaterialManager ].");
        return nullptr;
    }

    if (!fInstance) {
        fInstance = new MaterialManager();
    }

    return fInstance;

}

void MaterialManager::SetupElements(){

    C  = new G4Element("Carbon"  , symbol="C" , z=6., a=12.01*g/mole);
    H  = new G4Element("Hydrogen", symbol="H" , z=1., a=1.01*g/mole);
    O  = new G4Element("Oxygen"  , symbol="O" , z=8., a=16.00*g/mole);
    F  = new G4Element("Fluorine", symbol="F" , z=9., a=18.9984*g/mole);

}

void MaterialManager::CreateMaterials(){

    fGalactic = fNistMan->FindOrBuildMaterial("G4_Galactic");
    fAir      = fNistMan->FindOrBuildMaterial("G4_AIR");
    fCopper   = new G4Material("Copper", z = 29., a = 63.546 * g/mole, density = 8.96  * g/cm3);
    fLead     = new G4Material("Lead", z = 82., a = 207.2  * g/mole, density = 11.35 * g/cm3);
    fTungsten  = new G4Material("Tungsten" , z = 74., a = 183.84 * g/mole, density = 19.25 * g/cm3);

    // For Cherenkov Fiber Clading
    fFluoPoly = new G4Material("FluorinatedPolymer", density=1.43*g/cm3, nOfComponents = 2);
    fFluoPoly->AddElement(C, nOfAtoms = 2);
    fFluoPoly->AddElement(F, nOfAtoms = 2);

    // For Cherenkov Fiber Core & Scintillation Fiber Clading
    fPMMA = new G4Material("PMMA", density= 1.19*g/cm3, nOfComponents = 3);
    fPMMA->AddElement(C, nOfAtoms = 5);
    fPMMA->AddElement(H, nOfAtoms = 8);
    fPMMA->AddElement(O, nOfAtoms = 2);

    // Foir the Scintillation Fiber Core
    fPS = new G4Material("Polystyrene", density=1.05*g/cm3, nOfComponents = 2);
    fPS->AddElement(C, nOfAtoms = 8);
    fPS->AddElement(H, nOfAtoms = 8);

}

void MaterialManager::RegisterMaterial(){

    materialMap["Vacuum"]             = fGalactic;
    materialMap["Air"]                = fAir;
    materialMap["Copper"]             = fCopper;
    materialMap["Lead"]               = fLead;
    materialMap["Tungsten"]           = fTungsten;
    materialMap["FluorinatedPolymer"] = fFluoPoly;
    materialMap["PMMA"]               = fPMMA;
    materialMap["Polystyrene"]        = fPS;

}


G4Material* MaterialManager::GetMaterial(const G4String matName){

    auto materialMap_Iterator = materialMap.find(matName);

    if (materialMap_Iterator != materialMap.end()){
        // Return the pointer of G4Material in the materialMap container
        return materialMap_Iterator->second;
    } 
    else {
        // For the Error massage when material is not exist in the materialMap container

        std::ostringstream errorMsg;
        errorMsg << "ERROR : Material [ " << matName << " ] is not found!\n";
        errorMsg << "ERROR : Please check the [ DetectorConstruction ] or [ MaterialManager ] or etc..";

        G4Exception("MaterialManager::GetMaterial", "", FatalException, errorMsg.str().c_str());
        return nullptr;
    }

}

