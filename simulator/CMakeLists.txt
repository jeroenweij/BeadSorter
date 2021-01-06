cmake_minimum_required(VERSION 3.10)

# set the project name
project(BeadSorterSimulator)

add_subdirectory(ArduinoStub)
add_subdirectory(../BeadSorter BeadSorter)

# add the executable
add_executable(BeadSorterSimulator
    main.cpp
    )
target_link_libraries(BeadSorterSimulator ArduinoStub BeadSorter)
