cmake_minimum_required(VERSION 3.12)
project(Prueba LANGUAGES CXX)

# Establecer flags del compilador (puede ser útil en la Raspberry Pi Zero 2W)
set(CMAKE_CXX_FLAGS "" CACHE STRING "Flags for C++ compiler" FORCE)

# Configuración de OpenCV
find_package(OpenCV 4.7.0 REQUIRED)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")

# Buscar wiringPi
find_library(WIRINGPI_LIB wiringPi)
if(WIRINGPI_LIB)
    message(STATUS "Found wiringPi library: ${WIRINGPI_LIB}")
else()
    message(WARNING "wiringPi library not found. Proceeding without it.")
endif()

# Crear ejecutable
add_executable(Prueba Prueba.cpp)
target_include_directories(Prueba PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(Prueba PRIVATE ${OpenCV_LIBS} ${WIRINGPI_LIB})
