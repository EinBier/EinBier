if(HAVE_PETSC)
    add_definitions (${PETSC_DEFINITIONS})
endif(HAVE_PETSC)

# add all these directories to all subdirectories (c++ -I)
include_directories(${CMAKE_SOURCE_DIR}/src/Common)
include_directories(${CMAKE_SOURCE_DIR}/src/Geometry)
include_directories(${CMAKE_SOURCE_DIR}/src/BIO)
include_directories(${CMAKE_SOURCE_DIR}/src/Core)

#Build subdirectories
add_subdirectory(Common)
add_subdirectory(Geometry)
add_subdirectory(Core)

add_executable(EinBier
EinBier.cpp)

#unsure if this is good
set(TARGET_LINK_LIB Common Geometry Core)

if(HAVE_MPI)
    set(${TARGET_LINK_LIB} ${TARGET_LINK_LIB}  ${MPI_CXX_LIBRARIES})
endif(HAVE_MPI)

if(HAVE_PETSC)
    set(${TARGET_LINK_LIB} ${TARGET_LINK_LIB}  ${PETSC_LIBRARIES})
endif(HAVE_PETSC)

target_link_libraries(EinBier ${TARGET_LINK_LIB})
add_executable(Bert
Bert.cpp)

target_link_libraries(Bert ${TARGET_LINK_LIB})
