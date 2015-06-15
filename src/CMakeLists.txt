add_definitions (${PETSC_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(EinBier
EinBier.cpp)

#unsure if this is good 
if(HAVE_MPI)
	target_link_libraries(Test ${MPI_CXX_LIBRARIES} ${PETSC_LIBRARIES})
else(HAVE_MPI)
	target_link_libraries(Test ${PETSC_LIBRARIES})
endif(HAVE_MPI)
