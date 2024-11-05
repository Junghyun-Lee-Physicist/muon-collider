#ROOT CMakeLists.txt

# Find ROOT package
find_package(ROOT REQUIRED)

# List ROOT source files
file(GLOB ROOT_SOURCES
    ${PROJECT_SOURCE_DIR}/ROOT_Functions/src/*.cpp
)
file(GLOB ROOT_HEADERS
    ${PROJECT_SOURCE_DIR}/ROOT_Functions/include/*.hh
)

# Create library
add_library(ROOTLib ${ROOT_SOURCES} ${ROOT_HEADERS})

# Define include directories for ROOT
target_include_directories(ROOTLib PUBLIC
    ${ROOT_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link ROOT libraries
target_link_libraries(ROOTLib ${ROOT_LIBRARIES})

