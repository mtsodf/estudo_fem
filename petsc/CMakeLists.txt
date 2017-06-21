cmake_minimum_required (VERSION 3.1)
project (Estudo_FEM)

message("Variaveis do PETSC")
message("PETSC_DIR = " ${PETSC_DIR} )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
find_package(PETSc)
include_directories(${PETSC_INCLUDES})

find_package(MPI)

include_directories(${MPI_C_INCLUDE_PATH})

add_subdirectory(googletest)

add_executable(CriandoVetor CriandoVetor.cpp no.cpp)
target_link_libraries(CriandoVetor ${PETSC_LIBRARIES} ${MPI_LIBRARIES})


add_executable(CriarMalha CriarMalha.cpp)


add_executable(sampledsa1 sample.cc sample_unittest.cc)
target_link_libraries(sampledsa1 gtest_main)

