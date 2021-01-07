cmake_minimum_required(VERSION 3.10)

# set the project name
project(BeadSorterSimulator)

add_subdirectory(ArduinoStub)
add_subdirectory(../BeadSorter BeadSorter)

# add the executable
add_executable(BeadSorterSimulator
    main.cpp
    window.h
    window.cpp
    windowSize.h
    colors.h
    colors.cpp
    textRender.h
    textRender.cpp
    characters.h
    )

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(BeadSorterSimulator ArduinoStub BeadSorter glfw pthread freetype ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS})
include_directories (/usr/include/freetype2)
