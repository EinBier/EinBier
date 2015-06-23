if(HAVE_PETSC)
    add_definitions (${PETSC_DEFINITIONS})
endif(HAVE_PETSC)

#do not know if we must put this or not ...
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core)

add_library(Core
OperatorHandler.cpp
Operator.cpp
OperatorUtils.cpp
BIO.cpp
Matrix.cpp
)

target_link_libraries(Core) # ${PETSC_LIBRARIES}
