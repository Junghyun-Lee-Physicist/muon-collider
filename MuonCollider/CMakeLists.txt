cmake_minimum_required(VERSION 3.14...3.29)

project(MuonCollider)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add subdirectories
add_subdirectory(GEANT4_MuonCollider)
add_subdirectory(ROOT_Functions)

# List your main source file
set(MAIN_SOURCES ${PROJECT_SOURCE_DIR}/main.cc)

# Create executable
add_executable(ExeMuonCollider ${MAIN_SOURCES})

# Link libraries from subdirectories
target_link_libraries(ExeMuonCollider GEANT4Lib ROOTLib ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# Include directories for the executable
target_include_directories(ExeMuonCollider PUBLIC
    ${PROJECT_SOURCE_DIR}/GEANT4_MuonCollider/include
    ${PROJECT_SOURCE_DIR}/GEANT4_MuonCollider/include/DetectorGeometry
    ${PROJECT_SOURCE_DIR}/ROOT_Functions/include
)

# Copy scripts to the buildTemp directory
set(SCRIPTS
  run.mac
  vis.mac
)

foreach(_script ${SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/GEANT4_MuonCollider/${_script}
    ${CMAKE_BINARY_DIR}/${_script}
    COPYONLY
  )
endforeach()

# Install the executable and scripts
install(TARGETS ExeMuonCollider DESTINATION .)
install(FILES ${CMAKE_BINARY_DIR}/run.mac DESTINATION .)
install(FILES ${CMAKE_BINARY_DIR}/vis.mac DESTINATION .)

