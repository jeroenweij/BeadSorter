
set(FILE_SEARCH_LIST
    *.cpp
    *.c
    *.h
    *.ino)


file(GLOB LIB_FILES ${FILE_SEARCH_LIST})

add_library(BeadSorter ${LIB_FILES})

target_link_libraries(BeadSorter ArduinoStub)

target_include_directories(BeadSorter PUBLIC .)
