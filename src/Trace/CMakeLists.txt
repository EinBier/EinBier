add_definitions (${PETSC_DEFINITIONS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Geometry)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Trace)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Method)

add_library(Trace
Trace.cpp
VectorTrace.cpp)

target_link_libraries (Trace Geometry ${PETSC_LIBRARIES})
