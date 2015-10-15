if(HAVE_PETSC)
    add_definitions (${PETSC_DEFINITIONS})
endif(HAVE_PETSC)

# add all these directories to all subdirectories (c++ -I)
include_directories(${CMAKE_SOURCE_DIR}/src/Common)
include_directories(${CMAKE_SOURCE_DIR}/src/Geometry)
include_directories(${CMAKE_SOURCE_DIR}/src/Trace)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Method)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Barman)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Operator)

#Build subdirectories
add_subdirectory(Common)
add_subdirectory(Geometry)
add_subdirectory(Trace)
add_subdirectory(Barman)
add_subdirectory(Operator)

#unsure if this is good
set(TARGET_LINK_LIB Common Geometry Trace Barman Operator)

if(HAVE_MPI)
    set(${TARGET_LINK_LIB} ${TARGET_LINK_LIB}  ${MPI_CXX_LIBRARIES})
endif(HAVE_MPI)

if(HAVE_PETSC)
    set(${TARGET_LINK_LIB} ${TARGET_LINK_LIB}  ${PETSC_LIBRARIES})
endif(HAVE_PETSC)


add_executable(EinBier
EinBier.cpp)
target_link_libraries(EinBier ${TARGET_LINK_LIB})

add_executable(testTrace
testTrace.cpp)
target_link_libraries(testTrace ${TARGET_LINK_LIB})

add_executable(testBier
testBier.cpp)
target_link_libraries(testBier ${TARGET_LINK_LIB})
