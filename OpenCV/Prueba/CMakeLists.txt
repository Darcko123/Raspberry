cmake_minimum_required(VERSION 3.12)
project(Prueba LANGUAGES CXX)

# Release (-O3) por defecto: crítico en la Raspberry Pi Zero 2W
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenCV 4 REQUIRED COMPONENTS core)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")

add_executable(${PROJECT_NAME} Prueba.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
