cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(editor VERSION 0.1 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(gtkmain)
add_subdirectory(libeditor)
