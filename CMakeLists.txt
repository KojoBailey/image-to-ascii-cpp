cmake_minimum_required(VERSION 3.28)
project(Image_To_ASCII C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

file(GLOB SOURCES 
  src/main.cpp
	src/image.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})