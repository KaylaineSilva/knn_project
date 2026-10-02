cmake_minimum_required(VERSION 3.16)

project(knn_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

#procura a instalação do HDF5 e sua api C++
find_package(HDF5 REQUIRED COMPONENTS CXX)

add_executable(knn_project
    src/main.cpp
)

# Diz onde estão os headers do HDF5
target_include_directories(
    knn_project
    PRIVATE
    ${HDF5_INCLUDE_DIRS}
)

# Liga nosso executável às bibliotecas HDF5
target_link_libraries(
    knn_project
    PRIVATE
    ${HDF5_CXX_LIBRARIES}
)