cmake_minimum_required(VERSION 3.16)

project(knn_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

#procura a instalação do HDF5 e sua api C++
find_package(HDF5 REQUIRED COMPONENTS CXX)
find_package(TBB REQUIRED)

#Adiciona o executável principal (sequencial)
add_executable(knn_seq
    src/main.cpp
)

# Adiciona o executável paralelo
add_executable(knn_par
    src/parallel_main.cpp
)

# Informar onde estão os cabeçalhos do HDF5
target_include_directories(knn_seq PRIVATE ${HDF5_INCLUDE_DIRS})
target_include_directories(knn_par PRIVATE ${HDF5_INCLUDE_DIRS})

# Vincular as bibliotecas
target_link_libraries(knn_seq PRIVATE
    ${HDF5_CXX_LIBRARIES}
)

#Liga o executável paralelo às bibliotecas HDF5 e TBB
target_link_libraries(knn_par PRIVATE
    ${HDF5_CXX_LIBRARIES}
    TBB::tbb
)