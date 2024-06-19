cmake_minimum_required(VERSION 3.25)
project(MatDisp)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(MatDisp
        include/Mat.h
        include/Node.h
        include/Element.h
        include/Structure.h
        src/Node.cpp
        src/Element.cpp
        src/Structure.cpp
        test/main.cpp)
