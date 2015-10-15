add_definitions (${PETSC_DEFINITIONS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Barman)

add_library(Barman
Barman.cpp
Bier.cpp)

target_link_libraries (Barman Common ${PETSC_LIBRARIES})
