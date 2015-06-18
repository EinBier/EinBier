add_definitions (${PETSC_DEFINITIONS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/BIO)

add_library(BIO
BIO.cpp)

target_link_libraries (BIO ${PETSC_LIBRARIES})
