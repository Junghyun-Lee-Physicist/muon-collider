
#ifndef MaterialManager_HH
#define MaterialManager_HH 1

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Exception.hh"

#include <map>
#include <string>

class MaterialManager{

public:
    static MaterialManager* GetInstance();
    static void DeleteInstance();
    G4Material* GetMaterial(const G4String);

private:
    MaterialManager();
    ~MaterialManager();

    void SetupElements();
    void CreateMaterials();
    void RegisterMaterial();

    G4String symbol;
    G4double a, z, density;
    G4int nOfComponents, nOfAtoms;
   
    G4Element*  C;
    G4Element*  H;
    G4Element*  O;
    G4Element*  F;
    G4Material* fGalactic;
    G4Material* fAir;
    G4Material* fCopper;
    G4Material* fLead;
    G4Material* fTungsten;
    G4Material* fFluoPoly;
    G4Material* fPMMA;
    G4Material* fPS;
    
    static MaterialManager* fInstance;
    static bool isDeleted;
    G4NistManager* fNistMan;
    std::map<std::string, G4Material*> materialMap;

};

#endif
