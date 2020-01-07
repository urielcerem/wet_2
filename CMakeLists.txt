cmake_minimum_required(VERSION 3.13.4)
project(wet_1)

set(CMAKE_CXX_STANDARD 11)

add_executable(wet_2 main2.cpp library2.h
        Array.h AVL.h Union-Find.h hash_table.h
        DataCenterAndServer2.cpp DataCenterAndServer2.h
        DataStructureManager.h DataStructureManager.cpp)