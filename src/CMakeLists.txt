add_definitions (${PETSC_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/Common)
include_directories(${CMAKE_SOURCE_DIR}/src/Geometry)
include_directories(${CMAKE_SOURCE_DIR}/src/Core)

#Build subdirectories
add_subdirectory(Common)
add_subdirectory(Geometry)
add_subdirectory(Core)

add_executable(EinBier
EinBier.cpp)

#unsure if this is good 
if(HAVE_MPI)
	target_link_libraries(EinBier Common Geometry Core ${MPI_CXX_LIBRARIES} ${PETSC_LIBRARIES})
else(HAVE_MPI)
	target_link_libraries(EinBier Common Geometry Core ${PETSC_LIBRARIES})
endif(HAVE_MPI)
