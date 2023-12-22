cmake_minimum_required(VERSION 3.13)

project(OS_lab)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false ${SOURCE_DIR}/*.cpp)

add_executable(${CMAKE_PROJECT_NAME}_exe main.cpp
        src/Derivative.cpp
        PiCount.cpp)

# Создаем динамическую библиотеку
add_library(MyLibrary SHARED PiCount.cpp)

# Связываем исполняемый файл с библиотекой
target_link_libraries(${CMAKE_PROJECT_NAME}_exe MyLibrary)