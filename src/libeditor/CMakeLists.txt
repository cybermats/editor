project(libeditor VERSION 0.1 LANGUAGES CXX)

add_library(libeditor STATIC
  src/formatter.cpp
  src/mytextbuffer.cpp
  src/mytextview.cpp
  src/regexformatter.cpp
  src/stylizer.cpp
  src/style.cpp
  src/styletype.cpp
  src/selectionformatter.cpp
  )


target_include_directories(libeditor PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE src)

install(TARGETS libeditor EXPORT MyLibraryConfig
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT MyLibraryConfig DESTINATION share/libeditor/cmake)

export(TARGETS libeditor FILE MyLibraryConfig.cmake)

