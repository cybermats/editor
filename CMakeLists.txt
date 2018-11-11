cmake_minimum_required(VERSION 3.12)
project(hello_world_gtk)

list(APPEND CMAKE_MODULE_PATH
     ~/code/GCR_CMake/macros)

include(GlibCompileResourcesSupport)

FIND_PACKAGE(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

link_directories(${GTKMM_LIBRARY_DIRS})

include_directories(${GTKMM_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)

set(RESOURCE_LIST
        STRIPBLANKS mywindow.ui
    )

compile_gresources(RESOURCE_FILE
                   XML_OUT
                   TYPE EMBED_C
                   RESOURCES ${RESOURCE_LIST}
        PREFIX "/org/mats/exampleapp"
        TARGET resources.c
        )

message(${RESOURCE_FILE})
message(${XML_OUT})

add_custom_target(resource ALL DEPENDS ${RESOURCE_FILE})

add_executable(hello_world_gtk
        main.cpp
        mytextarea.cpp
        mytextarea.h
        mytextbuffer.cpp
        mytextbuffer.h
        myapplication.cpp
        myapplication.h
        myappwindow.cpp
        myappwindow.h
        ${RESOURCE_FILE}
        mytextview.cpp
        mytextview.h
        formatter/formatter.cpp
        formatter/formatter.h
        formatter/style.cpp
        formatter/style.h
        formatter/stylizer.cpp
        formatter/stylizer.h
        formatter/pangostylizer.cpp
        formatter/pangostylizer.h
        formatter/regexformatter.cpp
        formatter/regexformatter.h
        formatter/stylefactory.h
        formatter/stylemap.h
        formatter/stylelist.h
        )

target_link_libraries(hello_world_gtk ${GTKMM_LIBRARIES}  )