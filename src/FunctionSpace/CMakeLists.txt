add_definitions (${PETSC_DEFINITIONS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Geometry)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Trace)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Method)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/FunctionSpace)

add_library(FunctionSpace
FunctionSpace.cpp)

target_link_libraries (FunctionSpace Geometry ${PETSC_LIBRARIES})
