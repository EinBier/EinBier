cmake_minimum_required(VERSION 2.8.12)

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
      "In-source builds are not allowed.
      1- Please make a new directory (called e.g. build/) [cmd: mkdir build/ ]
      2- Then change directory to this new one            [cmd: cd build     ]
      3- And run CMake from there.                        [cmd: cmake ..     ]\n
      (do not forget: you may need to remove the create files by CMake)
                (i.e. CMakeCache.txt and CMakeFiles/)
             [cmd: rm -r CMakeCache.txt and CMakeFiles/   ]")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules/ )

# ==============
project(EinBier CXX C)
# ==============

#should be mandatory ?
option(ENABLE_PETSC "Compile with PETSc (Mat/Vec+LinSol)" ON)
option(ENABLE_MPI "Compile with MPI" OFF)
option(ENABLE_OMP "Compile with OpenMP" OFF)

if(ENABLE_PETSC)
    set (PETSC_EXECUTABLE_RUNS YES CACHE BOOL
      "Disable checking if this setup works" FORCE)
    add_definitions(-DHAVE_PETSC)
    find_package(PETSc)
    include_directories(${PETSC_INCLUDES})
    #To see the library path for PETSc
    mark_as_advanced(PETSC_LIBS)
endif(ENABLE_PETSC)

if(ENABLE_MPI)
    find_package(MPI REQUIRED)
    include_directories(${MPI_INCLUDE_PATH})
    if(MPI_FOUND)
        set(CMAKE_CXX_COMPILER mpicxx)
        add_definitions(-DHAVE_MPI)
    endif(MPI_FOUND)
endif(ENABLE_MPI)

if(ENABLE_OMP)
    find_package(OpenMP)
    if (OPENMP_FOUND)
        add_definitions(-DHAVE_OMP)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    endif(OPENMP_FOUND)
endif(ENABLE_OMP)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_subdirectory(src)
