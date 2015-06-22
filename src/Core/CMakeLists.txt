if(HAVE_PETSC)
    add_definitions (${PETSC_DEFINITIONS})
endif(HAVE_PETSC)

add_library(Core
Operator.cpp
OperatorUtils.cpp
Matrix.cpp
)

target_link_libraries(Core) # ${PETSC_LIBRARIES}
