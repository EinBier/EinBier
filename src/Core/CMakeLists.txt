if(HAVE_PETSC)
    add_definitions (${PETSC_DEFINITIONS})
endif(HAVE_PETSC)

add_library(Core
Barman.cpp
Operator.cpp
OperatorUtils.cpp
BIO.cpp
Matrix.cpp
)

target_link_libraries(Core) # ${PETSC_LIBRARIES}
