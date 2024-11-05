# Find Geant4 package
option(WITH_GEANT4_UIVIS "Build MuonCollider Project with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# Include Geant4 use file
include(${Geant4_USE_FILE})

# Define sources and headers
file(GLOB GEANT4_SOURCES
  ${PROJECT_SOURCE_DIR}/GEANT4_MuonCollider/src/*.cc
  ${PROJECT_SOURCE_DIR}/GEANT4_MuonCollider/src/DetectorGeometry/*.cc
)

# Create library
add_library(GEANT4Lib ${GEANT4_SOURCES})

# Set target properties
target_include_directories(GEANT4Lib PUBLIC
  ${PROJECT_SOURCE_DIR}/GEANT4_MuonCollider/include
  ${PROJECT_SOURCE_DIR}/ROOT_Functions/include
  ${PROJECT_SOURCE_DIR}/GEANT4_MuonCollider/include/DetectorGeometry
)

# Link Geant4 libraries
target_link_libraries(GEANT4Lib ${Geant4_LIBRARIES})

