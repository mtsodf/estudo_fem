cmake_minimum_required (VERSION 3.1)
project (Estudo_FEM)

message("Variaveis do PETSC")
message("PETSC_DIR = " ${PETSC_DIR} )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")

find_package(PETSc)
include_directories(${PETSC_INCLUDES})

find_package(MPI)
include_directories(${MPI_C_INCLUDE_PATH})

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-lpthread")

add_subdirectory(googletest)

add_executable(Laplaciano Laplaciano.cpp fem.cpp)
target_link_libraries(Laplaciano ${PETSC_LIBRARIES} ${MPI_LIBRARIES})


add_executable(CriarMalha CriarMalha.cpp)


add_executable(fem_unittest fem.cpp fem_unittest.cpp)
target_link_libraries(fem_unittest gtest_main)

configure_file(plot_sol.py plot_sol.py)
configure_file(gerar_exemplos.sh gerar_exemplos.sh)


