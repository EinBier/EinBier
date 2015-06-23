if(HAVE_PETSC)
    add_definitions (${PETSC_DEFINITIONS})
endif(HAVE_PETSC)

add_library(BIO
  )

target_link_libraries(BIO) # ${PETSC_LIBRARIES}
