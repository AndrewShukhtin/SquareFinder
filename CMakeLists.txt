cmake_minimum_required(VERSION 3.10)
project(CirclesWithinSquares)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

set(INCLUDE_DIRS project/criteriaSquareSelector/
                 project/shapes/)

set(HEADERS project/criteriaSquareSelector/criteriaSquareSelector.hpp
            project/criteriaSquareSelector/config.hpp
            project/shapes/circle.hpp
            project/shapes/square.hpp
            project/shapes/types.hpp)

set(SRC project/criteriaSquareSelector/criteriaSquareSelector.cpp
            project/shapes/circle.cpp
            project/shapes/square.cpp)

add_executable(main project/main.cpp ${SRC} ${HEADERS})
target_include_directories(main PRIVATE ${OpenCV_INCLUDE_DIRS} ${INCLUDE_DIRS})
target_link_libraries(main ${OpenCV_LIBS})
