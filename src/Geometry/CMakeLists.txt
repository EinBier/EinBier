add_definitions (${PETSC_DEFINITIONS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Geometry)

add_library(Geometry
Geometry.cpp
Circle.cpp
)

target_link_libraries (Geometry ${PETSC_LIBRARIES})
