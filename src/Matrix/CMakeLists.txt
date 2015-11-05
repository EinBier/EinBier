add_definitions (${PETSC_DEFINITIONS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Common)

add_library(Matrix
Matrix.cpp)

target_link_libraries (Matrix ${PETSC_LIBRARIES})
