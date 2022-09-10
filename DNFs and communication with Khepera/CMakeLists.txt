cmake_minimum_required(VERSION 3.5)

project(Project)
set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++11")
set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH /home/eloy/Qt/5.7/gcc_64)

find_package(OpenCV REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Charts REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

macro(add_lib libname)
	file(GLOB lib_headers "${libname}/include/*.hpp") # this is only for Qt Creator to show the files
	file(GLOB lib_sources "${libname}/src/*.cpp")
	add_library(${libname} ${lib_headers} ${lib_sources})
	include_directories("${libname}/include")
endmacro(add_lib)

add_lib(neural_fields)
target_link_libraries(neural_fields Qt5::Widgets)

add_lib(communication)
target_link_libraries(communication neural_fields)

add_lib(gui)
target_link_libraries(gui neural_fields communication Qt5::Charts)

file(GLOB APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/app app/*.cpp)
foreach(appsourcefile ${APP_SOURCES})
    string(REPLACE ".cpp" "" appname ${appsourcefile})
    add_executable(${appname} app/${appsourcefile})
    target_link_libraries(${appname} neural_fields communication gui ${OpenCV_LIBS})
endforeach(appsourcefile ${APP_SOURCES})
