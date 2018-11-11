project(editor VERSION 0.1 LANGUAGES CXX C)


list(APPEND CMAKE_MODULE_PATH
     "${CMAKE_SOURCE_DIR}/thirdparty/GCR_CMake/macros")

include(GlibCompileResourcesSupport)

FIND_PACKAGE(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})

include_directories(${GTKMM_INCLUDE_DIRS})

set(RESOURCE_LIST
        STRIPBLANKS assets/mywindow.ui
    )

compile_gresources(RESOURCE_FILE
                   XML_OUT
                   TYPE EMBED_C
                   RESOURCES ${RESOURCE_LIST}
        PREFIX "/org/mats/editor"
        TARGET resources.c
        )

set(RESOURCE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_FILE}")

message("Project: ${PROJECT_SOURCE_DIR}")
message("CMake: ${CMAKE_SOURCE_DIR}")

add_custom_target(resource ALL DEPENDS ${RESOURCE_PATH})

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/mytextarea.cpp
        src/mytextarea.h
        src/myapplication.cpp
        src/myapplication.h
        src/myappwindow.cpp
        src/myappwindow.h
        src/pangostylizer.cpp
        ${RESOURCE_PATH}
        )


target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES} libeditor )
