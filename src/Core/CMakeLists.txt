add_definitions (${PETSC_DEFINITIONS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Message)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/BIO)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core)

add_library(Core
Operator.cpp
)

target_link_libraries (Core ${PETSC_LIBRARIES})
