add_definitions (${PETSC_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/Common)
include_directories(${CMAKE_SOURCE_DIR}/src/Geometry)

#Build subdirectories
add_subdirectory(Common)
add_subdirectory(Geometry)

add_executable(EinBier
EinBier.cpp)

#unsure if this is good 
if(HAVE_MPI)
	target_link_libraries(EinBier Common Geometry ${MPI_CXX_LIBRARIES} ${PETSC_LIBRARIES})
else(HAVE_MPI)
	target_link_libraries(EinBier Common Geometry ${PETSC_LIBRARIES})
endif(HAVE_MPI)
