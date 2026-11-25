cmake_minimum_required(VERSION 3.12)
project(FaceDetect LANGUAGES CXX)

# Release (-O3) por defecto: crítico en la Raspberry Pi Zero 2W
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenCV 4 REQUIRED COMPONENTS core imgproc highgui objdetect videoio)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")

add_executable(${PROJECT_NAME} FaceDetect.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)

# Recurso que el ejemplo carga desde el directorio de trabajo (build/)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/haarcascade_frontalface_alt.xml ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
