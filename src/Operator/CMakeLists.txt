add_definitions (${PETSC_DEFINITIONS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Trace)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Operator)

add_library(Operator
Operator.cpp)

target_link_libraries (Operator Trace ${PETSC_LIBRARIES})
