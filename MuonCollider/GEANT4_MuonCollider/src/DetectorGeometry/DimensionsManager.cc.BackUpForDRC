
#include "DimensionsManager.hh"
#include "G4SystemOfUnits.hh"


DimensionsManager* DimensionsManager::fInstance = nullptr;
bool DimensionsManager::isDeleted = false;


DimensionsManager* DimensionsManager::GetInstance() {

    if (isDeleted) {
        G4Exception("DimensionsManager::GetInstance()",
                    "SingletonReuseAttempt",
                    FatalException,
                    "Attempt to create a new instance after deletion or disabling [ DimensionsManager ].");
        return nullptr;
    }

    if (!fInstance) {
        fInstance = new DimensionsManager();
    }

    return fInstance;

}


void DimensionsManager::DeleteInstance() {
    delete fInstance;
    fInstance = nullptr;
    isDeleted = true;
}


DimensionsManager::~DimensionsManager() {}


DimensionsManager::DimensionsManager()
{
   // Width, Height, Legnth --> X, Y, Z axis

   // World
   WorldSize_Width       = 0.50 * m;
   WorldSize_Height      = 0.50 * m;
   WorldSize_Length      = 12.0 * m;

   
   // Module 
   ModuleSize_Width      = 0.025 * m;
   ModuleSize_Height     = 0.025 * m;
   ModuleSize_Length     = 5.00 * m;
   NumberOfModule_Row    = 1;
   NumberOfModule_Column = 1;


   // Fiber
   //   - Cladding
   FiberCladdingSize_Radius = 0.500 * mm;
   FiberCladdingSize_Length = ModuleSize_Length;     
   //       1) --> Cherenkov Fiber
   CherenFiberSize_Radius   = 0.490 * mm;
   CherenFiberSize_Length   = ModuleSize_Length;
   //       2) --> Scintillation Fiber
   ScintFiberSize_Radius    = 0.485 * mm;
   ScintFiberSize_Length   = ModuleSize_Length;

}


const double* DimensionsManager::GetWorldSize_Width()          const { return &WorldSize_Width; }
const double* DimensionsManager::GetWorldSize_Height()         const { return &WorldSize_Height; }
const double* DimensionsManager::GetWorldSize_Length()         const { return &WorldSize_Length; }
const double* DimensionsManager::GetModuleSize_Width()         const { return &ModuleSize_Width; }
const double* DimensionsManager::GetModuleSize_Height()        const { return &ModuleSize_Height; }
const double* DimensionsManager::GetModuleSize_Length()        const { return &ModuleSize_Length; }
const int*    DimensionsManager::GetNumberOfModule_Row()       const { return &NumberOfModule_Row; }
const int*    DimensionsManager::GetNumberOfModule_Column()    const { return &NumberOfModule_Column; }
const double* DimensionsManager::GetFiberCladdingSize_Radius() const { return &FiberCladdingSize_Radius; }
const double* DimensionsManager::GetFiberCladdingSize_Length() const { return &FiberCladdingSize_Length; } 
const double* DimensionsManager::GetCherenFiberSize_Radius()   const { return &CherenFiberSize_Radius; }
const double* DimensionsManager::GetCherenFiberSize_Length()   const { return &CherenFiberSize_Length; }
const double* DimensionsManager::GetScintFiberSize_Radius()    const { return &ScintFiberSize_Radius; }
const double* DimensionsManager::GetScintFiberSize_Length()    const { return &ScintFiberSize_Length; }


void DimensionsManager::SetWorldSize_Width_Height_Length(double width, double height, double length) {
    WorldSize_Width  = width;
    WorldSize_Height = height;
    WorldSize_Length = length;
}

void DimensionsManager::SetModuleSize_Width_Height_Length(double width, double height, double length) {
    ModuleSize_Width  = width;
    ModuleSize_Height = height;
    ModuleSize_Length = length;
}

void DimensionsManager::SetNumberOfModule_Row_Column(int row, int column) {
    NumberOfModule_Row    = row;
    NumberOfModule_Column = column;
}

void DimensionsManager::SetFiberCladdingSize_Radius_Length(double radius, double length) {
    FiberCladdingSize_Radius = radius;
    FiberCladdingSize_Length = length;
}

void DimensionsManager::SetCherenFiberSize_Radius_Length(double radius, double length) {
    CherenFiberSize_Radius = radius;
    CherenFiberSize_Length = length;
}

void DimensionsManager::SetScintFiberSize_Radius_Length(double radius, double length) {
    ScintFiberSize_Radius = radius;
    ScintFiberSize_Length = length;
}

