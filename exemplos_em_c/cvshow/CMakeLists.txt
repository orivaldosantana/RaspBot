cmake_minimum_required (VERSION 2.8) 
project (raspicam_test)
set(raspicam_DIR "/usr/local/lib/cmake")
find_package(raspicam REQUIRED)
find_package(OpenCV REQUIRED)
IF  ( OpenCV_FOUND AND raspicam_CV_FOUND)
MESSAGE(STATUS "COMPILING OPENCV TEST")
add_executable (teste_cv teste_imshow.cpp)  
target_link_libraries (teste_cv ${raspicam_CV_LIBS} ${OpenCV_LIBS})
ELSE()
MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()
