
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "DetectorConstruction.hh"

#include "G4SDManager.hh"
////#include "SensitiveDetector.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

//#include "temp.hh"
//#include "TFile.h"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(), fMaterialManager(nullptr), fDimensionsManager(nullptr)
{
    SetMaterials();
    SetDimensions();
    SetupColorForVolume();
    Check_Overlap_of_Volumes = true;
}

DetectorConstruction::~DetectorConstruction() {
    MaterialManager::DeleteInstance();
    DimensionsManager::DeleteInstance();
 
    delete fColorOrange;
    delete fColorBlue;   
}

void DetectorConstruction::SetMaterials() {
    fMaterialManager = MaterialManager::GetInstance();
    fWorldMaterial = fMaterialManager->GetMaterial("Vacuum");
    fModuleMaterial = fMaterialManager->GetMaterial("Tungsten");
}

void DetectorConstruction::SetDimensions() {
   
    fModuleSize_Width = new G4double(10.00 * cm);
    fModuleSize_Height = new G4double(10.00 * cm);
    fModuleSize_Length = new G4double(500.00 * cm);

    fWorldEnvSize_Width  = new G4double( (*fModuleSize_Width)  + (1. * cm) );
    fWorldEnvSize_Height = new G4double( (*fModuleSize_Height) + (1. * cm) );
    fWorldEnvSize_Length = new G4double( (2.0*(*fModuleSize_Length)) + (1. * cm) );

    fWorldSize_Width  = new G4double( (*fWorldEnvSize_Width)  + (1. * cm) );
    fWorldSize_Height = new G4double( (*fWorldEnvSize_Height) + (1. * cm) );
    fWorldSize_Length = new G4double( (*fWorldEnvSize_Length) + (1. * cm) );

}

void DetectorConstruction::SetupColorForVolume() {
    fColorTransparency = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.0));
    fColorTransparency->SetVisibility(true);

    fColorGray = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3, 0.3));
    fColorGray->SetVisibility(true);
 
    fColorOrange = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0, 1.0));
    fColorOrange->SetVisibility(true);

    fColorBlue = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 1.0));
    fColorBlue->SetVisibility(true);
}

G4VPhysicalVolume* DetectorConstruction::Construct() {

    //G4GeometryManager::GetInstance()->OpenGeometry();
    //G4PhysicalVolumeStore::GetInstance()->Clean();
    //G4LogicalVolumeStore::GetInstance()->Clean();
    //G4SolidStore::GetInstance()->Clean();

    WorldSol = new G4Box("WorldSolid", 0.5 * (*fWorldSize_Width), 0.5 * (*fWorldSize_Height), 0.5 * (*fWorldSize_Length));
    WorldLog = new G4LogicalVolume(WorldSol, fWorldMaterial, "WorldLogical");
    WorldPhy = new G4PVPlacement(0, G4ThreeVector(), WorldLog, "WorldPhysical", nullptr, false, 0, Check_Overlap_of_Volumes);
    WorldLog->SetVisAttributes(fColorTransparency);


    WorldEnvSol = new G4Box("WorldEnvSolid", 0.5 * (*fWorldEnvSize_Width), 0.5 * (*fWorldEnvSize_Height), 0.5 * (*fWorldEnvSize_Length));
    WorldEnvLog = new G4LogicalVolume(WorldEnvSol, fWorldMaterial, "WorldEnvLogical");
    WorldEnvPhy = new G4PVPlacement(0, G4ThreeVector(), WorldEnvLog, "WorldEnvPhysical", WorldLog, false, 0, Check_Overlap_of_Volumes);
    WorldEnvLog->SetVisAttributes(fColorTransparency);


    ConstructModules();

    return WorldPhy;
}

void DetectorConstruction::ConstructModules() {

    //G4double voxelSize = 0.01 * m;
    G4double voxelSize = 0.04 * m;
    G4double voxelHalfSize = 0.5 * voxelSize;

    //G4Box* voxelSolid = new G4Box("VoxelSolid", voxelHalfSize, voxelHalfSize, voxelHalfSize);
    G4Box* voxelSolid = new G4Box("VoxelSolid", 0.5 * (*fModuleSize_Width), 0.5 * (*fModuleSize_Height), 0.5 * (*fModuleSize_Length) );
    voxelLog = new G4LogicalVolume(voxelSolid, fModuleMaterial, "VoxelLogical");

    //G4VisAttributes* voxelVisAttr = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.0));
    //voxelVisAttr->SetVisibility(true);
    //voxelLog->SetVisAttributes(voxelVisAttr);

    G4int numVoxelsX = static_cast<G4int>(*fModuleSize_Width / voxelSize);
    G4int numVoxelsY = static_cast<G4int>(*fModuleSize_Height / voxelSize);
    G4int numVoxelsZ = static_cast<G4int>(*fModuleSize_Length / voxelSize);

    new G4PVPlacement(0, G4ThreeVector(0,0, 0.5 * (*fModuleSize_Length) ), voxelLog, "VoxelPhysical", WorldEnvLog, false, 0, Check_Overlap_of_Volumes);
/*
    for (G4int i = 0; i < numVoxelsX; ++i) {
        for (G4int j = 0; j < numVoxelsY; ++j) {
            for (G4int k = 0; k < numVoxelsZ; ++k) {
                G4ThreeVector voxelPosition(
                    -(*fModuleSize_Width) / 2 + voxelHalfSize + i * voxelSize,
                    -(*fModuleSize_Height) / 2 + voxelHalfSize + j * voxelSize,
                    -(*fModuleSize_Length) / 2 + voxelHalfSize + k * voxelSize
                );

                new G4PVPlacement(
                    0, voxelPosition, voxelLog, "VoxelPhysical", WorldLog, false, 
                    i * numVoxelsY * numVoxelsZ + j * numVoxelsZ + k, Check_Overlap_of_Volumes
                );
            }
        }
    }
*/

    //TFile* outputFile = new TFile("output.root", "RECREATE");
    //outputFile->Close();
 
}

void DetectorConstruction::ConstructSDandField() {

////    // 감지기 매니저를 사용하기 위해 G4SDManager 헤더 파일을 포함해야 합니다.
////    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
////    
////    // 새로운 민감한 탐지기 생성 및 등록
////    SensitiveDetector* sensDet = new SensitiveDetector("SensitiveDetector");
////    sdManager->AddNewDetector(sensDet);
////    
////    // 논리적 볼륨에 민감한 탐지기 설정
////    SetSensitiveDetector("VoxelLogical", sensDet);

//////    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
//////    SensitiveDetector* sensitiveDetector = new SensitiveDetector("SensitiveDetector");
//////    sdManager->AddNewDetector(sensitiveDetector);
//////    SetSensitiveDetector("VoxelLogical", sensitiveDetector); // 민감한 탐지기를 설정할 논리 볼륨 이름으로 변경
 

    
    //TFile* outputFile = new TFile("output.root", "RECREATE");
    //TTree* tree = new TTree("Hits", "Sensitive Detector Data");
    //tree->Branch("cellID", &sensDet->cellID, "cellID/I");
    //tree->Branch("depositedEnergy", &sensDet->depositedEnergy, "depositedEnergy/D");

    //sensDet->outputFile = outputFile;
    //sensDet->tree = tree;
}

